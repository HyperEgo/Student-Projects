/*
 * 
 * 
 * 
 */
package hotelmgmt;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Delilah Steen, Casey
 */
public class EmployeeTest {
    
    public EmployeeTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {
    }
    
    @Before
    public void setUp() {
        instance = new Employee("Jane Doe", 151515, 16.50);
    }
    
    @After
    public void tearDown() {
    }

    /**
     * Test of getEmpName method, of class Employee.
     */
    @Test
    public void testGetEmpName() {
        System.out.println("getEmpName");
        String expResult = "Jane Doe";
        String result = instance.getEmpName();
        assertEquals(expResult, result);
    }

    /**
     * Test of getEmpId method, of class Employee.
     */
    @Test
    public void testGetEmpId() {
        System.out.println("getEmpId");
        int expResult = 151515;
        int result = instance.getEmpId();
        assertEquals(expResult, result);
    }

    /**
     * Test of getEmpPay method, of class Employee.
     */
    @Test
    public void testGetEmpPay() {
        System.out.println("getEmpPay");
        double expResult = 16.50;
        double result = instance.getEmpPay();
        assertEquals(expResult, result, 16.50);
    }

    /**
     * Test of setEmpName method, of class Employee.
     */
    @Test
    public void testSetEmpName() {
        System.out.println("setEmpName");
        String g = "Jane Smith";
        instance.setEmpName(g);
        String result = instance.getEmpName();
        assertEquals(g, result);
    }

    /**
     * Test of setId method, of class Employee.
     */
    @Test
    public void testSetId() {
        System.out.println("setId");
        int d = 165;
        instance.setId(d);
        int result = instance.getEmpId();
        assertEquals(d, result);
    }

    /**
     * Test of setPay method, of class Employee.
     */
    @Test
    public void testSetPay() {
        System.out.println("setPay");
        double d = 17.50;
        instance.setPay(d);
        double result = instance.getEmpPay();
        assertEquals(d, result, 17.50);
    }
    
    Employee instance;
}
