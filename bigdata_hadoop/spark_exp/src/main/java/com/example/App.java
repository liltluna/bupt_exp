package com.example;

import org.apache.spark.sql.SparkSession;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import org.apache.spark.sql.Dataset;

public class App {
  public static void main(String[] args) {
    // Should be some file on your system
    String logFiles[] = {"input/db.txt", "input/ds.txt", "input/os.txt"}; 

    // N records the number of course
    int N = logFiles.length;

    HashMap<String, ArrayList<Integer>> hashMapGrades = new HashMap<String, ArrayList<Integer>>();
    List<String> allDataList = new ArrayList<String>();
    SparkSession spark = SparkSession.builder().appName("Average Conputation").getOrCreate();

    for(String logFile : logFiles){
      Dataset<String> logData = spark.read().textFile(logFile).cache();
      allDataList.addAll(logData.collectAsList());
    }

    for(String data : allDataList){
      String splitData[] = data.split(" ");
      // if a student is new, create a new list to storage
      if(hashMapGrades.get(splitData[0]) == null) {
        hashMapGrades.put(splitData[0], new ArrayList<Integer>());
      }
      hashMapGrades.get(splitData[0]).add(Integer.parseInt(splitData[1]));
    }

    List<String> resultInList = new ArrayList<String>();

    for(String key : hashMapGrades.keySet()){
        int sum = 0;
        for(int grade : hashMapGrades.get(key)){
            sum += grade;
        }
        resultInList.add(String.format("%s,%.2f\n", key, (double)sum / N));
    }
    try{
        BufferedWriter out = new BufferedWriter(new FileWriter("output/result.txt"));
        for(String str : resultInList){
          out.write(str);
        }
        out.close();
    }catch(Exception e){}
    spark.stop();
  }
}