import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.partition.HashPartitioner;

import org.apache.hadoop.util.GenericOptionsParser;

public class InvertedIndex{
    public static class InvertedIndexMapper extends Mapper<Object, Text, Text, IntWritable>{
        private final static IntWritable one = new IntWritable(1);

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException{
            FileSplit fileSplit = (FileSplit) context.getInputSplit();
            String fileName = fileSplit.getPath().getName().toLowerCase();
            int pos = fileName.indexOf(".");
            if(pos > 0){
                fileName = fileName.substring(0, pos);
            }
            Text word = new Text();
            StringTokenizer itr = new StringTokenizer(value.toString(), "\t\n\r\f ,.:-()?\"");
            while (itr.hasMoreTokens()) {
                word.set(itr.nextToken() + "#" + fileName);
                context.write(word, one);
            }
        }
    }

    public static class SumCombiner extends Reducer<Text, IntWritable, Text, IntWritable>{
        private final IntWritable result = new IntWritable();

        public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
            int sum = 0;
            for(IntWritable val : values){
                sum += val.get();
            }
            result.set(sum);
            context.write(key, result);
        }
    }

    public static class NewPartitioner extends HashPartitioner<Text, IntWritable>{
        public int getPartition(Text key, IntWritable value, int numReduceTasks){
            String term = key.toString().split("#")[0];
            return super.getPartition(new Text(term), value, numReduceTasks);
        }
    }

    public static class InvertedIndexReducer extends Reducer<Text, IntWritable, Text, Text>{
        private String last = "";
        private int countItem;
        private int countDoc;
        private StringBuilder out = new StringBuilder();
        private float frequency;

        public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException{
            String term = key.toString().split("#")[0];
            if(!term.equals(last)){
                if(!last.equals("")){
                    out.setLength(out.length() - 1);
                    frequency = (float)countItem / countDoc;
                    context.write(new Text(String.format("%-10s", last)), new Text(String.format("%.2f, %s", frequency, out.toString())));
                    countItem = 0;
                    countDoc = 0;
                    out = new StringBuilder();
                }
                last = term;
            }
            int sum = 0;
            for(IntWritable val : values){
                sum += val.get();
            }
            out.append(key.toString().split("#")[1]).append(":").append(sum).append("; ");
            countItem += sum;
            countDoc += 1;
        }

        public void cleanup(Context context) throws IOException, InterruptedException{
            out.setLength(out.length() - 1);
            frequency = (float)countItem / countDoc;
            context.write(new Text(String.format("%-10s", last)), new Text(String.format("%.2f, %s", frequency, out.toString())));
        }
    }

    public static void main(String[] args) throws  Exception{
        Configuration conf = new Configuration();
        String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
        Job job = new Job(conf, "inverted index");
        job.setJarByClass(InvertedIndex.class);
        job.setMapperClass(InvertedIndexMapper.class);
        job.setCombinerClass(SumCombiner.class);
        job.setReducerClass(InvertedIndexReducer.class);
        job.setNumReduceTasks(5);
        job.setPartitionerClass(NewPartitioner.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(IntWritable.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);
        FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
        FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}