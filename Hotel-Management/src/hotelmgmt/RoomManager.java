

package hotelmgmt;

import java.io.IOException;
import java.util.ArrayList;
import org.joda.time.Days;
import org.joda.time.LocalDate;
import java.io.Serializable;
import java.util.Scanner;

/**
 * Management class for rooms
 * @author Everett Weimer
 */
public class RoomManager implements Serializable{

    private ArrayList<RoomType> roomtypes;
    private LocalDate lastupdate;
    private int dateoffset;
   
    /**
     * Constructor for the class
     */
    public RoomManager()
    {
        roomtypes = new ArrayList<RoomType>();
        lastupdate = new LocalDate();
        dateoffset = 0;
    }
    
    /**
     * Add room type to the system
     * @param type the type of room
     * @param price the price of room
     */
    public void addRoomType(String type, double price)
    {
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
            {
                return;
            }
        }
        RoomType newroom = new RoomType(type, price);
        roomtypes.add(newroom);
    }
    
     /**
     * Add room of the specified type to the system
     * @param type the type of room
     * @param roomnum the room number
     * @param price the price
     */
    public void addRoom(String type, int roomnum, double price)
    {
        addRoomType(type, price);
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
            {
                roomtypes.get(i).addRoom(roomnum);
            }
        }
        return;
    }
    
    /**
     * Marks room as unavailable to reserve
     * @param type the type of room
     * @param roomnum the room number
     */
    public void deleteRoom(String type, int roomnum)
    {
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
            {
                for (int j = 0; j < roomtypes.get(i).roomlist.size(); j++)
                    if (roomtypes.get(i).roomlist.get(j).getNumber() == roomnum)
                    {
                        roomtypes.get(i).roomlist.get(j).setAvail(false);
                        roomtypes.get(i).roomcount--;
                        return;
                    }
            }
        }
    }
    
    /**
     * Obtain the price of the type room
     * @param type the type of room
     * @return the price of the type of room specified
     */
    public double getRoomPrice(String type)
    {
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
                return roomtypes.get(i).price;
        }
        return 0.0;
    }
    
    /**
     * Display list of all rooms
     */
    public void displayAllRooms(){
        UserView.speak("\nShow all Hotel Rooms.");
        for(int i=0; i<roomtypes.size(); i++)
        {
            for (int j = 0; j < roomtypes.get(i).roomlist.size(); j++)
                //if (roomtypes.get(i).roomlist.get(j).getAvail())
                    UserView.listspeak(roomtypes.get(i).roomlist.get(j).getNumber() +
                            "   " + roomtypes.get(i).roomname +
                            "   " + roomtypes.get(i).price);
        }        
    }
    
    
    /**
     * Display available rooms for dates specified
     * @param indate the check in date
     * @param outdate the check out date
     */
    public void roomsAvailable(LocalDate indate, LocalDate outdate)
    {
        //First check to see if date has changed since lastupdate
        LocalDate today = new LocalDate();
        if (Days.daysBetween(lastupdate, today).getDays() > 0)
        {
            dateoffset += Days.daysBetween(lastupdate, today).getDays();
            lastupdate = today;
        }
        //See if checkout date is outside of 30 days
        if (Days.daysBetween(lastupdate, outdate).getDays() > 30)
        {
            UserView.speak("more than 30 days out");
            return;
        }//Check if checkin date is before today
        if (Days.daysBetween(lastupdate, indate).getDays() < 0)
        {
            UserView.speak("Less than 0 days out");
            return;
        }
        int first = Days.daysBetween(lastupdate, indate).getDays();
        int last = Days.daysBetween(lastupdate, outdate).getDays();
        for (int i = 0; i < roomtypes.size(); i++)
        {
            boolean ispossible = true;
            for (int j = first+dateoffset; j < last+dateoffset; j++)
            {
                
                if (roomtypes.get(i).reserved[j%30] >= roomtypes.get(i).roomcount)
                {
                    ispossible = false;
                }
            }    
            
            if (ispossible)
                UserView.listspeak("Room Type: " + roomtypes.get(i).roomname + 
                        "  Price: " + roomtypes.get(i).price);
        }
    }
    
    /**
     * Determine if a room is available for the date range
     * @param type the type of room
     * @param indate the check in date
     * @param outdate the check out date
     * @return true if available and false if not available
     */
    public boolean isRoomAvailable(String type, LocalDate indate, LocalDate outdate)
    {
        //First check to see if date has changed since lastupdate
        LocalDate today = new LocalDate();
        if (Days.daysBetween(lastupdate, today).getDays() > 0)
        {
            dateoffset += Days.daysBetween(lastupdate, today).getDays();
            lastupdate = today;
        }
        //See if checkout date is outside of 30 days
        if (Days.daysBetween(lastupdate, outdate).getDays() > 30)
        {
            UserView.speak("more than 30 days out");
            return false;
        }//Check if checkin date is before today
        if (Days.daysBetween(lastupdate, indate).getDays() < 0)
        {
            UserView.speak("Less than 0 days out");
            return false;
        }
        int first = Days.daysBetween(lastupdate, indate).getDays();
        int last = Days.daysBetween(lastupdate, outdate).getDays();
        boolean ispossible = true;
        for (int i = 0; i < roomtypes.size(); i++)
        {
            //boolean ispossible = true;
            for (int j = first+dateoffset; j < last+dateoffset; j++)
            {
                if (roomtypes.get(i).reserved[j%30] >= roomtypes.get(i).roomcount)
                {
                    ispossible = false;
                }
            }    
            
            if (ispossible)
                return true;
        }
        return false;
    }
    
    /**
     * Make a reservation in the system for the date specified
     * @param type the type of room
     * @param indate the check in date
     * @param outdate the check out date
     * @return the price for one night
     */
    public double makeReserve(String type, LocalDate indate, LocalDate outdate)
    {
        //First check to see if date has changed since lastupdate
        LocalDate today = new LocalDate();
        if (Days.daysBetween(lastupdate, today).getDays() > 0)
        {
            dateoffset += Days.daysBetween(lastupdate, today).getDays();
            lastupdate = today;
        }
        //See if checkout date is outside of 30 days
        if (Days.daysBetween(lastupdate, outdate).getDays() > 30)
            return 0.0;
        //Check if checkin date is before today
        if (Days.daysBetween(lastupdate, indate).getDays() < 0)
            return 0.0;
        
        int first = Days.daysBetween(lastupdate, indate).getDays();
        int last = Days.daysBetween(lastupdate, outdate).getDays();
        
        for (int i = 0; i < roomtypes.size(); i++)
        {
            boolean ispossible = false;
            if (type.equals(roomtypes.get(i).roomname))
            {
                ispossible = true;
                for (int j = first+dateoffset; j < last+dateoffset; j++)
                {
                    if (roomtypes.get(i).reserved[j%30] >= roomtypes.get(i).roomcount)
                        ispossible = false;
                }    
            }
            if (ispossible)
            {
                for (int j = first+dateoffset; j < last+dateoffset; j++)
                {
                    roomtypes.get(i).reserved[j%30]++;
                }
                return roomtypes.get(i).price;
            }
        }
        return 0.0;
    }
    
    /**
     * Clear a reservation from the system and return the room back to use
     * @param type the type of room
     * @param indate the check in date
     * @param outdate the check out date
     */
    public void removeReservation(String type, LocalDate indate, LocalDate outdate)
    {
        //First check to see if date has changed since lastupdate
        LocalDate today = new LocalDate();
        if (Days.daysBetween(lastupdate, today).getDays() > 0)
        {
            dateoffset += Days.daysBetween(lastupdate, today).getDays();
            lastupdate = today;
        }
        //See if checkout date is outside of 30 days
        if (Days.daysBetween(lastupdate, outdate).getDays() > 30)
            return;
        //Check if checkin date is before today
        if (Days.daysBetween(lastupdate, indate).getDays() < 0)
            return;
        
        int first = Days.daysBetween(lastupdate, indate).getDays();
        int last = Days.daysBetween(lastupdate, outdate).getDays();
        
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
            {
                for (int j = first+dateoffset; j < last+dateoffset; j++)
                {
                    roomtypes.get(i).reserved[j%30]--;
                }
            }
        }
    }
    
    /**
     * Check in a customer to a room
     * @param type the type of room
     * @param occ the occupant
     * @return the room number
     */
    public int checkIn(String type, Customer occ)
    {
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
            {
                int j = 0;
                while (!roomtypes.get(i).roomlist.get(j).getAvail()) 
                    j++;
                if (j < roomtypes.get(i).roomlist.size())
                    return roomtypes.get(i).roomlist.get(j).checkIn(occ);
            }
        }
        return 0;
    }
    
    /**
     * Check out a customer from a room
     * @param type the type of room
     * @param roomnum the room number
     */
    public void checkOut(String type, int roomnum)
    {
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
            {
                int j = 0;
                while (roomtypes.get(i).roomlist.get(j).getNumber() != roomnum)
                    j++;
                if (j < roomtypes.get(i).roomlist.size())
                    roomtypes.get(i).roomlist.get(j).checkOut();
                return;
            }
        }
    }
    
     /**
     * Allow the price of a room type to be changed
     * @param type the type of room
     * @param newprice the new price of the type specified
     */
    public void changePrice(String type, double newprice)
    {
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
                roomtypes.get(i).setPrice(newprice);
        }
    }
    
    /**
     * Room type class
     * 
     */
    public class RoomType implements Serializable{
        public String roomname;
        public int roomcount;
        public int[] reserved;
        public double price;
        public ArrayList<Room> roomlist;
        
        /**
         * Constructor for RoomType
         * @param name
         * @param rmprice 
         */
        public RoomType(String name, double rmprice)
        {
            roomname = name;
            reserved = new int[30];
            roomcount = 0;
            price = rmprice;
            roomlist = new ArrayList<Room>();
            
            for (int i = 0; i < 30; i++)
                reserved[i] = 0;
        }
        
        /**
         * Add room to the array of rooms
         * @param number the room number
         */
        public void addRoom(int number)
        {
            for (int i = 0; i < roomlist.size(); i++)
            {
                if (number == roomlist.get(i).getNumber())
                {
                    if (!roomlist.get(i).getAvail())
                    {
                        //UserView.speak("Trying to do it");
                        roomlist.get(i).setAvail(true);
                        roomcount++;
                    }
                    return;
                }
            }
            
            roomcount++;
            Room newroom = new Room(number);
            roomlist.add(newroom);
        }
        
        /**
         * Set the price of the room.
         * @param newprice the new price
         */
        public void setPrice(double newprice)
        {
            if (newprice > 0)
                price = newprice;
            else
                UserView.speak("Invalid Price");
        }
    }
}
