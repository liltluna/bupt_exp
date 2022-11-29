import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.HashMap;

public class Master {
    HashMap<String, Integer> nodesMap;
    
    public static void main(String[] args) {
        
    }

    public void readData(){
        BufferedReader bufferedReader;
        String temString;
        String nodeFilePath = "data/nodeaddr.txt";
        File nodeFile = new File(nodeFilePath);
        nodesMap = new HashMap<>();
        
        // test if the file exists
        if(!nodeFile.exists()){
           System.out.println("File not exists."); 
        }

        try{
            bufferedReader = new BufferedReader(new FileReader(nodeFile));
            while((temString = bufferedReader.readLine()) != null){
                String[] paras = temString.split("\r");
                nodesMap.put(paras[0], Integer.parseInt(paras[1]));
            }
            bufferedReader.close();
        // not good, no detail
        }catch (Exception exception){
            System.out.println(exception.getStackTrace());
        }

    

    }
}
