///**
// *
// * @author Delilah Steen
// */
//public class PurchaseTest {
//    
//    public PurchaseTest() {
//    }
//    
//    @BeforeClass
//    public static void setUpClass() {
//    }
//    
//    @AfterClass
//    public static void tearDownClass() {
//    }
//    
//    @Before
//    public void setUp() {
//        instance = new Purchase();
//        instance.setRoomType("King room");
//        instance.setBalance(150.50);
//    }
//    
//    @After
//    public void tearDown() {
//    }
//
//    /**
//     * Test of getBalance method, of class Purchase.
//     */
//    @Test
//    public void testGetBalance() {
//        System.out.println("getBalance");
//        double expResult = 150.50;
//        double result = instance.getBalance();
//        assertEquals(expResult, result, 150.50);
//    }
//
//    /**
//     * Test of getRoomType method, of class Purchase.
//     */
//    @Test
//    public void testGetRoomType() {
//        System.out.println("getRoomType");
//        String expResult = "King room";
//        String result = instance.getRoomType();
//        assertEquals(expResult, result);
//    }
//
//    /**
//     * Test of setBalance method, of class Purchase.
//     */
//    @Test
//    public void testSetBalance() {
//        System.out.println("setBalance");
//        double b = 50.20;
//        instance.setBalance(b);
//        double result = instance.getBalance();
//        assertEquals(b, result, 50.20);
//    }
//
//    /**
//     * Test of setRoomType method, of class Purchase.
//     */
//    @Test
//    public void testSetRoomType() {
//        System.out.println("setRoomType");
//        String room = "Queen room";
//        instance.setRoomType(room);
//        String result = instance.getRoomType();
//        assertEquals(room, result);
//    }
//    
//    Purchase instance;
//}
//



/*
 * 
 * 
 * 
 */
package hotelmgmt;

import org.joda.time.LocalDate;
import org.joda.time.format.DateTimeFormat;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Delilah Steen
 */
public class PurchaseTest {
    
    public PurchaseTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {
    }
    
    @Before
    public void setUp() {
        instance = new Purchase();
        instance.setRoomType("King room");
        instance.setBalance(150.50);
    }
    
    @After
    public void tearDown() {
    }

    /**
     * Test of getBalance method, of class Purchase.
     */
    @Test
    public void testGetBalance() {
        System.out.println("getBalance");
        double expResult = 150.50;
        double result = instance.getBalance();
        assertEquals(expResult, result, 150.50);
    }

    /**
     * Test of getRoomType method, of class Purchase.
     */
    @Test
    public void testGetRoomType() {
        System.out.println("getRoomType");
        String expResult = "King room";
        String result = instance.getRoomType();
        assertEquals(expResult, result);
    }

    /**
     * Test of getReserve method, of class Purchase.
     */
    @Test
    public void testGetReserve() {
        System.out.println("getReserve");
        boolean expResult = false;
        boolean result = instance.getReserve();
        assertEquals(expResult, result);
    }

    /**
     * Test of getRoom method, of class Purchase.
     */
    @Test
    public void testGetRoom() {
        System.out.println("getRoom");
        instance.setRoom(2);
        int expResult = 2;
        int result = instance.getRoom();
        assertEquals(expResult, result);
    }

    /**
     * Test of getDateIn method, of class Purchase.
     */
    @Test
    public void testGetDateIn() {
        System.out.println("getDateIn");
        String start = "03/30/2015";
        LocalDate startdate = LocalDate.parse(start, DateTimeFormat.forPattern("MM/dd/yyyy"));
        instance.setDateIn(startdate);
        LocalDate result = instance.getDateIn();
        assertEquals(startdate, result);
    }

    /**
     * Test of getDateOut method, of class Purchase.
     */
    @Test
    public void testGetDateOut() {
        System.out.println("getDateOut");
        String end = "04/01/2015";
        LocalDate enddate = LocalDate.parse(end, DateTimeFormat.forPattern("MM/dd/yyyy")); 
        instance.setDateOut(enddate);
        LocalDate result = instance.getDateOut();
        assertEquals(enddate, result);
    }

    /**
     * Test of setBalance method, of class Purchase.
     */
    @Test
    public void testSetBalance() {
        System.out.println("setBalance");
        double b = 50.20;
        instance.setBalance(b);
        double result = instance.getBalance();
        assertEquals(b, result, 50.20);
    }

    /**
     * Test of setRoomType method, of class Purchase.
     */
    @Test
    public void testSetRoomType() {
        System.out.println("setRoomType");
        String room = "Queen room";
        instance.setRoomType(room);
        String result = instance.getRoomType();
        assertEquals(room, result);
    }

    /**
     * Test of setReserve method, of class Purchase.
     */
    @Test
    public void testSetReserve() {
        System.out.println("setReserve");
        boolean flag = true;
        instance.setReserve(flag);
        boolean result = instance.getReserve();
        assertEquals(flag, result);

    }

    /**
     * Test of setRoom method, of class Purchase.
     */
    @Test
    public void testSetRoom() {
        System.out.println("setRoom");
        instance.setRoom(3);
        int expResult = 3;
        int result = instance.getRoom();
        assertEquals(expResult, result);
    }

    /**
     * Test of setDateIn method, of class Purchase.
     */
    @Test
    public void testSetDateIn() {
        System.out.println("setDateIn");
        String start = "04/30/2015";
        LocalDate startdate = LocalDate.parse(start, DateTimeFormat.forPattern("MM/dd/yyyy"));
        instance.setDateIn(startdate);
        LocalDate result = instance.getDateIn();
        assertEquals(startdate, result);
    }

    /**
     * Test of setDateOut method, of class Purchase.
     */
    @Test
    public void testSetDateOut() {
        System.out.println("setDateOut");
        String end = "05/01/2015";
        LocalDate enddate = LocalDate.parse(end, DateTimeFormat.forPattern("MM/dd/yyyy")); 
        instance.setDateOut(enddate);
        LocalDate result = instance.getDateOut();
        assertEquals(enddate, result);
    }
     Purchase instance;
    
}
