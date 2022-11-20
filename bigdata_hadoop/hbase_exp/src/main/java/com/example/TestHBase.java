package com.example;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.*;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.util.Bytes;

import java.io.IOException;

public class TestHBase{

    public static Configuration configuration;
    public static Connection connection;
    public static Admin admin;

    public static void main(String[] args) throws IOException{
        createTale(null, args);
    }

    // establish connection 
    public static void init(){
        configuration = HBaseConfiguration.create();
        configuration.set("hbase.rootdir", "hdfs://localhost:9000/hbase");
        try{
            connection = ConnectionFactory.createConnection(configuration);
            admin = connection.getAdmin();
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    // disconnect 
    public static void close(){
        try{
            if(admin != null){
                admin.close();
            }
            if(null != connection){
                connection.close();
            }
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    // create a table
    public static void createTale(String myTableNaString, String[] colFamilyStrings) throws IOException{
        init();
        TableName tableName = TableName.valueOf(myTableNaString);
        if(admin.tableExists(tableName)){
            System.out.println("The tables exists!");
        }else{
            TableDescriptorBuilder tableDescriptorBuilder = TableDescriptorBuilder.newBuilder(tableName);

            // set columns 
            for(String str:colFamilyStrings){
                ColumnFamilyDescriptor columnFamilyDescriptor = ColumnFamilyDescriptorBuilder.of(str);
                tableDescriptorBuilder.setColumnFamily(columnFamilyDescriptor);
            }

            // build table descriptor
            TableDescriptor tableDescriptor = tableDescriptorBuilder.build();
            
            // create table
            admin.createTable(tableDescriptor);

            close();
        }
    }

    // add data to table
    public static void insertData(String taleNameString, String rowkeyString, String colFamilyString, String colString, String valString) throws IOException{
        init();
        Table table = connection.getTable(TableName.valueOf(taleNameString));
        Put put = new Put(Bytes.toBytes(rowkeyString));
        put.addColumn(Bytes.toBytes(rowkeyString), Bytes.toBytes(colString), Bytes.toBytes(valString));
        table.put(put);
        table.close();
        close();
    }

    // scan the data
    public static void scanData(String tableNameString, String rowkeyString, String colFamilyString, String colString) throws IOException{
        init();

        Table table = connection.getTable(TableName.valueOf(tableNameString));
        Get get = new Get(Bytes.toBytes(rowkeyString));
        get.addColumn(Bytes.toBytes(colFamilyString), Bytes.toBytes(colString));
        Result result = table.get(get);
        System.out.println(new String(result.getValue(colFamilyString.getBytes(), colString == null ? null : colString.getBytes())));
        table.close();

        close();
    }

}