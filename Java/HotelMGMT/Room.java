package roommanager;

public class Room {
    private int roomnumber;
    private double price;
    private boolean isavailable;
    //Customer occupant; Once declared

    public Room(int number, double rprice)
    {
        roomnumber = number;
        price = rprice;
        
        isavailable = true;
        //occupant = 0; Once customer declared
    }
    
    public int getNumber()
    {
        return roomnumber;
    }
    
    public void setPrice(double newprice)
    {
        price = newprice;
    }
    
    public double getPrice()
    {
        return price;
    }
    
    /*
    public void setOccupant(Customer occ)
    {
        occupant = occ;
    }
    
    public Customer getOccupant()
    {
        return occupant;
    }
    */
    
    public void setAvail(boolean avail)
    {
        isavailable = avail;
    }
    
    public boolean getAvail()
    {
        return isavailable;
    }
}
