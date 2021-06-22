/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
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
 * @author Casey
 */
public class UserViewTest {
    
    public UserViewTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {
    }
    
    @Before
    public void setUp() {
    }
    
    @After
    public void tearDown() {
    }

    /**
     * Test of setFlags method, of class UserView.
     */
    @Test
    public void testSetFlags() {
        System.out.println("setFlags");
        boolean man = true;
        boolean cus = false;
        boolean emp = false;
        HotelSystem testHotel = HotelSystem.getInstance();        
        UserView instance = new UserView(testHotel);
        boolean result = instance.setFlags(man, cus, emp);
        assertTrue ("UserView failed to set flags.",result);
    }
    
}
