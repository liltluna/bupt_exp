import java.util.HashMap;

public class Neighbor {
    int linkCost;
    int port; 
    int timeout;
    HashMap<String, Integer> paths;
    
    public Neighbor(int linkCost, int port, int timeout, HashMap<String, Integer> paths) {
        this.linkCost = linkCost;
        this.port = port;
        this.timeout = timeout;
        this.paths = paths;
    }

    public int getLinkCost() {
        return linkCost;
    }

    public void setLinkCost(int linkCost) {
        this.linkCost = linkCost;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public int getTimeout() {
        return timeout;
    }

    public void setTimeout(int timeout) {
        this.timeout = timeout;
    }

    public HashMap<String, Integer> getPaths() {
        return paths;
    }

    public void setPaths(HashMap<String, Integer> paths) {
        this.paths = paths;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + linkCost;
        result = prime * result + port;
        result = prime * result + timeout;
        result = prime * result + ((paths == null) ? 0 : paths.hashCode());
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
        Neighbor other = (Neighbor) obj;
        if (linkCost != other.linkCost)
            return false;
        if (port != other.port)
            return false;
        if (timeout != other.timeout)
            return false;
        if (paths == null) {
            if (other.paths != null)
                return false;
        } else if (!paths.equals(other.paths))
            return false;
        return true;
    }

    
    
}
