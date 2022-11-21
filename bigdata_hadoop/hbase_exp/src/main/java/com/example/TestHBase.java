package com.example;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.*;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.util.Bytes;
import java.io.IOException;

public class TestHBase {

    public static Configuration configuration;
    public static Connection connection;
    public static Admin admin;

    public static void main(String[] args) throws IOException {
        String tableNameString = "Students-Course-SC";
        String[] colFamilyStrings = {"Student", "SC", "Course"};
        System.out.println("going in ");
        // String[] studentColStrings = {"S_Name", "S_Sex", "S_Age"};
        // String[] courseColStrings = {"C_No", "C_Name", "C_Credit"};
        // String[] scColStrings = {"SC_Cno", "SC_Score"};

        createTale(tableNameString, colFamilyStrings);
        addRecord(tableNameString, "row1", colFamilyStrings, colFamilyStrings);
        scanColumn(tableNameString, "Student:Student");
        scanTest();

    }

    // establish connection
    public static void init() {
        configuration = HBaseConfiguration.create();
        configuration.set("hbase.zookeeper.quorum","*.*.*.*");
        configuration.set("hbase.zookeeper.property.clientPort","2181");
        try {
            connection = ConnectionFactory.createConnection(configuration);
            admin = connection.getAdmin();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // disconnect
    public static void close() {
        try {
            if (admin != null) {
                admin.close();
            }
            if (null != connection) {
                connection.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // create a table,
    // if it exits, then delete is and create a new one.
    public static void createTale(String myTableNaString, String[] colFamilyStrings) throws IOException {
        init();
        TableName tableName = TableName.valueOf(myTableNaString);
        if (admin.tableExists(tableName)) {
            System.out.println("The table exists and will be deleted!");
            admin.deleteTable(tableName);
        }

        TableDescriptorBuilder tableDescriptorBuilder = TableDescriptorBuilder.newBuilder(tableName);

        // set columns for descriptor
        for (String str : colFamilyStrings) {
            ColumnFamilyDescriptor columnFamilyDescriptor = ColumnFamilyDescriptorBuilder.of(str);
            tableDescriptorBuilder.setColumnFamily(columnFamilyDescriptor);
        }

        // build table descriptor
        TableDescriptor tableDescriptor = tableDescriptorBuilder.build();

        // create table
        admin.createTable(tableDescriptor);

        close();
    }

    // add data to an attribute of a row
    public static void insertData(String taleNameString, String rowkeyString, String colFamilyString, String colString,
            String valString) throws IOException {
        init();
        Table table = connection.getTable(TableName.valueOf(taleNameString));
        Put put = new Put(Bytes.toBytes(rowkeyString));
        put.addColumn(Bytes.toBytes(rowkeyString), Bytes.toBytes(colString), Bytes.toBytes(valString));
        table.put(put);
        table.close();
        close();
    }

    // add a record to a table
    public static void addRecord(String tableName, String row, String[] fields, String[] values) throws IOException {
        init();
        Table table = connection.getTable(TableName.valueOf(tableName));
        Put put = new Put(Bytes.toBytes(row));

        for (int i = 0; i < fields.length; i++) {
            put.addColumn(Bytes.toBytes(row), Bytes.toBytes(fields[i]), i < values.length ? Bytes.toBytes(values[i]) : Bytes.toBytes(""));
        }

        table.put(put);
        table.close();
        close();
    }

    // scan the data
    public static void scanData(String tableNameString, String rowkeyString, String colFamilyString, String colString)
            throws IOException {
        init();

        Table table = connection.getTable(TableName.valueOf(tableNameString));
        Get get = new Get(Bytes.toBytes(rowkeyString));
        get.addColumn(Bytes.toBytes(colFamilyString), Bytes.toBytes(colString));
        Result result = table.get(get);
        System.out.println(new String(
                result.getValue(colFamilyString.getBytes(), colString == null ? null : colString.getBytes())));
        table.close();

        close();
    }

    // scan a column or a column family
    public static void scanColumn(String tableName, String columnFamily) throws IOException{
        init();
        Table table = connection.getTable(TableName.valueOf(tableName));
        Scan scan = new Scan();
        ResultScanner resultScanner = table.getScanner(scan);
        String[] splitResult = columnFamily.split(":");
        if(splitResult.length == 1){
            // scan.addFamily(Bytes.toBytes(columnFamily));
        }
        else{
            scan.addColumn(Bytes.toBytes(splitResult[0]), Bytes.toBytes(splitResult[1]));
            resultScanner = table.getScanner(scan);
            for(Result r : resultScanner){
                System.out.println("\n row: "+new String(r.getRow()));
            }
        }
        resultScanner.close();
        close();
    }

    public static void scanTest() throws IOException{
        init();
        Table table = connection.getTable(TableName.valueOf("Students-Course-SC"));
        Scan scan = new Scan();
        ResultScanner resultScanner = table.getScanner(scan);
        for(Result result : resultScanner){
            System.out.println(new String(result.getRow()));
        }
        close();
    }

}