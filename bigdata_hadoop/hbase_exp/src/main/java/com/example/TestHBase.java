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
        String tableNameString = "StuInfo";
        String[] colFamilyStrings = {"SID", "SI", "Ma", "CS", "Eng"};
    
        String[] infoFields = {"SI:name", "SI:sex", "SI:age", "Ma:id", "Ma:credit", "Ma:score", "CS:id", "CS:credit", "CS:score", "Eng:id", "Eng:credit", "Eng:score"};

        String[] info_1 = {"Zhangsan", "male", "23", "123001", "2", "86", "123002", "5", "", "123003", "3", "69"};
        String[] info_2 = {"Mary", "female", "22", "123001", "2", "", "123002", "5", "77", "123003", "3", "99"};
        String[] info_3 = {"Lisi", "male", "24", "123001", "2", "98", "123002", "5", "95", "123003", "3", ""};

        System.out.println("Start the progress of creating a table named 'StuInfo'.\n");
        createTable(tableNameString, colFamilyStrings);

        System.out.println("Add record of stu-2015001");
        addRecord(tableNameString, "2015001", infoFields, info_1);

        System.out.println("Add record of stu-2015002");
        addRecord(tableNameString, "2015002", infoFields, info_2);
        
        System.out.println("Add record of stu-2015003");
        addRecord(tableNameString, "2015003", infoFields, info_3);
        
        System.out.println("\nScanning the data of colfamily 'SI'... \n");
        scanColumn(tableNameString, "SI");

        System.out.println("\nScanning the data of col 'Ma:score'... \n");
        scanColumn(tableNameString, "Ma:score");

        System.out.println("\nModifying data of the score of English(2015002)\n");
        modifyData(tableNameString, "2015002", "Eng:score", "100");
        System.out.println("\nCheck...");
        scanColumn(tableNameString, "Eng:score");

        System.out.println("\nDeleting the data of 2015003");
        deleteRow(tableNameString, "2015003");
        System.out.println("\nCheck...");
        scanColumn(tableNameString, "SI");
    }

    // establish connection
    public static void init() {
        configuration = HBaseConfiguration.create();
        configuration.set("hbase.rootdir","hdfs://localhost:9000/hbase");
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
    // if it exits, then delete it and create a new one.
    public static void createTable(String myTableNaString, String[] colFamilyStrings) throws IOException {
        // operate the strings
        // HashMap<String, List<String>> hashMap = new HashMap<>();
        // for(String field : colFamilyStrings){
        //     String[] res = field.split(":");
        //     if(hashMap.get(res[0]) == null){
        //         hashMap.put(res[0], new ArrayList<String>());
        //     }else{
        //         hashMap.get(res[0]).add(res[1]);
        //     }
        // }

        init();
        TableName tableName = TableName.valueOf(myTableNaString);
        if (admin.tableExists(tableName)) {
            System.out.println("The table exists and will be deleted!");
            admin.disableTable(tableName);
            admin.deleteTable(tableName);
        }

        HTableDescriptor hTableDescriptor = new HTableDescriptor(tableName);

        // set colFamilies for descriptor
        // Set<String> keySet = hashMap.keySet();
        for (String str : colFamilyStrings) {
            HColumnDescriptor hColumnDescriptor = new HColumnDescriptor(str);
            hTableDescriptor.addFamily(hColumnDescriptor);
        }

        // build table descriptor
        //TableDescriptor tableDescriptor = hTableDescriptor.build();

        // create table
        admin.createTable(hTableDescriptor);

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
            put = new Put(row.getBytes());
            String[] columns = fields[i].split(":");
            if(columns.length == 1){
                put.addColumn(columns[0].getBytes(), "".getBytes(), values[i].getBytes());
            }else{
                put.addColumn(columns[0].getBytes(), columns[1].getBytes(), values[i].getBytes());
            }
            table.put(put);
        }
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
            scan.addFamily(columnFamily.getBytes());
        }else{
            scan.addColumn(Bytes.toBytes(splitResult[0]), Bytes.toBytes(splitResult[1]));
        }
        resultScanner = table.getScanner(scan);
        for(Result r : resultScanner){
            Cell[] cells = r.rawCells();
            if(cells.length == 0) {
                System.out.println("null");
            }
            for(Cell cell : cells){
                
                System.out.println(new String(CellUtil.cloneRow(cell)) + "\t" + (CellUtil.cloneValue(cell).length == 0 ? "null" : new String(CellUtil.cloneValue(cell))) + "\t" + new String(CellUtil.cloneQualifier(cell))+ "\t" + new String(CellUtil.cloneFamily(cell)));
            }
        }
        resultScanner.close();
        close();
    }

    // modify the data
    public static void modifyData(String tableName, String row, String column, String value) throws IOException{
        init();
        Table table = connection.getTable(TableName.valueOf(tableName));
        Put put = new Put(row.getBytes());
        put.addColumn(column.split(":")[0].getBytes(), (column.split(":").length == 1 ? "".getBytes() : column.split(":")[1].getBytes()), value.getBytes());
        table.put(put);
        close();
    }

    // delete the row
    public static void deleteRow(String tableName, String row) throws IOException{
        init();
        Table table = connection.getTable(TableName.valueOf(tableName));
        Delete delete = new Delete(row.getBytes());
        table.delete(delete);
        close();
    }
}   