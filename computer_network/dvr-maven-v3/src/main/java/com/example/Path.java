package com.example;


public class Path{
    private int distance;
    private String nextHop;
    
    public Path(int distance, String nextHop) {
        this.distance = distance;
        this.nextHop = nextHop;
    }

    public void setDistance(int distance) {
        this.distance = distance;
    }

    public void setNextHop(String nextHop) {
        this.nextHop = nextHop;
    }

    public int getDistance() {
        return distance;
    }

    public String getNextHop() {
        return nextHop;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        Path other = (Path) obj;
        if (distance != other.distance)
            return false;
        if (nextHop != other.nextHop)
            return false;
        return true;
    }

    
}