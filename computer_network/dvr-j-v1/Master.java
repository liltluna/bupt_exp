import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.HashMap;

public class Master {
    HashMap<String, Integer> nodesMap;
    HashMap<String, Router> routerMap;
    
    public static void main(String[] args) {
        Master master = new Master();
        master.readData();
        master.iterateExchange();
    }

    public void readData(){
        BufferedReader bufferedReader;
        String temString;
        int counter = 0;
        String nodeFilePath = "data/nodeaddr.txt";
        String topologyFilePath = "data/topology.txt";
        File nodeFile = new File(nodeFilePath);
        File topologyConfigFile = new File(topologyFilePath);

        nodesMap = new HashMap<>();
        routerMap = new HashMap<>();
        
        // test if the file exists
        if(!nodeFile.exists() || !topologyConfigFile.exists()){
           System.out.println("File not exists."); 
        }

        // read data from file and config routers
        try{
            bufferedReader = new BufferedReader(new FileReader(nodeFile));
            while((temString = bufferedReader.readLine()) != null){
                String[] paras = temString.split("[\t]");
                nodesMap.put(paras[0], Integer.parseInt(paras[1]));
                routerMap.put(paras[0], new Router(++counter, Integer.parseInt(paras[1]), paras[0], new HashMap<>(), new HashMap<>()));
            }
            bufferedReader.close();
            bufferedReader = new BufferedReader(new FileReader(topologyConfigFile));
            while((temString = bufferedReader.readLine()) != null){
                String[] paras = temString.split("\t");
                routerMap.get(paras[0]).getR_neighbors().put(paras[1], new Neighbor(Integer.parseInt(paras[2]), nodesMap.get(paras[1]), 10, null));
                routerMap.get(paras[1]).getR_neighbors().put(paras[0], new Neighbor(Integer.parseInt(paras[2]), nodesMap.get(paras[0]), 10, null));
            }
            bufferedReader.close();
        // not good, no detail
        }catch (Exception exception){
            System.out.println(exception.getStackTrace());
        }

        for(Router router : routerMap.values()){
            router.initRoutes();
        }

    }

    // DVR core code
    public void iterateExchange(){

        for(int i = 0; i< 3; i++){
            for(Router router : routerMap.values()){
                for(String nameOfNeighbor : router.getR_neighbors().keySet()){
                    Router neighborRouter = routerMap.get(nameOfNeighbor);
                    router.receiveRoutesTable(neighborRouter.getR_routes(), nameOfNeighbor);
                }
            }
            this.routerMap.get("nodeA").printRoutes();
        }
    }


}
