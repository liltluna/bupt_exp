import java.util.HashMap;

public class Router {
    int r_ID;
    int r_port;
    String r_filename;
    HashMap<String, Integer> r_neighbors;
    HashMap<String, Integer> r_routes;
    
    public Router(int r_ID, int r_port, String r_filename, HashMap<String, Integer> r_neighbors,
            HashMap<String, Integer> r_routes) {
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

    public HashMap<String, Integer> getR_neighbors() {
        return r_neighbors;
    }

    public void setR_neighbors(HashMap<String, Integer> r_neighbors) {
        this.r_neighbors = r_neighbors;
    }

    public HashMap<String, Integer> getR_routes() {
        return r_routes;
    }

    public void setR_routes(HashMap<String, Integer> r_routes) {
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
    
    
    
}
