/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hotelmgmt;

import java.util.Scanner;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.math.RoundingMode;
import javax.swing.*;
import javax.swing.event.*;
//import java.awt.TextField;
import org.joda.time.*;
import org.joda.time.format.DateTimeFormat;
import java.text.NumberFormat;
import java.text.ParseException;
import java.util.Locale;


/**
 * This class is the view class used to display the appropriate interface type
 * @author Casey, edit by entire team
 */
public class UserView {
    private JList<String> list;
    private JList<String> list2;
    private static DefaultListModel listModel;
    private boolean run;
 
    public UserView(HotelSystem hs)
    {
        hotel = hs;
        listModel = new DefaultListModel();
        run = false;
    }
    
    /**
     * Sets the view up as the correct type (manager, employee, or customer)
     * @param man the manager flag
     * @param emp the employee flag
     * @param cus the customer flag
     * @return true if successful and false if not successful
     */
    public boolean setFlags(boolean man, boolean emp, boolean cus)
    {
        manFlag = man;
        cusFlag = cus;
        empFlag = emp;
        return true;
    }
   
    /**
     * Method for speaking information to the user
     * @param str the string
     */
   public static void speak(String str)  // this is static
   {
       System.out.println(str);
   }
   
   /**
     * Method for speaking error information to the user
     * @param str the string
     */
   public static void speakError(String str)  // this is static
   {
       JOptionPane.showMessageDialog(null, str, "Hotel System", JOptionPane.ERROR_MESSAGE);
   }
    
   /**
     * Method for speaking custom information to the user
     * @param str the string
     */
   public static void speakInfo(String str)  // this is static
   {
       JOptionPane.showMessageDialog(null, str, "Hotel System", JOptionPane.INFORMATION_MESSAGE);
   }
   
   /**
    * Invalid input message warning.
    */
   public static void speakInvalid(){JOptionPane.showMessageDialog(null, "Invalid input.  Please try again.", "Hotel System", JOptionPane.WARNING_MESSAGE);}
     
   
    /**
    * Adding an element to a list
    * @param str the string
    */
   public static void listspeak(String str)
   {
       listModel.addElement(str);
   }
   
   /**
    * Clearing the list
    */
   public static void listClear()
   {
       listModel.clear();
   } 
   
   /**
    * Logout and go to login screen
    */   
   public void logout()
   {
       run = false;
       cusFlag = false;
       empFlag = false;
       manFlag = false;
       UserControls.LoginScreen();
   }
   
  /**
    * This method runs the appropriate view start screen
    *@param Name the name
    * @param ID the id
    * @param console the console
    * @return true while running and false when not running
    */
    public boolean runView(String Name,int ID,Scanner console)
    {
        if (cusFlag) 
        {
            customerMenu(ID);
            return true;
        }
        else if (empFlag)  
        {
            employeeMenu();
            return true;
        }
        if (manFlag)
        {
            //runEmployeeOption(console,Name);
            managerMenu();
            return true;
        }
        return true;
    }
    
    /**
     * The Customer Menu
     * @param ID the customer's id
     */
    public void customerMenu(int ID)
    {
        JFrame frame = new JFrame();
        frame.setTitle("Customer Menu");
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frame.setLayout(new BorderLayout());
        JPanel cusPanel = new JPanel();
        list = new JList(listModel);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list.setPreferredSize(new Dimension(600, 550));
        //cusPanel.add(list);
        cusPanel.add(new JScrollPane(list));
        frame.add(cusPanel, BorderLayout.CENTER);

        //This is all creating the bottom row of buttons 
        JButton leftButton = new JButton("Left");
        leftButton.setVisible(false);
        leftButton.setPreferredSize(new Dimension(150,26));
        JButton rightButton = new JButton("Right");
        rightButton.setVisible(false);
        rightButton.setPreferredSize(new Dimension(150,25));
        JButton logoutButton = new JButton("Logout");
        logoutButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    logout();
                    listModel.clear();
                    frame.dispose();
                }
            });
        JPanel bottomPanel = new JPanel();
        bottomPanel.add(leftButton);
        bottomPanel.add(logoutButton);
        frame.add(bottomPanel, BorderLayout.SOUTH);

        //This is creating the top row of the frame and setting there
        TextField start, end;
        start = new TextField("", 20);
        end = new TextField("", 20);
        JButton roomsButton = new JButton("Display Rooms");
        roomsButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    listModel.clear();
                    String startd = checkDate(start.getText());
                    String endd = checkDate(end.getText());
                    if (startd != "" && endd != "")
                    {
                        LocalDate startdate = LocalDate.parse(startd, DateTimeFormat.forPattern("MM/dd/yyyy"));
                        LocalDate enddate = LocalDate.parse(endd, DateTimeFormat.forPattern("MM/dd/yyyy"));
                        int numdays = Days.daysBetween(startdate, enddate).getDays();
                        if (numdays <= 0) speakError("Check out date must be after check in date");
                        else hotel.displayRoomsAvailable(startdate,enddate);
                        leftButton.setText("Reserve Room");
                        leftButton.setVisible(true);
                        for( ActionListener al : leftButton.getActionListeners() )
                            leftButton.removeActionListener( al );
                        leftButton.addActionListener(new ActionListener() 
                        {
                            public void actionPerformed(ActionEvent event)
                            {
                                try
                                {
                                    //Note here how to parse a string
                                    //This line will find a string and return the position of first character
                                    int priceindex = list.getSelectedValue().indexOf("Price:");
                                    //String.substring(first, last) creates a substring from first position to the last position
                                    hotel.bookRoom(true, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                    listModel.clear();
                                    hotel.displayRoomsAvailable(startdate, enddate);
                                }
                                catch(NullPointerException error)
                                {
                                     speakError("Error: please select a room");
                                }
                            }
                        });
                    }
                }
            });
        JButton reserveButton = new JButton("Display Reservations");
        reserveButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    listModel.clear();
                    hotel.displayCustomerBalance(ID);
                    leftButton.setText("Delete Reservation");
                    leftButton.setVisible(true);
                    for( ActionListener al : leftButton.getActionListeners() )
                        leftButton.removeActionListener( al );
                    leftButton.addActionListener(new ActionListener() 
                    {
                        public void actionPerformed(ActionEvent event)
                        {                
                            System.out.println(list.getSelectedIndex());
                            hotel.removeBookRoom(ID, list.getSelectedIndex());
                            listModel.clear();
                            hotel.displayCustomerBalance(ID);
                        }
                    });
                }
            });
        JPanel buttonPanel = new JPanel();
        buttonPanel.add(roomsButton);
        buttonPanel.add(reserveButton);
        JLabel startLabel = new JLabel("Start Date: ");
        buttonPanel.add(startLabel);
        buttonPanel.add(start);
        JLabel endLabel = new JLabel("End Date: ");
        buttonPanel.add(endLabel);
        buttonPanel.add(end);
        frame.add(buttonPanel, BorderLayout.NORTH);             

        //Setting some final settings for the frame itself
        frame.setPreferredSize(new Dimension(800, 600));
        frame.pack();
        frame.setVisible(true);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent event)
            {
                logout();
                listModel.clear();
                frame.dispose();
            }
        });
    }
    
    /**
     * The Employee Menu
     */
    public void employeeMenu()
    {
        JFrame frame = new JFrame();
        frame.setTitle("Employee Menu");
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frame.setLayout(new BorderLayout());
        JPanel empPanel = new JPanel();
        list = new JList(listModel);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list.setPreferredSize(new Dimension(600, 550));
        //empPanel.add(list);
        empPanel.add(new JScrollPane(list));
        frame.add(empPanel, BorderLayout.CENTER);
        
        TextField name;
        name = new TextField("", 20);
        JLabel nameLabel = new JLabel("          Customer Name: ");
        nameLabel.setVisible(true);
        name.setVisible(true);
        TextField start, end;
        start = new TextField("", 15);
        end = new TextField("", 15);
        
        JButton leftButton = new JButton("Left");
        leftButton.setVisible(false);
        JButton blankButton = new JButton("Blank");
        blankButton.setVisible(false);
        JButton logoutButton = new JButton("Logout");
        JButton checkInButton = new JButton("Check In");
        JButton checkOutButton = new JButton("Check Out");
        JButton roomsButton = new JButton("Display Rooms");
        checkInButton.setVisible(false);
        checkOutButton.setVisible(false);
        JButton displayCustomersButton = new JButton("Display All Customers");
        JButton displayAllReservationsButton = new JButton("Display All Reservations");
        JButton reserveButton = new JButton("Display Reservations");
        reserveButton.setPreferredSize(new Dimension(150,26));
        JButton addcustomerButton = new JButton("Add Customer");
        JButton editCustomerButton = new JButton("Edit Customer");
        JButton deleteCustomerButton = new JButton("Delete Customer");
        editCustomerButton.setVisible(false);
        deleteCustomerButton.setVisible(false);
        
        JPanel bottomPanel = new JPanel();
        bottomPanel.add(displayCustomersButton);
        bottomPanel.add(displayAllReservationsButton);
        bottomPanel.add(addcustomerButton);
        bottomPanel.add(logoutButton);
        frame.add(bottomPanel, BorderLayout.SOUTH);
        
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new GridLayout(0, 6, 2, 10));
        buttonPanel.add(roomsButton);
        buttonPanel.add(reserveButton);
        buttonPanel.add(leftButton);
        buttonPanel.add(blankButton);
        buttonPanel.add(checkInButton);
        buttonPanel.add(checkOutButton);
        JLabel startLabel = new JLabel("          Start Date:");
        buttonPanel.add(startLabel);
        buttonPanel.add(start);
        JLabel endLabel = new JLabel("          End Date:");
        buttonPanel.add(endLabel);
        buttonPanel.add(end);
        buttonPanel.add(nameLabel);
        buttonPanel.add(name);
        buttonPanel.add(editCustomerButton);
        buttonPanel.add(deleteCustomerButton);
        frame.add(buttonPanel, BorderLayout.NORTH); 
        
        addcustomerButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                editCustomerButton.setVisible(false);
                deleteCustomerButton.setVisible(false);
                addEmployeeorCustomer(false, true);
            }
        });
        
        logoutButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    logout();
                    listModel.clear();
                    frame.dispose();
                }
            });
      
        displayAllReservationsButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                editCustomerButton.setVisible(false);
                deleteCustomerButton.setVisible(false);
                listModel.clear();
                hotel.displayReservations();
            }
        });
        
        displayCustomersButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    editCustomerButton.setVisible(true);
                    deleteCustomerButton.setVisible(true);
                    listModel.clear();
                    hotel.displayCusList();
                }
            });
        
        editCustomerButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
//                editPerson(false, true);
                EditCustomer edCus = new EditCustomer(hotel);
                edCus.setVisible(true);
            }
        });
        
        deleteCustomerButton.addActionListener(new ActionListener() 
        {
            public void actionPerformed(ActionEvent event)
            {
                try
                {
                    int idIndex = list.getSelectedValue().indexOf("ID:");
                    int paymentIndex = list.getSelectedValue().indexOf("Payment");
                    String cusId = list.getSelectedValue().substring(idIndex+4, paymentIndex-1);
                    hotel.deleteCustomerManual(Integer.parseInt(cusId));
                    listModel.clear();
                }
                catch(NullPointerException error)
                {
                     speakError(error.toString());
                }
                catch (StringIndexOutOfBoundsException error)
                {
                    speakError(error.toString());
                }
            }
        });

        roomsButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    editCustomerButton.setVisible(false);
                    deleteCustomerButton.setVisible(false);
                    listModel.clear();
                    checkOutButton.setVisible(false);
                    String startd = checkDate(start.getText());
                    String endd = checkDate(end.getText());
                    if (startd != "" && endd != "")
                    {
                        LocalDate startdate = LocalDate.parse(startd, DateTimeFormat.forPattern("MM/dd/yyyy"));
                        LocalDate enddate = LocalDate.parse(endd, DateTimeFormat.forPattern("MM/dd/yyyy"));
                        int numdays = Days.daysBetween(startdate, enddate).getDays();
                        if (numdays <= 0) speakError("Check out date must be after check in date");
                        else hotel.displayRoomsAvailable(startdate,enddate);
                        checkInButton.setVisible(true);
                        leftButton.setText("Reserve Room");
                        leftButton.setVisible(true);
                        for( ActionListener al : leftButton.getActionListeners() )
                            leftButton.removeActionListener( al );
                        leftButton.addActionListener(new ActionListener() 
                        {
                            public void actionPerformed(ActionEvent event)
                            {
                                try
                                {
                                   int priceindex = list.getSelectedValue().indexOf("Price:");
                                   int ID = -1;
                                   String username = name.getText();
                                   if (hotel.findCustomer(username) > 0) 
                                   {
                                       ID = hotel.findCustomer(username);
                                       hotel.bookRoom(true, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                       listModel.clear();
                                       hotel.displayRoomsAvailable(startdate, enddate);
                                   }
                                   else
                                   {
                                        hotel.displayCusList();
                                        idFrame(false, true, false, false, false, priceindex, startdate, enddate, "noType");
                                   }
                                }
                               catch (NullPointerException error)
                                {
                                    speakError("Error");
                                }
                            }
                        });
                        for( ActionListener al : checkInButton.getActionListeners() )
                            checkInButton.removeActionListener( al );
                        checkInButton.addActionListener(new ActionListener()
                        {
                            public void actionPerformed(ActionEvent event)
                            {
                                try
                                {
                                    int priceindex = list.getSelectedValue().indexOf("Price:");
                                    int ID = -1;
                                    String username = name.getText();
                                    if (hotel.findCustomer(username) > 0) 
                                    {
                                        ID = hotel.findCustomer(username);
                                        hotel.bookRoom(true, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                        hotel.bookRoom(false, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                        listModel.clear();
                                        hotel.displayRoomsAvailable(startdate, enddate);
                                    }
                                    else
                                    {
                                         hotel.displayCusList();
                                         idFrame(false, false, false, true, false, priceindex, startdate, enddate, "noType");
                                    }
                                }
                                catch (NullPointerException error)
                                {
                                    speakError("Error");
                                }
                            }
                        });
                    }
                }
            });
       
        reserveButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    editCustomerButton.setVisible(false);
                    deleteCustomerButton.setVisible(false);
                    listModel.clear();
                    nameLabel.setVisible(true);
                    name.setVisible(true);
                    checkInButton.setVisible(false);
                    checkOutButton.setVisible(true);
                    for( ActionListener al : checkInButton.getActionListeners() )
                            checkInButton.removeActionListener( al );
                    String username = name.getText();
                    if (username != " ")
                    {
                        if (hotel.findCustomer(username) > 0)
                        {
                            int ID = hotel.findCustomer(username);
                            hotel.displayCustomerBalance(ID);
                        }
                        else
                        {
                             hotel.displayCusList();
                             String empty = "03/01/2015";
                             LocalDate emptydate = LocalDate.parse(empty, DateTimeFormat.forPattern("MM/dd/yyyy"));
                             idFrame(true, false, false, false, false, 0, emptydate, emptydate, "noType");
                        }
                    }
                    else speakError("Enter a customer name");
                    leftButton.setText("Delete Reservation");
                    leftButton.setVisible(true);
                    for( ActionListener al : leftButton.getActionListeners() )
                        leftButton.removeActionListener( al );
                    leftButton.addActionListener(new ActionListener() 
                    {
                        public void actionPerformed(ActionEvent event)
                        {                
                            System.out.println(list.getSelectedIndex());
                            String username = name.getText();
                            if (hotel.findCustomer(username) > 0)
                            {
                                int ID = hotel.findCustomer(username);
                                hotel.removeBookRoom(ID, list.getSelectedIndex());
                                listModel.clear();
                                //hotel.displayCustomerBalance(ID);
                            }
                            else
                            {
                                hotel.removeBookRoom(customerID, list.getSelectedIndex());
                                listModel.clear();
                            }
                        }
                    });
                    checkOutButton.addActionListener(new ActionListener() 
                    {
                        public void actionPerformed(ActionEvent event)
                        {                
                            System.out.println(list.getSelectedIndex());
                            String username = name.getText();
                            if (hotel.findCustomer(username) > 0)
                            {
                                int ID = hotel.findCustomer(username);
                                hotel.removeBookRoom(ID, list.getSelectedIndex());
                                listModel.clear();
                                //hotel.displayCustomerBalance(ID);
                            }
                            else
                            {
                                hotel.removeBookRoom(customerID, list.getSelectedIndex());
                                listModel.clear();
                            }
                        }
                    });
                    checkInButton.setVisible(true);
                    for( ActionListener al : checkInButton.getActionListeners() )
                            checkInButton.removeActionListener( al );
                    checkInButton.addActionListener(new ActionListener()
                    {
                         public void actionPerformed(ActionEvent event)
                         {
                             try{
                                System.out.println(list.getSelectedIndex());
                                String username = name.getText();
                                int startIndex = list.getSelectedValue().indexOf("Start:");
                                int endIndex = list.getSelectedValue().indexOf("End:");
                                int typeIndex = list.getSelectedValue().indexOf("Type:");
                                int balanceIndex = list.getSelectedValue().indexOf("Balance:");
                                String startString = list.getSelectedValue().substring(startIndex+7, endIndex-1);
                                LocalDate startdate = LocalDate.parse(startString, DateTimeFormat.forPattern("yyyy-MM-dd"));
                                String endString = list.getSelectedValue().substring(endIndex+5, endIndex+15);
                                LocalDate enddate = LocalDate.parse(endString, DateTimeFormat.forPattern("yyyy-MM-dd"));
                                String type = list.getSelectedValue().substring(typeIndex+6, balanceIndex-4);
                                if (hotel.findCustomer(username) > 0)
                                {
                                   int ID = hotel.findCustomer(username);
                                   hotel.bookRoom(false, ID, type, startdate, enddate);
                                   listModel.clear();
                                   hotel.displayCustomerBalance(ID);
                                }
                                else
                                {
                                    hotel.displayCusList();
                                    idFrame(false, false, false, false, true, 0, startdate, enddate, type);
                                }
                             }
                             catch(NullPointerException error)
                             {
                                 speakError("Error");
                             }
                         }
                    });
                }
            });
           
        frame.setPreferredSize(new Dimension(950, 600));
        frame.pack();
        frame.setVisible(true);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent event)
            {
                logout();
                listModel.clear();
                frame.dispose();
            }
        });
    }
    
    /**
     * The frame used when there are duplicate names
     * @param displayReserve the boolean for displaying individual reservations
     * @param reserve the boolean from reserving a room
     * @param deleteReserve the boolean for deleting a reservation
     * @param checkInWithoutReserve the boolean for checking in with out a reservation
     * @param checkInWithReserve the boolean for checking in with a reservation
     * @param priceindex the index if the word "Price:"
     * @param startdate the check in date
     * @param enddate the check out date
     * @param type the type of room
     */
    private void idFrame(boolean displayReserve, boolean reserve, boolean deleteReserve, boolean checkInWithoutReserve, boolean checkInWithReserve, int priceindex, LocalDate startdate, LocalDate enddate, String type)
    {
        JFrame frame = new JFrame();
        frame.setTitle("HMS");
        frame.setLayout(new BorderLayout());
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        TextField id = new TextField("", 20);
        JLabel idLabel = new JLabel("Customer ID: ");
        
        JButton enterButton = new JButton("Enter");
        enterButton.setVisible(true);
        enterButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                   try
                   {
                        int ID = Integer.parseInt(id.getText());
                        if (hotel.findCustomerName(ID) != null)
                        {
                            customerID = ID;
                            if (customerID != 0 && hotel.checkCustomerId(customerID) && displayReserve)
                            {
                                listModel.clear(); 
                                hotel.displayCustomerBalance(customerID);
                            }
                            if (customerID != 0 && hotel.checkCustomerId(customerID) && reserve)
                            {
                                hotel.bookRoom(true, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                listModel.clear();
                                hotel.displayRoomsAvailable(startdate, enddate);
                            }
                            if (customerID != 0 && hotel.checkCustomerId(customerID) && checkInWithoutReserve)
                            {
                                hotel.bookRoom(true, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                hotel.bookRoom(false, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                listModel.clear();
                                hotel.displayRoomsAvailable(startdate, enddate);
                            }
                             if (customerID != 0 && hotel.checkCustomerId(customerID) && checkInWithReserve)
                            {
                                hotel.bookRoom(false, ID, type, startdate, enddate);
                                listModel.clear();
                                hotel.displayRoomsAvailable(startdate, enddate);
                            }
                            frame.dispose();
                        }
                        else 
                        {
                            speakError("Name not found in customer List\n");
                            customerID = 0;
                            frame.dispose();
                        }
                   }
                   catch (NumberFormatException error)
                   {
                       speakError("Enter an integer");
                   }
                   catch (NullPointerException error)
                   {
                       speakError("Error");
                   }
                    
                }
            });
        
        JPanel idPanel = new JPanel();
        idPanel.add(idLabel);
        idPanel.add(id);
        frame.add(idPanel, BorderLayout.NORTH);   
        
        JPanel buttonPanel = new JPanel();
        buttonPanel.add(enterButton);
        frame.add(buttonPanel, BorderLayout.SOUTH);

        frame.setPreferredSize(new Dimension(300, 200));
        frame.pack();
        frame.setVisible(true);
    }         
    
    
    /**
     * Edit employee or customer information.
     * @param flag true for employee, false for customer
     * @param title dictates action of function (e.g. Edit Employee, Edit Customer)
     * @param id is unique number assigned to person
     * @param namez associated name of target person
     * @param second is pay rate if employee, pay type if customer
     */
    public void editEmployeeOrCustomer(boolean flag, String title, int id, String namez, String second){
        // recycle code from addEmployeeorCustomer - modified
        JFrame frame = new JFrame();
        frame.setTitle("Edit "+title);
        frame.setLayout(new BorderLayout());
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        TextField name = new TextField("", 20);
        JLabel nameLabel = new JLabel("       Name: ");
        TextField pay = new TextField("", 20);
        JLabel payLabel = new JLabel("       Pay: ");
        TextField payType = new TextField("", 20);
        JLabel paytypeLabel = new JLabel("       Pay Type: ");
        if(flag){
            paytypeLabel.setVisible(false);
            payType.setVisible(false);
        }
        else{
            payLabel.setVisible(false);
            pay.setVisible(false);
        }
        
        JButton cancelButton = new JButton("Cancel");
        cancelButton.setVisible(true);
        cancelButton.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent event){
                frame.dispose();
            }  // end actionPerformed
        });  // end cancelButton addActionListener
        
        JButton enterButton = new JButton("Enter");
        enterButton.setVisible(true);
        enterButton.addActionListener(new ActionListener(){
                public void actionPerformed(ActionEvent event){
                   try{
                       if(flag){
                           String empName = name.getText();
                           Double empPay = Double.parseDouble(pay.getText());
                           if (UserControls.stringCheck(empName))
                           hotel.addEmployee(empName, empPay);
                           listModel.clear();
                           frame.dispose();
                       }
                       else{
                           String cusName = name.getText();
                           String cusPayType = payType.getText();
                           if (cusPayType.length() < 30 && UserControls.stringCheck(cusPayType) && UserControls.stringCheck(cusName))
                           {
                               hotel.addCustomer(cusName, cusPayType);
                               listModel.clear();
                               frame.dispose();
                           }
                           else
                           {
                                speakError("Invalid Input");
                                frame.dispose();
                           }
                       }
                   }
                   catch (Exception e){
                       speakError("Error has occurred.");
                   }                    
                }  // end actionPerformed
            });  // end enterButton addActionListener
        
        JPanel topPanel = new JPanel();
        topPanel.setLayout(new GridLayout(3, 4, 0, 10));
        topPanel.add(nameLabel);
        topPanel.add(name);
        topPanel.add(payLabel);
        topPanel.add(pay);
        topPanel.add(paytypeLabel);
        topPanel.add(payType);
        frame.add(topPanel, BorderLayout.NORTH);   
        
        JPanel buttonPanel = new JPanel();
        buttonPanel.add(enterButton);
        buttonPanel.add(cancelButton);
        frame.add(buttonPanel, BorderLayout.SOUTH);

        frame.setPreferredSize(new Dimension(300, 200));
        frame.pack();
        frame.setVisible(true); 
                
    }  // end EditEmployeeOrCustomer function
    
    
    
    /**
     * Add an employee or customer
     * @param employee the boolean for employee
     * @param customer the boolean for customer
     */
    private void addEmployeeorCustomer(boolean employee, boolean customer)
    {
        JFrame frame = new JFrame();
        frame.setTitle("Add Unit");
        frame.setLayout(new BorderLayout());
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        TextField name = new TextField("", 20);
        JLabel nameLabel = new JLabel("       Name: ");
        TextField pay = new TextField("", 20);
        JLabel payLabel = new JLabel("       Pay: ");
        TextField payType = new TextField("", 20);
        JLabel paytypeLabel = new JLabel("       Pay Type: ");
        if (employee)
        {
            paytypeLabel.setVisible(false);
            payType.setVisible(false);
        }
        if (customer)
        {
            payLabel.setVisible(false);
            pay.setVisible(false);
        }
        
        JButton cancelButton = new JButton("Cancel");
        cancelButton.setVisible(true);
        cancelButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                frame.dispose();
            }
        });
        
        JButton enterButton = new JButton("Enter");
        enterButton.setVisible(true);
        enterButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                   try
                   {
                       if (employee)
                       {
                           String empName = name.getText();
                           //Double empPay = Double.parseDouble(pay.getText());
                           NumberFormat formatted = NumberFormat.getInstance(Locale.US);
                           formatted.setRoundingMode(RoundingMode.HALF_EVEN);
                           formatted.setMaximumFractionDigits(2);
                           formatted.setMinimumFractionDigits(2);
                           Double empPay = formatted.parse(pay.getText()).doubleValue();
                           String empPayString = formatted.format(empPay);
                           empPay = Double.parseDouble(empPayString);
                           if (UserControls.stringCheck(empName))
                           {
                                hotel.addEmployee(empName, empPay);
                                listModel.clear();
                                frame.dispose();
                           }
                           else speakError("Invalid input");
                           
                       }
                       if (customer)
                       {
                           String cusName = name.getText();
                           String cusPayType = payType.getText();
                           if (cusPayType.length() < 30 && UserControls.stringCheck(cusPayType) && UserControls.stringCheck(cusName))
                           {
                                hotel.addCustomer(cusName, cusPayType);
                                listModel.clear();
                                frame.dispose();
                           }
                           else speakError("Invalid input");
                       }
                   }
                   catch (NumberFormatException error)
                   {
                       speakError("Enter a number");
                   }
                   catch (NullPointerException error)
                   {
                       speakError("Error");
                   }
                   catch(ParseException error) 
                   {
                       speakError("Error");
                   }
                } 
            });
        
        JPanel topPanel = new JPanel();
        topPanel.setLayout(new GridLayout(3, 4, 0, 10));
        topPanel.add(nameLabel);
        topPanel.add(name);
        topPanel.add(payLabel);
        topPanel.add(pay);
        topPanel.add(paytypeLabel);
        topPanel.add(payType);
        frame.add(topPanel, BorderLayout.NORTH);   
        
        JPanel buttonPanel = new JPanel();
        buttonPanel.add(enterButton);
        buttonPanel.add(cancelButton);
        frame.add(buttonPanel, BorderLayout.SOUTH);

        frame.setPreferredSize(new Dimension(300, 200));
        frame.pack();
        frame.setVisible(true);
    }
    
    /**
     * Add a room or edit room price by type
     * @param addRoom the boolean for adding a room
     * @param changePrice the boolean for changing the price
     */
    private void addOrEditRoom(boolean addRoom, boolean changePrice)
    {
        JFrame frame = new JFrame();
        frame.setTitle("Hotel System Room");
        frame.setLayout(new BorderLayout());
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        TextField roomNumber = new TextField("", 20);
        JLabel roomNumberLabel = new JLabel("Room Number: ");
        TextField roomPrice = new TextField("", 20);
        JLabel roomPriceLabel = new JLabel("Room Price: ");
        if (addRoom)
        {
            roomPriceLabel.setVisible(false);
            roomPrice.setVisible(false);
        }
        if (changePrice)
        {
            roomNumberLabel.setVisible(false);
            roomNumber.setVisible(false);
        }
        
        JComboBox combo = new JComboBox();
        combo.addItem("SINGLE");
        combo.addItem("DOUBLE");
    
        JButton cancelButton = new JButton("Cancel");
        cancelButton.setVisible(true);
        cancelButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                frame.dispose();
            }
        });
        
        JButton enterButton = new JButton("Enter");
        enterButton.setVisible(true);
        enterButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                   try
                   {
                       int type = combo.getSelectedIndex()+1;
                       if (addRoom)
                       {
                            if (type == 1)
                            {
                                listModel.clear();
                                hotel.addRoom("SINGLE", Integer.parseInt(roomNumber.getText()), 100.00);
                                hotel.displayAllRooms();
                            }
                            if (type == 2)
                            {
                                listModel.clear();
                                hotel.addRoom("DOUBLE", Integer.parseInt(roomNumber.getText()), 200.00);
                                hotel.displayAllRooms();
                            }
                       }
                       if (changePrice)
                       {
                           if (type == 1)
                            {
                                listModel.clear();
                                hotel.editRoom("SINGLE", Double.parseDouble(roomPrice.getText()));
                                hotel.displayAllRooms();
                            }
                            if (type == 2)
                            {
                                listModel.clear();
                                hotel.editRoom("DOUBLE", Double.parseDouble(roomPrice.getText()));
                                hotel.displayAllRooms();
                            }
                       }
                       frame.dispose();
                       
                   }
                   catch (NumberFormatException error)
                   {
                       speakError("Enter a number");
                   }
                   catch (NullPointerException error)
                   {
                       speakError("Error");
                   }
                    
                }
            });
        
        JPanel topPanel = new JPanel();
        topPanel.add(roomNumberLabel);
        topPanel.add(roomNumber);
        topPanel.add(roomPriceLabel);
        topPanel.add(roomPrice);
        frame.add(topPanel, BorderLayout.NORTH);   
        
        JPanel buttonPanel = new JPanel();
        buttonPanel.add(enterButton);
        buttonPanel.add(cancelButton);
        frame.add(buttonPanel, BorderLayout.SOUTH);
        
        JPanel centerPanel = new JPanel();
        centerPanel.add(combo);
        frame.add(centerPanel, BorderLayout.CENTER);

        frame.setPreferredSize(new Dimension(300, 200));
        frame.pack();
        frame.setVisible(true);
    }
           
     
    /**
     * The Manager Menu
     */
    private void managerMenu()
    {
        //Create the window.
        JFrame mainframe = new JFrame("Manager Menu");
    
        //******employee options       
        JPanel frame = new JPanel();
        frame.setLayout(new BorderLayout());
        JPanel empPanel = new JPanel();
        list = new JList(listModel);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list.setPreferredSize(new Dimension(600, 550));
        //empPanel.add(list);
        empPanel.add(new JScrollPane(list));
        frame.add(empPanel, BorderLayout.CENTER);
        
        TextField customerName;
        customerName = new TextField("", 20);
        JLabel nameLabel = new JLabel("          Customer Name: ");
        nameLabel.setVisible(true);
        customerName.setVisible(true);
        TextField start, end;
        start = new TextField("", 15);
        end = new TextField("", 15);
        
        JButton leftButton = new JButton("Left");
        leftButton.setVisible(false);
        JButton blankButton = new JButton("Blank");
        blankButton.setVisible(false);
        JButton logoutButton = new JButton("Logout");
        logoutButton.setVisible(true);
        JButton checkInButton = new JButton("Check In");
        JButton checkOutButton = new JButton("Check Out");
        JButton roomsButton = new JButton("Display Rooms");
        checkInButton.setVisible(false);
        checkOutButton.setVisible(false);
        JButton displayCustomersButton = new JButton("Display All Customers");
        JButton displayAllReservationsButton = new JButton("Display All Reservations");
        JButton reserveButton = new JButton("Display Reservations");
        reserveButton.setPreferredSize(new Dimension(150,26));
        JButton addCustomerButton = new JButton("Add Customer");
        JButton editCustomerButton = new JButton("Edit Customer");
        JButton deleteCustomerButton = new JButton("Delete Customer");
        editCustomerButton.setVisible(false);
        deleteCustomerButton.setVisible(false);
        
        JPanel bottomPanel = new JPanel();
        bottomPanel.add(displayCustomersButton);
        bottomPanel.add(displayAllReservationsButton);
        bottomPanel.add(addCustomerButton);
        bottomPanel.add(logoutButton);
        frame.add(bottomPanel, BorderLayout.SOUTH);
        
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new GridLayout(0, 6, 2, 10));
        buttonPanel.add(roomsButton);
        buttonPanel.add(reserveButton);
        buttonPanel.add(leftButton);
        buttonPanel.add(blankButton);
        buttonPanel.add(checkInButton);
        buttonPanel.add(checkOutButton);
        JLabel startLabel = new JLabel("          Start Date:");
        buttonPanel.add(startLabel);
        buttonPanel.add(start);
        JLabel endLabel = new JLabel("          End Date:");
        buttonPanel.add(endLabel);
        buttonPanel.add(end);
        buttonPanel.add(nameLabel);
        buttonPanel.add(customerName);
        buttonPanel.add(editCustomerButton);
        buttonPanel.add(deleteCustomerButton);
        frame.add(buttonPanel, BorderLayout.NORTH); 
        
        addCustomerButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                editCustomerButton.setVisible(false);
                deleteCustomerButton.setVisible(false);
                addEmployeeorCustomer(false, true);
            }
        });
        
        logoutButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    logout();
                    listModel.clear();
                    mainframe.dispose();
                }
            });
      
        displayAllReservationsButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                listModel.clear();
                editCustomerButton.setVisible(false);
                deleteCustomerButton.setVisible(false);
                hotel.displayReservations();
            }
        });
        
        displayCustomersButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    listModel.clear();
                    hotel.displayCusList();
                    deleteCustomerButton.setVisible(true);
                    editCustomerButton.setVisible(true);
                }
            });
                       
        
        deleteCustomerButton.addActionListener(new ActionListener() 
        {
            public void actionPerformed(ActionEvent event)
            {
                
                DeleteCustomer delCus = new DeleteCustomer(hotel);
                delCus.setVisible(true);                
            }
        });
        
        editCustomerButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
//                editPerson(false, true);  // moor?
                EditCustomer edCus = new EditCustomer(hotel);
                edCus.setVisible(true);
            }
        });
        
        roomsButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    listModel.clear();
                    checkOutButton.setVisible(false);
                    editCustomerButton.setVisible(false);
                    deleteCustomerButton.setVisible(false);
                    String startd = checkDate(start.getText());
                    String endd = checkDate(end.getText());
                    if (startd != "" && endd != "")
                    {
                        LocalDate startdate = LocalDate.parse(startd, DateTimeFormat.forPattern("MM/dd/yyyy"));
                        LocalDate enddate = LocalDate.parse(endd, DateTimeFormat.forPattern("MM/dd/yyyy"));
                        int numdays = Days.daysBetween(startdate, enddate).getDays();
                        if (numdays <= 0) speakError("Check out date must be after check in date");
                        else hotel.displayRoomsAvailable(startdate,enddate);
                        checkInButton.setVisible(true);
                        leftButton.setText("Reserve Room");
                        leftButton.setVisible(true);
                        for( ActionListener al : leftButton.getActionListeners() )
                            leftButton.removeActionListener( al );
                        leftButton.addActionListener(new ActionListener() 
                        {
                            public void actionPerformed(ActionEvent event)
                            {
                                try
                                {
                                   int priceindex = list.getSelectedValue().indexOf("Price:");
                                   int ID = -1;
                                   System.out.println(list.getSelectedIndex());
                                   String username = customerName.getText();
                                   if (hotel.findCustomer(username) > 0) 
                                   {
                                       ID = hotel.findCustomer(username);
                                       hotel.bookRoom(true, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                       listModel.clear();
                                       hotel.displayRoomsAvailable(startdate, enddate);
                                   }
                                   else
                                   {
                                        hotel.displayCusList();
                                        idFrame(false, true, false, false, false, priceindex, startdate, enddate, "noType");
                                   }
                                }
                               catch (NullPointerException error)
                                {
                                    speakError("Error");
                                }
                            }
                        });
                        for( ActionListener al : checkInButton.getActionListeners() )
                            checkInButton.removeActionListener( al );
                        checkInButton.addActionListener(new ActionListener()
                        {
                            public void actionPerformed(ActionEvent event)
                            {
                                try
                                {
                                    int priceindex = list.getSelectedValue().indexOf("Price:");
                                    int ID = -1;
                                    String username = customerName.getText();
                                    if (hotel.findCustomer(username) > 0) 
                                    {
                                        ID = hotel.findCustomer(username);
                                        hotel.bookRoom(true, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                        hotel.bookRoom(false, ID, list.getSelectedValue().substring(11, priceindex-2), startdate, enddate);
                                        listModel.clear();
                                        hotel.displayRoomsAvailable(startdate, enddate);
                                    }
                                    else
                                    {
                                         hotel.displayCusList();
                                         idFrame(false, false, false, true, false, priceindex, startdate, enddate, "noType");
                                    }
                                }
                                catch (NullPointerException error)
                                {
                                    speakError("Error");
                                }
                            }
                        });
                    }
                }
            });
       
        reserveButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    listModel.clear();
                    nameLabel.setVisible(true);
                    customerName.setVisible(true);
                    checkInButton.setVisible(false);
                    checkOutButton.setVisible(true);
                    editCustomerButton.setVisible(false);
                    deleteCustomerButton.setVisible(false);
                    for( ActionListener al : checkInButton.getActionListeners() )
                            checkInButton.removeActionListener( al );
                    String username = customerName.getText();
                    if (username != " ")
                    {
                        if (hotel.findCustomer(username) > 0)
                        {
                            int ID = hotel.findCustomer(username);
                            hotel.displayCustomerBalance(ID);
                        }
                        else
                        {
                             hotel.displayCusList();
                             String empty = "03/01/2015";
                             LocalDate emptydate = LocalDate.parse(empty, DateTimeFormat.forPattern("MM/dd/yyyy"));
                             idFrame(true, false, false, false, false, 0, emptydate, emptydate, "noType");
                        }
                    }
                    else speakError("Enter a customer name");
                    leftButton.setText("Delete Reservation");
                    leftButton.setVisible(true);
                    for( ActionListener al : leftButton.getActionListeners() )
                        leftButton.removeActionListener( al );
                    leftButton.addActionListener(new ActionListener() 
                    {
                        public void actionPerformed(ActionEvent event)
                        {                
                            System.out.println(list.getSelectedIndex());
                            String username = customerName.getText();
                            if (hotel.findCustomer(username) > 0)
                            {
                                int ID = hotel.findCustomer(username);
                                hotel.removeBookRoom(ID, list.getSelectedIndex());
                                listModel.clear();
                                //hotel.displayCustomerBalance(ID);
                            }
                            else
                            {
                                hotel.removeBookRoom(customerID, list.getSelectedIndex());
                                listModel.clear();
                            }
                        }
                    });
                    checkOutButton.addActionListener(new ActionListener() 
                    {
                        public void actionPerformed(ActionEvent event)
                        {                
                            System.out.println(list.getSelectedIndex());
                            String username = customerName.getText();
                            if (hotel.findCustomer(username) > 0)
                            {
                                int ID = hotel.findCustomer(username);
                                hotel.removeBookRoom(ID, list.getSelectedIndex());
                                listModel.clear();
                                //hotel.displayCustomerBalance(ID);
                            }
                            else
                            {
                                hotel.removeBookRoom(customerID, list.getSelectedIndex());
                                listModel.clear();
                            }
                        }
                    });
                    checkInButton.setVisible(true);
                    for( ActionListener al : checkInButton.getActionListeners() )
                            checkInButton.removeActionListener( al );
                    checkInButton.addActionListener(new ActionListener()
                    {
                         public void actionPerformed(ActionEvent event)
                         {
                             try{
                                    String username = customerName.getText();
                                    int startIndex = list.getSelectedValue().indexOf("Start:");
                                    int endIndex = list.getSelectedValue().indexOf("End:");
                                    int typeIndex = list.getSelectedValue().indexOf("Type:");
                                    int balanceIndex = list.getSelectedValue().indexOf("Balance:");
                                    String startString = list.getSelectedValue().substring(startIndex+7, endIndex-1);
                                    LocalDate startdate = LocalDate.parse(startString, DateTimeFormat.forPattern("yyyy-MM-dd"));
                                    String endString = list.getSelectedValue().substring(endIndex+5, endIndex+15);
                                    LocalDate enddate = LocalDate.parse(endString, DateTimeFormat.forPattern("yyyy-MM-dd"));
                                    String type = list.getSelectedValue().substring(typeIndex+6, balanceIndex-4);
                                    if (hotel.findCustomer(username) > 0)
                                    {
                                       int ID = hotel.findCustomer(username);
                                       hotel.bookRoom(false, ID, type, startdate, enddate);
                                       listModel.clear();
                                       hotel.displayCustomerBalance(ID);
                                    }
                                    else
                                    {
                                        hotel.displayCusList();
                                        idFrame(false, false, false, false, true, 0, startdate, enddate, type);
                                    }
                             }
                             catch(NullPointerException error)
                             {
                                 speakError("Error");
                             }
                         }
                    });
                }
            });
           
        frame.setPreferredSize(new Dimension(950, 600));
        frame.setVisible(true);
   
        //*****manager options       
        JPanel managerframe = new JPanel();
        managerframe.setLayout(new BorderLayout());
        JPanel managerCenterPanel = new JPanel();
        list2 = new JList(listModel);
        list2.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list2.setPreferredSize(new Dimension(600, 550));
        //managerCenterPanel.add(list2);
        managerCenterPanel.add(new JScrollPane(list2));
        managerframe.add(managerCenterPanel, BorderLayout.CENTER);
        
        JButton managerLogoutButton = new JButton("Logout");
        JButton listRoomsButton = new JButton("Display Rooms");
        JButton displayEmployeesButton = new JButton("Display All Employees");
        JButton displayTotalButton = new JButton("Display Total");
        JButton addEmployee = new JButton("Add Employee");
        JButton addRoomButton = new JButton("Add Room");
        JButton editRoomButton = new JButton("Change Room Price");
        JButton editEmployeeButton = new JButton("Edit Employee");
        editEmployeeButton.setVisible(false);
        JButton deleteEmployeeButton = new JButton("Delete Employee");
        deleteEmployeeButton.setVisible(false);
        
        JPanel managerBottomPanel = new JPanel();
        managerBottomPanel.add(listRoomsButton);
        managerBottomPanel.add(displayEmployeesButton);
        managerBottomPanel.add(displayTotalButton);
        managerBottomPanel.add(managerLogoutButton);
        managerframe.add(managerBottomPanel, BorderLayout.SOUTH);
        
        JPanel managerTopPanel = new JPanel();
        managerTopPanel.add(addEmployee);
        managerTopPanel.add(addRoomButton);
        managerTopPanel.add(editRoomButton);
        managerTopPanel.add(editEmployeeButton);
        managerTopPanel.add(deleteEmployeeButton);
        managerframe.add(managerTopPanel, BorderLayout.NORTH); 
        
        addRoomButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                editEmployeeButton.setVisible(false);
                deleteEmployeeButton.setVisible(false);
                addOrEditRoom(true, false);
            }
        });
        
        editRoomButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                editEmployeeButton.setVisible(false);
                deleteEmployeeButton.setVisible(false);
                addOrEditRoom(false, true);
            }
        });
        
        addEmployee.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                addEmployeeorCustomer(true, false);
            }
        });
        
        managerLogoutButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    logout();
                    listModel.clear();
                    mainframe.dispose();
                }
            });
      
        listRoomsButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                editEmployeeButton.setVisible(false);
                deleteEmployeeButton.setVisible(false);
                listModel.clear();
                hotel.displayAllRooms();
            }
        });
        
        displayEmployeesButton.addActionListener(new ActionListener()
            {
                public void actionPerformed(ActionEvent event)
                {
                    listModel.clear();
                    hotel.displayEmpList();
                    editEmployeeButton.setVisible(true);
                    deleteEmployeeButton.setVisible(true);
                }
            });
                
                
        editEmployeeButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                EditEmployee edEmp =  new EditEmployee(hotel);
                edEmp.setVisible(true);
            }
        });

        deleteEmployeeButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {                
                DeleteEmployee delEmp = new DeleteEmployee(hotel);
                delEmp.setVisible(true);                
            }
        });
        
        displayTotalButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent event)
            {
                editEmployeeButton.setVisible(false);
                deleteEmployeeButton.setVisible(false);
                listModel.clear();
                hotel.displayTotalBalance();
            }
        });
        
        managerframe.setPreferredSize(new Dimension(950, 600));
        managerframe.setVisible(true);
        
       //tabbed section
        JTabbedPane tabbedPane = new JTabbedPane();

        tabbedPane.addTab("Manager Options", managerframe);
        tabbedPane.setMnemonicAt(0, KeyEvent.VK_1);
       
        tabbedPane.addTab("Employee Options", frame);
        tabbedPane.setMnemonicAt(1, KeyEvent.VK_2);
        
        ChangeListener changeListener = new ChangeListener() 
        {
            public void stateChanged(ChangeEvent changeEvent) 
            {
                listModel.clear();
                customerName.setText("");
                start.setText(""); 
                end.setText("");
                
            }
        };
        tabbedPane.addChangeListener(changeListener);
        
        tabbedPane.setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);
         
        mainframe.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        mainframe.add(tabbedPane, BorderLayout.CENTER);
        mainframe.pack();
        mainframe.setVisible(true);
        
        mainframe.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent event)
            {
                logout();
                listModel.clear();
                mainframe.dispose();
            }
        });
    }
    
    private JComponent makeTextPanel(String text) { //got this from a tutorial https://docs.oracle.com/javase/tutorial/displayCode.html?code=https://docs.oracle.com/javase/tutorial/uiswing/examples/components/TabbedPaneDemoProject/src/components/TabbedPaneDemo.java
        JPanel panel = new JPanel(false);
        JLabel filler = new JLabel(text);
        filler.setHorizontalAlignment(JLabel.CENTER);
        panel.setLayout(new GridLayout(1, 1));
        panel.add(filler);
        return panel;
    }
    
     /**
     * This method prompts user to enter a date in the correct format (MM/DD/YYYY)
     * @param date the date to check
     * @return the valid date
     */
    private String checkDate(String date)
    {
        String regex = "^([0]?[1-9]|[1][0-2])[./-]([0]?[1-9]|[1|2][0-9]|[3][0|1])[./-]([0-9]{4})$";
        String validdate = "";
        if (date.matches(regex))
        {
            validdate = date;
        }
        else
            speakError("Date must be in MM/DD/YYYY format");
        return validdate;
    }
    
    private double SINGLE_PRICE = 50;
    private double DOUBLE_PRICE = 75;
    private HotelSystem hotel;
    private boolean manFlag;
    private boolean cusFlag;
    private boolean empFlag;
    int MAX_ROOMS = 100;
    private int customerID = 0;
}
