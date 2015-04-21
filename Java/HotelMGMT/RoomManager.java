package roommanager;
import java.io.IOException;
import java.util.ArrayList;
//import java.util.Date;
import org.joda.time.Days;
import org.joda.time.LocalDate;
import java.util.Scanner;

/**
 *
 * @author Everett Weimer
 */
public class RoomManager {

    private ArrayList<RoomType> roomtypes;
    private LocalDate lastupdate;
   
    public RoomManager()
    {
        roomtypes = new ArrayList<RoomType>();
        lastupdate = new LocalDate();
    }
    
    public void AddRoomType(String type)
    {
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
            {
                return;
            }
        }
        RoomType newroom = new RoomType(type);
        roomtypes.add(newroom);
    }
    
    public void AddRoom(String type, int roomnum, double price)
    {
        for (int i = 0; i < roomtypes.size(); i++)
        {
            if (type.equals(roomtypes.get(i).roomname))
            {
                roomtypes.get(i).addRoom(roomnum, price);
            }
        }
        return;
    }
    
    public void CheckAvail(LocalDate indate, LocalDate outdate)
    {
        //See if checkout date is outside of 30 days
        if (Days.daysBetween(lastupdate, outdate).getDays() > 30)
        {
            System.out.println("more than 30");
            return;
        }//Check if checkin date is before today
        if (Days.daysBetween(lastupdate, indate).getDays() < 0)
        {
            System.out.println("Less than 0");
            return;
        }
        int first = Days.daysBetween(lastupdate, indate).getDays();
        int last = Days.daysBetween(lastupdate, outdate).getDays();
        for (int i = 0; i < roomtypes.size(); i++)
        {
            boolean ispossible = true;
            for (int j = first; j < last; j++)
            {
                if (roomtypes.get(i).reserved[j] >= roomtypes.get(i).roomcount)
                {
                    ispossible = false;
                }
            }    
            
            if (ispossible)
                System.out.println(roomtypes.get(i).roomname);
        }
    }
    
    public double MakeReserve(String type, LocalDate indate, LocalDate outdate)
    {
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
                for (int j = first; j < last; j++)
                {
                    if (roomtypes.get(i).reserved[j] >= roomtypes.get(i).roomcount)
                        ispossible = false;
                }    
            }
            if (ispossible)
            {
                for (int j = first; j < last; j++)
                    roomtypes.get(i).reserved[j]++;
                return roomtypes.get(i).roomlist.get(0).getPrice();
            }
        }
        return 0.0;
    }
    
    /**
     * @param args the command line arguments
     */
    public class RoomType {
        public String roomname;
        public int roomcount;
        public int[] reserved;
        public ArrayList<Room> roomlist;
        
        public RoomType(String name)
        {
            roomname = name;
            reserved = new int[30];
            roomcount = 0;
            roomlist = new ArrayList<Room>();
            
            for (int i = 0; i < 30; i++)
                reserved[i] = 0;
        }
        
        public void addRoom(int number, double price)
        {
            for (int i = 0; i < roomlist.size(); i++)
            {
                if (number == roomlist.get(i).getNumber())
                    return;
            }
            
            roomcount++;
            Room newroom = new Room(number, price);
            roomlist.add(newroom);
        }
    }
    
    public static void main(String[] args) throws IOException {
        // TODO code application logic here
        Scanner scanner = new Scanner(System.in);
        
        RoomManager rm = new RoomManager();
        String text;
        int input = 0;
        while (input != 5)
        {
            System.out.println("1. add type, 2. add room, 3. rooms avail, 4. reserve");
            input = scanner.nextInt();
            
            if (input == 1)
            {
                System.out.println("Enter name of room type");
                text = scanner.next();
                rm.AddRoomType(text);
            }
            if (input == 2)
            {
                System.out.println("Enter room type");
                text = scanner.next();
                double money;
                int roomn;
                System.out.println("Enter room number");
                roomn = scanner.nextInt();
                System.out.println("Enter room cost");
                money = scanner.nextDouble();
                rm.AddRoom(text, roomn, money);
            }
            if (input == 3)
            {
                LocalDate before = new LocalDate();
                LocalDate after = new LocalDate();
                System.out.println("Enter number of day from now to checkin");
                int days = scanner.nextInt();
                before = before.plusDays(days);
                System.out.println("Enter number of day from now to checkout");
                days = scanner.nextInt();
                after = after.plusDays(days);
                rm.CheckAvail(before, after);
            }
            if (input == 4)
            {
                LocalDate before = new LocalDate();
                LocalDate after = new LocalDate();
                System.out.println("Enter number of day from now to checkin");
                int days = scanner.nextInt();
                before.plusDays(days);
                System.out.println("Enter number of day from now to checkout");
                days = scanner.nextInt();
                after.plusDays(days);
                System.out.println("Enter room type");
                text = scanner.next();
                double rmprice = rm.MakeReserve(text, before, after);
                System.out.println(rmprice);
                //rm.CheckAvail(before, after);
            }
        }
    }
    
}
