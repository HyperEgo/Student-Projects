package hotelmgmt;

import java.io.Serializable;

/**
 * Container for the room objects
 * @author Casey
 */
public class Room implements Serializable{
    private int roomnumber;
    private boolean isavailable;
    Customer occupant;

    /**
     * Constructor for the room object
     * @param number the room number
     */
    public Room(int number)
    {
        roomnumber = number;
        isavailable = true;
    }
    
    /**
     * Method for obtaining the room number
     * @return the room number
     */
    public int getNumber()
    {
        return roomnumber;
    }
    
    /**
     * Method for checking into a room by a customer
     * @param occ the occupant
     * @return the room number
     */
    public int checkIn(Customer occ)
    {
        isavailable = false;
        occupant = occ;
        return roomnumber;
    }
    
    /**
     * Method for returning the current occupant of a room
     * @return the occupant
     */
    public Customer getOccupant()
    {
        return occupant;
    }
    
    /**
     * Method for setting the room availability flag of a room object.
     * @param avail the availability of the room
     */
    public void setAvail(boolean avail)
    {
        isavailable = avail;
    }
    
    /**
     * Method to return the availability of a room object
     * @return the availability of the room
     */
    public boolean getAvail()
    {
        return isavailable;
    }
    
    /**
     * Method to mark a room as available again.
     */
    public void checkOut()
    {
        isavailable = true;
    }
}
