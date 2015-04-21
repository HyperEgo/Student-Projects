package hotelmgmt;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Scanner;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import javax.swing.*;
import java.awt.*;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * This module provides controls for the user interface
 * @author Casey
 */
public class UserControls {
   public UserControls(Scanner aScanner)
   {
        console = aScanner;
        if (!readHotelSystemIn())
        {
            hs = HotelSystem.getInstance();  // added static instance of hs
        }
        view = new UserView(hs);
        LoginScreen();
   }
   
   /**
    * This module sets the user view based on what type of account
    * @param viewType the type of view
    * @return true if the view is set or false if the view was not set
    */
   public static boolean setView(String viewType)
   {
       if (viewType.toLowerCase() == "manager")
           view.setFlags(true, false, false);
       else if (viewType.toLowerCase() == "employee")
           view.setFlags(false, true, false);
       else if (viewType.toLowerCase() == "customer")
           view.setFlags(false,false,true);       
       else 
           return false;
       return true;
   }
   
   
   /**
    * Sanitize incoming strings.
    * @param str incoming string
    * @return boolean true if pass, false otherwise
    */
   public static boolean stringCheck(String str){       
       if(str.matches("^[a-zA-Z ]+$")){
           return true;
       }
       else{return false;}       
   }
   
   /**
    * Sanitize incoming integers.
    * @param testInt incoming integer
    * @return boolean true if pass, false otherwise
    */
   public static boolean integerCheck(String testInt){
        if(testInt.matches("^[1-9]\\d*$")){
            return true;
        }
        else{return false;}
   }
   
   /**
    * Sanitize incoming doubles.
    * @param testDouble incoming double
    * @return boolean true if pass, false otherwise
    */
   public static boolean doubleCheck(String testDouble){
       if(testDouble.matches("[0-9]{1,13}(\\.[0-9]*)?")){
           return true;
       }
       else{return false;}
   }
   
   
   
   @SuppressWarnings("unchecked")
   public static void LoginScreen()  // this is static
   {
       //while (true)
       {
            JFrame frame = new JFrame();
            // Create Panels
            JPanel masterPanel = new JPanel();
            JPanel topPanel = new JPanel();
            JPanel midPanel = new JPanel();
            JPanel lowerPanel = new JPanel();
            JPanel closePanel = new JPanel();

            // Set layouts
            masterPanel.setLayout(new GridLayout(4,1,10,5));
            topPanel.setLayout(new GridLayout(1,3,10,5));
            midPanel.setLayout(new BorderLayout ());
            lowerPanel.setLayout(new FlowLayout ());
            closePanel.setLayout(new BorderLayout ());               

            // Create Text Box
            JTextField nameTextField = new JTextField();

            // Create Labels
            JLabel nameLabel = new JLabel("NAME :");

            // Create Combobox
            final JComboBox combo = new JComboBox();
            combo.addItem("Manager");
            combo.addItem("Employee");
            combo.addItem("Customer");     

            // Create Buttons
            JButton registerButton = new JButton("  Register  ");
            JButton loginButton = new JButton   ("  Login     ");
            JButton closeButton = new JButton("Close");

            // Add ActionListeners
            registerButton.addActionListener(new 
                    ActionListener()
                    {
                        public void actionPerformed(ActionEvent event)
                        {
                            frame.setVisible(false);
                            if(stringCheck(nameTextField.getText())){  // sanitize strings; exit on false
                                registerAction(nameTextField.getText(),combo.getSelectedIndex()+1);
//                                frame.setVisible(true);
                                frame.dispose();                                                                
                            }
                            else{
                                UserView.speakInvalid();
                                //closeAction();
                                LoginScreen();
                            }
                        }
                    });

            loginButton.addActionListener(new 
                    ActionListener()
                    {
                        public void actionPerformed(ActionEvent event)
                        {
                            frame.setVisible(false);
                            if(stringCheck(nameTextField.getText())){  // sanitize strings; exit on false
                                loginAction(nameTextField.getText(),combo.getSelectedIndex()+1);
                                frame.dispose();                                
                            }
                            else{
                                UserView.speakInvalid();
                                //closeAction();
                                LoginScreen();
                            }                            
                        }
                    });

            closeButton.addActionListener(new 
                    ActionListener()
                    {
                        public void actionPerformed(ActionEvent event)
                        {
                            closeAction();
                        }
                    });
            
            // added separators
            JSeparator jseph =  new JSeparator(JSeparator.HORIZONTAL);
            jseph.setPreferredSize(new Dimension(25,0));
            
            JSeparator jsepv =  new JSeparator(JSeparator.VERTICAL);
            jsepv.setPreferredSize(new Dimension(0,10));            
            

            topPanel.add(nameLabel);
            topPanel.add(nameTextField);
            midPanel.add(combo,BorderLayout.NORTH);
            lowerPanel.add(registerButton);
            lowerPanel.add(jseph);
            lowerPanel.add(loginButton);
            closePanel.add(closeButton,BorderLayout.NORTH);
            masterPanel.add(topPanel);
            masterPanel.add(midPanel);
            masterPanel.add(lowerPanel);
            masterPanel.add(closePanel);

            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

            frame.add(masterPanel);
            frame.pack();
            frame.setResizable(false);  // resizing frame makes buttons and dialogue skewed
            frame.setAlwaysOnTop(true);
            frame.setTitle("Hotel Management");  // original text to long for frame size
            frame.setSize(300,200);
            frame.setVisible(true);
            frame.setLocationRelativeTo(null);  // center dialogue
             
            frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent event)
            {
                writeHotelSystemOut();
                System.exit(0);
            }
        });
       }
   }
   
   @SuppressWarnings("unchecked")
   private static void registerAction(String name, int type)
   {      
       int ID = -1; 
       if (!isValidUserName(name))
        {
            JOptionPane.showMessageDialog(null,
                                         "Invalid Name Provided",
                                         "Hotel Management System",
                                         JOptionPane.ERROR_MESSAGE);
            //LoginScreen();
            return;
        }
        if (type == 3)
        {
            setView("customer");
            String input = JOptionPane.showInputDialog(null,
                    "Enter the payment type:");
            if (input != null)
            { 
                ID = hs.addCustomer(name, input);
                view.runView(name,ID,console);
                frame.setVisible(false);
                frame.dispose();
            }
            else 
            {
                UserView.speakError("Error: did not enter payment type");
                LoginScreen();
            }
        }
        else
        {
            if (type == 1) setView("manager");
            else if (type == 2) setView("employee");
        
            try
            {
                double num = Double.parseDouble(JOptionPane.showInputDialog(null,
                    "Employee Pay Rate:"));
                ID = hs.addEmployee(name, num);
                view.runView(name,ID,console);
                frame.setVisible(false);
                frame.dispose();
            }
            catch (NumberFormatException ex)
            {
                UserView.speakError(ex.toString());
                LoginScreen();
            }
            catch (NullPointerException ex)
            {
                UserView.speakError(ex.toString());
                LoginScreen();
            }
        }
        
        //view.runView(name,ID,console);
        //frame.setVisible(false);
        //frame.dispose();
   }
   
   private static void loginAction(String name, int type)
   {
        int num = -1;
        if (!isValidUserName(name))
        {
            JOptionPane.showMessageDialog(null,
                                         "Invalid Name Provided",
                                         "Hotel Management System",
                                         JOptionPane.ERROR_MESSAGE);
            return;
        }
        if (type == 3)
        {
            setView("customer");
            num = hs.findCustomer(name);
            if (num < 0)
            {
                try 
                {
                    num = Integer.parseInt(JOptionPane.showInputDialog(null,
                    "Customer ID Number:"));
                    if (!hs.checkCustomerId(num))
                    {
                        UserView.speakError("ID Number not found.");
                        LoginScreen();
                    }
                    else
                    {
                        view.runView(name,num,console);
                        frame.setVisible(false);
                        frame.dispose();
                    }
                }
                catch (NumberFormatException ex)
                {
                    UserView.speakError(ex.toString());
                    LoginScreen();
                    
                }
                catch (NullPointerException ex)
                {
                    UserView.speakError(ex.toString());
                    LoginScreen();
                }
            } 
            else
            {
                view.runView(name,num,console);
                frame.setVisible(false);
                frame.dispose();
            }
        }
        else
        {
            if (type == 1) setView("manager");
            else if (type == 2) setView("employee");
            num = hs.findEmployee(name);
            if (num < 0)
            {
                try
                {
                    num = Integer.parseInt(JOptionPane.showInputDialog(null,
                        "Employee ID Number:"));
                    if (!hs.checkEmployeeId(num))
                    {
                        UserView.speakError("ID Number not found.");
                        LoginScreen();
                    }
                    else
                    {
                        view.runView(name,num,console);
                        frame.setVisible(false);
                        frame.dispose();
                    }
                }
                catch (NumberFormatException ex)
                {
                    UserView.speakError(ex.toString());
                    LoginScreen();
                }
                catch (NullPointerException ex)
                {
                    UserView.speakError(ex.toString());
                    LoginScreen();
                }
            }
            else
            {
                view.runView(name,num,console);
                frame.setVisible(false);
                frame.dispose();
            }
        }
        //view.runView(name,num,console);
        //frame.setVisible(false);
        //frame.dispose();
   }
   
   @SuppressWarnings("unchecked")
   private static void closeAction()
   {
       writeHotelSystemOut();
       System.exit(0);       
   }
      
   /**
     * This method forces user to provide an appropriate name
     * @param name the user name
     * @return true if name is valid and false if the name is invalid
     */
    @SuppressWarnings("unchecked")
    private static boolean isValidUserName(String name) {
       if (name.length() >= 5
           && name.length() <= 30)
       {
           return true; 
       }
       else return false;           
    }
    
    /**
     * This function writes the serialized hotel system to the save file
     * @return true if successful and false if not successful
     */
    @SuppressWarnings("unchecked")
    private static boolean writeHotelSystemOut() 
    {    
        try
        {
            FileOutputStream fout = new FileOutputStream("hotel.save");
            ObjectOutputStream oos = new ObjectOutputStream(fout);
            oos.writeObject(hs);
        } catch (FileNotFoundException ex)
        {
            System.out.println("The source file hotel.save does not exist");
            ex.printStackTrace();
            return false;
        } catch (IOException ex)
        {
            System.out.println("There was an exception thrown while trying to write out to hotel.save");
            ex.printStackTrace();
            return false;
        }
        return true;
    }
    
    /**
     * This function reads in the serialized class from the save file
     * @return true if successful and false if not successful 
     */
    private boolean readHotelSystemIn()
    {
        File f = new File("hotel.save");
        if(f.exists()) 
        {
            try
            {  
                FileInputStream fin = new FileInputStream(f);
                ObjectInputStream ois = new ObjectInputStream(fin);
                hs = (HotelSystem)ois.readObject();
            }
            catch (Exception ex)
            {
                System.out.println("There was an exception occurr while trying to read in hotelSystem\n");
                return false;
            }
        }
        else
            return false;
        return true;
    }
    
   String name;
   int ID;
   private static Scanner console;
   private static HotelSystem hs;
   private static UserView view;
   static JFrame frame = new JFrame();
}
