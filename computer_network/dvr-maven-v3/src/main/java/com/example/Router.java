package com.example;


import java.util.HashMap;

import javax.swing.plaf.basic.BasicScrollPaneUI.HSBChangeListener;

import com.google.gson.Gson;

public class Router {
    int r_ID;
    int r_port;
    String r_filename;
    HashMap<String, Neighbor> r_neighbors;
    HashMap<String, Path> r_routes;
    
    public Router(int r_ID, int r_port, String r_filename, HashMap<String, Neighbor> r_neighbors, HashMap<String, Path> r_routes) {
        this.r_ID = r_ID;
        this.r_port = r_port;
        this.r_filename = r_filename;
        this.r_neighbors = r_neighbors;
        this.r_routes = r_routes;
    }

    public int getR_ID() {
        return r_ID;
    }

    public void setR_ID(int r_ID) {
        this.r_ID = r_ID;
    }

    public int getR_port() {
        return r_port;
    }

    public void setR_port(int r_port) {
        this.r_port = r_port;
    }

    public String getR_filename() {
        return r_filename;
    }

    public void setR_filename(String r_filename) {
        this.r_filename = r_filename;
    }

    public HashMap<String, Neighbor> getR_neighbors() {
        return r_neighbors;
    }

    public void setR_neighbors(HashMap<String, Neighbor> r_neighbors) {
        this.r_neighbors = r_neighbors;
    }

    public HashMap<String, Path> getR_routes() {
        return r_routes;
    }

    public void setR_routes(HashMap<String, Path> r_routes) {
        this.r_routes = r_routes;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + r_ID;
        result = prime * result + r_port;
        result = prime * result + ((r_filename == null) ? 0 : r_filename.hashCode());
        result = prime * result + ((r_neighbors == null) ? 0 : r_neighbors.hashCode());
        result = prime * result + ((r_routes == null) ? 0 : r_routes.hashCode());
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        Router other = (Router) obj;
        if (r_ID != other.r_ID)
            return false;
        if (r_port != other.r_port)
            return false;
        if (r_filename == null) {
            if (other.r_filename != null)
                return false;
        } else if (!r_filename.equals(other.r_filename))
            return false;
        if (r_neighbors == null) {
            if (other.r_neighbors != null)
                return false;
        } else if (!r_neighbors.equals(other.r_neighbors))
            return false;
        if (r_routes == null) {
            if (other.r_routes != null)
                return false;
        } else if (!r_routes.equals(other.r_routes))
            return false;
        return true;
    }

    public void initRoutes(){
        for(String name : r_neighbors.keySet()){
            r_routes.put(name, new Path(r_neighbors.get(name).getLinkCost(), name));
        }
    }

    public void printRoutes(){
        System.out.println(r_filename);
        System.out.println("destination  distance    nextHop");
        for(String node : r_routes.keySet()){
            System.out.printf("%-12s\t%-8d\t%-6s\n", node, r_routes.get(node).getDistance(), r_routes.get(node).getNextHop());
        }
        System.out.println();
    }
    
    public void receiveRoutesTable(HashMap<String, Path> routes, String nodeName){
        // HashMap<String, Path> hashtable = new HashMap<>(routes);
        HashMap<String, Path> hashtable = new HashMap<>();
        for(String key : routes.keySet()){
            hashtable.put(key, new Path(routes.get(key).getDistance(), routes.get(key).getNextHop()));
        }
        

        // no need to test the distance to itself
        hashtable.remove(this.r_filename);

        // we need to calculate the new state of the comming table, 
        // if all the entries are replaced by the new table
        for(String destNet : hashtable.keySet()){
            hashtable.get(destNet).setNextHop(nodeName);
            hashtable.get(destNet).setDistance(hashtable.get(destNet).getDistance() + this.r_routes.get(nodeName).getDistance());
        }

        // System.out.println(gson.toJson(r_routes));
        // compare the entry one by one
        // 1. if old one doesn't has, new one has, add to the origin table
        // 2. old one has, new one has, nextHop diffenent, select a one with lower cost
        // 3. old one has, new one has, nextHop same, update
        for(String destNet : hashtable.keySet()){
            
            // hashtable.get(keyString).setNextHop(nodeName);

            // oldTablePath refers to the original distance table of the router.
            // newTablePath refers to the new distance table of the comming table.
            Path oldTablePath = r_routes.get(destNet);
            Path newTablePath = hashtable.get(destNet);

            // 1
            if(oldTablePath == null){
                r_routes.put(destNet, new Path(hashtable.get(destNet).getDistance(), nodeName));
            }
            else {
                // 2
                if(oldTablePath.getNextHop().equals(nodeName)){
                    oldTablePath.setDistance(newTablePath.getDistance());
                }
                // 3
                else{
                    if(newTablePath.getDistance() < oldTablePath.getDistance()){
                        oldTablePath.setDistance(newTablePath.getDistance());
                        oldTablePath.setNextHop(nodeName);
                    }
                }
            }
        }
    }
}
