/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hotelmgmt;

import java.util.ArrayList;
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
 * @author Casey, Delilah
 */
public class HotelSystemTest {
    
    private HotelSystem hs;
    
    public HotelSystemTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {

    }
    
    @Before
    public void setUp() {
        hs = HotelSystem.getInstance();
    }
    
    @After
    public void tearDown() {
    }

    /**
     * Test of addEmployee method, of class HotelSystem.
     */
    @Test
    public void testAddEmployeeAndEditEmployeeAndFindEmployee() {
        System.out.println("addEmployee");
        String name = "Jessica Smith";
        double pay = 0.0;
        hs.addEmployee(name, pay);
        assertTrue("Did not find new employe with findEmployee()",hs.findEmployee(name) > 0);
        assertTrue("Did not edit employee", hs.editEmployee(hs.findEmployee(name), "Jessica Anderson", 10.20));
        assertTrue("Did not find emplyee in employee list",hs.findEmployee("Jessica Anderson") > 0);
        assertFalse("Found an employee in employee list that doesn't exist", hs.findEmployee("Gone Byebye") > 0);
        hs.deleteEmployeeManual(hs.findEmployee("Jessica Anderson"));
        System.out.println("end addEmployee and edit employee\n");
    }


    /**
     * Test of deleteEmployeeAuto method, of class HotelSystem.
     */
    @Test
    public void testDeleteEmployeeAuto() {
        System.out.println("deleteEmployeeAuto");
        String name = "Kyle Reece";
        assertFalse("DeleteEmployeeAuto deleted an employee who didn't exist", hs.deleteEmployeeAuto(name));
        hs.addEmployee(name, 1.0);
        assertTrue("DeleteEmployeeAuto failed to delete an employee",hs.deleteEmployeeAuto(name));
        System.out.println("end deleteEmployeeAuto\n");
    }

    /**
     * Test of deleteEmployeeManual method, of class HotelSystem.
     */
    @Test
    public void testDeleteEmployeeManual() {
        System.out.println("deleteEmployeeManual");
        int id = 0;
        assertFalse("DeleteEmployeeManual deleted an employee who didn't exist",hs.deleteEmployeeManual(1747));
        String name = "Kyle Reece";
        hs.addEmployee(name, 1.0);
        assertTrue("DeleteEmployeeManual failed to delete an employe who did exist",hs.deleteEmployeeManual(hs.findEmployee(name)));
        System.out.println("end deleteEmployeeManual\n");
    }

    /**
     * Test of addCustomer method, of class HotelSystem.
     */
    @Test
    public void testAddCustomerAndEditCustomer() {
        System.out.println("addCustomer");
        String name = "Stan Lee";
        assertFalse("Found customer who did not exist",hs.findCustomer(name)>0);
        hs.addCustomer(name, "Visa");
        assertTrue("Did not find a customer who exists in customer list",hs.findCustomer(name)>0);
        assertTrue("Did not edit existing customer", hs.editCustomer(hs.findCustomer(name), "Stan Lee II", "Master Card"));
        hs.deleteCustomerManual(hs.findCustomer("Stan Lee II"));
        System.out.println("end addCustomer\n");
    }

    /**
     * Test of findCustomer method, of class HotelSystem.
     */
    @Test
    public void testFindCustomer() {
        System.out.println("findCustomer");
        String name = "Bob Sagot";
        assertFalse("Found Customer who cannot exist",hs.findCustomer("")>0);
        hs.addCustomer(name, "visa");
        assertTrue("Did not find customer who exists",hs.findCustomer(name)>0);
        hs.deleteCustomerAuto(name);
        System.out.println("end findCustomer\n");
    }

    /**
     * Test of findCustomerName method, of class HotelSystem.
     */
    @Test
    public void testFindCustomerNameAndCheckId() {
        System.out.println("findCustomerName");
        String name = "Eric Cartman";
        hs.addCustomer(name, "visa");
        int id = hs.findCustomer(name);
        assertFalse("Found a record that did not exist",hs.findCustomerName(1747) == "Billy Bob");
        assertTrue("Did not find a record that exists",hs.findCustomerName(id) == "Eric Cartman");
        assertTrue("CheckCustomerID return false for a record that does exist",hs.checkCustomerId(id));
        hs.deleteCustomerManual(id);
        System.out.println("end findCustomerNameAndCheckId\n");
    }

    /**
     * Test of deleteCustomerAuto method, of class HotelSystem.
     */
    @Test
    public void testDeleteCustomerAuto() {
        System.out.println("deleteCustomerAuto");
        String name = "Eric Cartman2";
        hs.addCustomer(name, "Visa");
        assertFalse("Deletion of non-existant record returned true",hs.deleteCustomerAuto("Billy Bob"));
        assertTrue("Deletion of existant record return false",hs.deleteCustomerAuto(name));
        System.out.println("end deleteCustomerAuto\n");
    }

    /**
     * Test of deleteCustomerManual method, of class HotelSystem.
     */
    @Test
    public void testDeleteCustomerManual() {
        System.out.println("deleteCustomerManual");
        int id = 0;
        String name = "Ralph Nader";
        assertFalse("DeleteCustomerManual succeded when record should not exist",hs.deleteCustomerManual(7777));
        hs.addCustomer(name, "visa");
        assertTrue("DeleteCusomerManual failed when record exists",hs.deleteCustomerManual(hs.findCustomer(name)));
        System.out.println(" end deleteCustomerManual\n");
    }

    /**
     * Test of bookRoom method, of class HotelSystem.
     */
    @Test
    public void testBookRoomAndRemoveBookRoom() {
        System.out.println("bookRoom");
        hs.addRoom("Double", 4, 50.00);
        String name = "Jordan Brown";
        hs.addCustomer(name, "visa");
        int id = hs.findCustomer(name);
        String start = "4/16/2015";
        String end = "04/17/2015";
        LocalDate startdate = LocalDate.parse(start, DateTimeFormat.forPattern("MM/dd/yyyy"));
        LocalDate enddate = LocalDate.parse(end, DateTimeFormat.forPattern("MM/dd/yyyy"));
        assertFalse("Allowed non-existant customer to book room",hs.bookRoom(true, 777, "Double", startdate, enddate));
        assertTrue("Failed to book room for existing customer",hs.bookRoom(true, id,"Double", startdate, enddate));
        assertTrue("Failed to remove room reservation for existing customer", hs.removeBookRoom(id, 1));
        hs.deleteCustomerManual(id);
        System.out.println("end bookRoom and remove room\n");
    }


    /**
     * Test of getInstance method, of class HotelSystem.
     */
    @Test
    public void testGetInstance() {
        System.out.println("getInstance");
        HotelSystem expResult = hs;
        HotelSystem result = HotelSystem.getInstance();
        assertEquals(expResult, result);
    }

    /**
     * Test of getEmployeeList method, of class HotelSystem.
     */
    @Test
    public void testGetEmployeeList() {
        System.out.println("getEmployeeList");
        ArrayList<Employee> result = hs.getEmployeeList();
        assertNotNull(result);
        System.out.println("end getEmployeeList\n");
    }

    /**
     * Test of displayEmpList method, of class HotelSystem.
     */
    @Test
    public void testDisplayEmpList() {
        System.out.println("displayEmpList");
        hs.addEmployee("Richard Wilson", 15.20);
        hs.displayEmpList();
        hs.deleteEmployeeManual(hs.findEmployee("Richard Wilson"));
        System.out.println("end displayEmpList\n");
    }

    /**
     * Test of checkEmployeeId method, of class HotelSystem.
     */
    @Test
    public void testCheckEmployeeId() {
        System.out.println("checkEmployeeId");
        hs.addEmployee("Billy Bob", 10.20);
        int id = hs.findEmployee("Billy Bob");
        boolean result = hs.checkEmployeeId(id);
        assertTrue("Check employee id returned false for current employee", result);
        hs.deleteEmployeeManual(id);
        System.out.println("end checkEmployeeId\n");
    }

    /**
     * Test of getCustomerList method, of class HotelSystem.
     */
    @Test
    public void testGetCustomerList() {
        System.out.println("getCustomerList");
        ArrayList<Customer> result = hs.getCustomerList();
        assertNotNull(result);
        System.out.println("end getCustomerList\n");
    }

    /**
     * Test of displayCusList method, of class HotelSystem.
     */
    @Test
    public void testDisplayCusList() {
        System.out.println("displayCusList");
        hs.addCustomer("Sally Smith", "cash");
        hs.displayCusList();
        hs.deleteCustomerManual(hs.findCustomer("Sally Smith"));
        System.out.println("end displayCusList\n");
    }

    /**
     * Test of displayRoomsAvailable method, of class HotelSystem.
     */
    @Test
    public void testDisplayRoomsAvailable() {
        System.out.println("displayRoomsAvailable");
        String start = "04/16/2015";
        String end = "04/17/2015";
        LocalDate startdate = LocalDate.parse(start, DateTimeFormat.forPattern("MM/dd/yyyy"));
        LocalDate enddate = LocalDate.parse(end, DateTimeFormat.forPattern("MM/dd/yyyy"));
        hs.displayRoomsAvailable(startdate, enddate);
        System.out.println("end displayRoomsAvailable\n");
    }

 
    /**
     * Test of displayReservations method, of class HotelSystem.
     */
    @Test
    public void testDisplayReservations() {
        System.out.println("displayReservations");
        hs.displayReservations();
        System.out.println("end displayReservations");
    }

    /**
     * Test of addRoom method, of class HotelSystem.
     */
    @Test
    public void testAddRoomAndDeleteRoom() {
        System.out.println("addRoom");
        System.out.println("addRoom Single, number: 2, price: 50");
        hs.addRoom("Single", 2, 50.00);
        hs.displayAllRooms();
        System.out.println("Remove Room Single, number: 2, price: 50");
        hs.deleteRoom("Single", 2);
        hs.displayAllRooms();
        System.out.println("end addRoom and delete room\n");
    }

    /**
     * Test of deleteRoom method, of class HotelSystem.
     */
    @Test
    public void testDeleteRoom() {
        System.out.println("deleteRoom");
        System.out.println("delete Suite room: 3");
        hs.deleteRoom("Suite", 3);
        hs.displayAllRooms();
        System.out.println("end deleteRoom\n");
    }

    /**
     * Test of displayTotalBalance method, of class HotelSystem.
     */
    @Test
    public void testDisplayTotalBalance() {
        System.out.println("displayTotalBalance");
        hs.displayTotalBalance();
        System.out.println("End displayTotalBalance\n\n");
    }

    /**
     * Test of displayCustomerBalance method, of class HotelSystem.
     */
    @Test
    public void testDisplayCustomerBalance() {
        System.out.println("displayCustomerBalance");
        hs.addCustomer("Lewis Richard", "Master Card");
        int id = hs.findCustomer("Lewis Richard");
        hs.displayCustomerBalance(id);
        hs.deleteCustomerManual(hs.findCustomer("Lewis Richard"));
        System.out.println(" end displayCustomerBalance\n");
    }
}
