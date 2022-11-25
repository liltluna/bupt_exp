package com.example;

import org.apache.spark.sql.SparkSession;
import org.apache.spark.sql.Dataset;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.SparkConf;

public class App {
  public static void main(String[] args) {
    SparkConf conf = new SparkConf().setAppName("average calculate").setMaster("local");
    JavaSparkContext sc = new JavaSparkContext(conf);

    
    // Should be some file on your system
    String logFile = "/usr/local/spark/README.md"; 
    SparkSession spark = SparkSession.builder().appName("Average Calculate").getOrCreate();
    Dataset<String> logData = spark.read().textFile(logFile).cache();

    long numAs = logData.count();
    // long numBs = logData.filter(s -> s.contains("b")).count();

    System.out.println("Lines with a: " + numAs + ", lines with b: " + numAs);

    spark.stop();
  }
}