

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hotelmgmt;

import java.util.ArrayList;
import java.util.*;
import java.io.Serializable;
import org.joda.time.*;

/**
 * Connects UserView to HotelSystem.  The purpose of this class is to keep
 * track of: customer purchases, customer | employee lists and act as a general
 * usage common aggregate interface for basic hotel functionality.
 * @author fc, edit by entire team
*/
public class HotelSystem implements Serializable{
    
    RoomManager roomMan = new RoomManager();    
    
    Random rand = new Random();  // create random number generator
    int min = 1;
    int max = 9999999;    
    
    private ArrayList<Employee> empList = new ArrayList<Employee>();  // employee array list    
    private ArrayList<Customer> cusList = new ArrayList<Customer>();  // customer array list
    
    private HashMap<Integer, ArrayList<Purchase>> cAccounts = new HashMap<Integer, ArrayList<Purchase>>();  // hash map for customer accounts
    
    private static HotelSystem singleton = new HotelSystem();  // Hotel System singleton member variable
    
    
    /**
     * A private constructor that prevents any other class from instantiating.
     * Call getInstance() to instantiate HotelSystem object.
     */
    private HotelSystem(){}  // private
    
    
    /**
     * Static 'instance' method.
     * @return singleton instance of HotelSystem
     */
    public static HotelSystem getInstance(){  // call this function to instantiate HotelSystem object
        return singleton;        
    }
    
    
    /**
     * Returns 'copy' of employee array list.
     * @return empList clone
     */
    public ArrayList<Employee> getEmployeeList(){
        ArrayList<Employee> tempy = new ArrayList<Employee>(empList);  // shallow copy
        return tempy;
    }            
    
    
    /**
     * Displays the contents of the employee list.
     * MANAGEMENT view access function.
     */
    public void displayEmpList(){
        if(empList.isEmpty()){UserView.speakInfo("Employee list is empty.");}
        else{
            //UserView.speak("\nEMPLOYEES");
            for(Employee e : empList) {
                UserView.listspeak(e.getEmpName()+" ID: "+e.getEmpId()+" Rate: $"+e.getEmpPay());              
            }                        
        }
    }
    
    
    /**
     * Creates a new employee by using the supplied name and pay rate, 
     * assigns an id number and adds the employee to the account.
     * MANAGEMENT view access function.
     * @param name is employee name
     * @param pay is employee pay rate
     */
    public int addEmployee(String name, double pay) {
        int ran = rand.nextInt((max-min)+1)+min;  // generate random number        
        empList.add(new Employee(name, ran, pay));  // create employee object, add to list        
        UserView.speakInfo("Employee: "+name+" is added.  ID: "+ran); 
        return ran;
    }
    
    
    /**
     * Search for an employee id number, given the name as the argument.
     * @param name is employee name
     * @return employee id if found, nothing found return -1, if duplicates return -1
     */
    public int findEmployee(String name){
        int cnt=0;
        for(int i=0; i<empList.size(); i++){  // run loop to test for duplicates
            Employee e = empList.get(i);
            if((e.getEmpName()).equals(name)){  // similar names may have different capitalizations
                cnt++;
            } 
        }
        if(cnt==1){  // no duplicates exist, go get first entry
            for(int j=0; j<empList.size(); j++){
                Employee p = empList.get(j);
                if((p.getEmpName()).equals(name)){return p.getEmpId();}
            }            
        }                        
        if(cnt>1){  // duplicate entries exist, direct user to run display list to verify
            //UserView.speak("Duplicate names exist. Please run display list to verify.");
            return -1;
        }
        else{  // no entries exist, inform user no entry found
            UserView.speak("Name not found.");
            return -1;
        }
    }
    
    
     /**
     * Checks to see if an employee id is valid, assumes all id numbers are unique.
     * @param id is customer id number.
     * @return boolean true if found, false otherwise
     */
    public boolean checkEmployeeId(int id){
        for(int i=0; i<empList.size(); i++){
            Employee e = empList.get(i);
            if(id==e.getEmpId()){return true;}
        }
        return false;
    }    
    
    
    /**
     * Delete an employee, takes a name as the argument.
     * Calls findEmployee to acquire id number.
     * @param name is employee name
     * @return boolean true if delete successful, false otherwise
     */
    public boolean deleteEmployeeAuto(String name){        
        int j = findEmployee(name);
        if(j!=-1){  // id not equal -1: success
            for(int i=0; i<empList.size(); i++){
                Employee e = empList.get(i);
                if(j==e.getEmpId()){
                    empList.remove(e);
                    UserView.speakInfo("Employee removed: "+e.getEmpName());
                    return true;
                }
            }
        }
        else {
            UserView.speak("Error or duplicates exist.  Please try again or run manual delete.");
        }
        return false;
    }
    
    
    /**
     * Delete an employee, takes id as the argument.
     * Can run display list first, then manually enter id to delete.
     * @param id is employee id number
     */
    public boolean deleteEmployeeManual(int id){
        for(int i=0; i<empList.size(); i++){
            Employee e = empList.get(i);
            if(id==e.getEmpId()) {
                empList.remove(e);
//                UserView.speakInfo("Employee removed manually ID: "+e.getEmpId());
                return true;
            }
        }
//        UserView.speakError("Error or duplicates exist.  Please try again.");
        return false;
    }
    
    
    /**
     * Returns 'copy' of customer array list.
     * @return cusList clone
     */
    public ArrayList<Customer> getCustomerList(){
        ArrayList<Customer> tempy = new ArrayList<Customer>(cusList);  // shallow copy
        return tempy;
    }
    
    
    /**
     * Displays contents of customer list
     * MANAGEMENT view access function.
     */
    public void displayCusList(){
        if(cusList.isEmpty()){UserView.speakInfo("Customer list is empty.");}
        else {
            //UserView.speak("\nCUSTOMERS");
            for(Customer c : cusList){
                UserView.listspeak(c.getName()+" ID: "+c.getId()+" Payment type: "+c.getPayment());
            }                                   
        }
    }
    
    
    /**
     * Creates a customer account; uses a given name, assigns an id number and 
     * adds it to customer list.  Also, injects the new customer in to the accounts object.
     * @param name is customer name
     */
    public int addCustomer(String name, String paytype) {        
        int ran = rand.nextInt((max-min)+1)+min;  // generate random number        
        cusList.add(new Customer(name, ran, paytype));  // create customer object, add to customer list
        cAccounts.put(ran, new ArrayList<Purchase>());  // intialize purchase array list, add custommer to hash map object
        UserView.speakInfo("Customer: "+name+" is added.  ID: "+ran);
        return ran;
    }
    
    
    /**
     * Search for a customer id number, takes a customer name as the argument.
     * @param name is customer name
     * @return customer id if found, nothing found return -1, if duplicates return -1
     */
    public int findCustomer(String name){
        int cnt=0;
        for(int i=0; i<cusList.size(); i++){  // run loop to test for duplicates
            Customer c = cusList.get(i);
            if((c.getName()).equals(name)){  // similar names may have different capitalizations
                cnt++;
            } 
        }
        if(cnt==1){  // no duplicates exist, go get first entry
            for(int j=0; j<cusList.size(); j++){
                Customer t = cusList.get(j);
                if((t.getName()).equals(name)){return t.getId();}
            }            
        }                        
        if(cnt>1){  // duplicate entries exist, direct user to run display list to verify
            //UserView.speak("Duplicate names exist. Please run display list to verify.");
            return -1;
        }
        else{  // no entries exist, inform user no entry found
            //UserView.speakError("Name not found.");
            return -1;
        }        
    }
    
    
    /**
     * Find a customer name given an id number as the argument.
     * Use in conjunction with checkCustomerId to avoid null exceptions.
     * @param id is customer id number
     * @return customer name if found, null otherwise
     */
    public String findCustomerName(int id){
        for(int i=0; i<cusList.size(); i++){
            Customer c = cusList.get(i);
            if(id==c.getId()){return c.getName();}
        }
        return null;
    }
    
    
    /**
     * Checks to see if customer id is valid, assumes all id numbers are unique.
     * @param id is customer id number.
     * @return boolean true if found, false otherwise
     */
    public boolean checkCustomerId(int id){
        for(int i=0; i<cusList.size(); i++){
            Customer c = cusList.get(i);
            if(id==c.getId()){return true;}
        }
        return false;
    }
    
    
    /**
     * Delete customer account.  Takes name as argument.
     * Calls findCustomer to acquire id number.
     * @param name is customer name
     * @return boolean true if delete successful, false otherwise
     */
    public boolean deleteCustomerAuto(String name){
        int j = findCustomer(name);
        if(j!=-1){  // id not equal -1: success
            for(int i=0; i<cusList.size(); i++){
                Customer c = cusList.get(i);
                if(j==c.getId()){
                    cusList.remove(c);  // remove customer from array list                    
                    cAccounts.remove(c.getId());  // remove customer from accounts hash map
                    UserView.speakInfo("Customer removed: "+c.getName());
                    return true;
                }
            }
        }
        else {
            UserView.speakError("Error or duplicates exist.  Please try again or run manual delete.");
        }
        return false;
    }
    
    
    /**
     * Delete customer account.  Takes id as argument.
     * Can run display list first, then manually enter id to delete.
     * @param id is customer id number
     * @return boolean true if delete successful, false otherwise
     */
    public boolean deleteCustomerManual(int id){
        for(int i=0; i<cusList.size(); i++){
            Customer c = cusList.get(i);
            if(id==c.getId()) {
                cusList.remove(c);
                cAccounts.remove(c.getId());
//                UserView.speakInfo("Customer removed manually ID: "+c.getId());
                return true;
            }
        }
//        UserView.speakError("Error or duplicates exist.  Please try again.");
        return false;
    }
    
    
    /**
     * Display room types available with prices + amenities.  Call Room Manager.
     * @param in is date check in
     * @param out is date check out
     */       
    public void displayRoomsAvailable(LocalDate in, LocalDate out){roomMan.roomsAvailable(in, out);}
    
    
    /**
     * Display all rooms in Hotel.  MANAGEMENT view access function.
     */
    public void displayAllRooms(){roomMan.displayAllRooms();}
    
    
    /**
     * Books or reserves a room for a customer.  Set flag true to reserve, false to book.
     * Checks with Room Manager for room type availability, if true returns price 
     * and sets customer balance.
     * @param reserve is flag to indicate room reserve or booking
     * @param id is customer id number (must be valid and unique)
     * @param roomType is type of room the customer is interested in
     * @param in is date check in
     * @param out is date check out
     * @return boolean true if room booking is successful, false otherwise
     */
    public boolean bookRoom(boolean reserve, int id, String roomType, LocalDate in, LocalDate out){        
        if(checkCustomerId(id)){  // check if customer id exists, proceed to booking room
            ArrayList<Purchase> purchaseList = cAccounts.get(id);  // get pointer to array list object for this customer            
            if(reserve){  // customer makes reservation
                for(int i=0; i<purchaseList.size(); i++){  // check for reservations in purchase list
                    Purchase p = purchaseList.get(i);
                    if(p.getReserve()){  // look at reservations in purchase list
                        if(p.getRoomType().equals(roomType) && p.getDateIn().isEqual(in) && p.getDateOut().isEqual(out)){
                            UserView.speakInfo("Reservation already exists.");
                            return false;                        
                        }  // end if statement; compare reservation: room type, local date in | out                    
                    }  // end if statement; check purchase reserve flag                
                }  // end for loop; purchase list scan
                if(roomMan.isRoomAvailable(roomType, in, out)){  // ask Room Manager if room type and dates are available
                    Purchase pur = new Purchase();  // room available; proceed to make reservation
                    pur.setRoomType(roomType);
                    int numberOfDays = Days.daysBetween(in, out).getDays();
                    pur.setBalance(roomMan.getRoomPrice(roomType) * numberOfDays);
                    pur.setDateIn(in);
                    pur.setDateOut(out);
                    pur.setReserve(true);
                    purchaseList.add(pur);  // add reservation to purchase list
                    roomMan.makeReserve(roomType, in, out);  // notify Room Manager of reservation
                    UserView.speakInfo("Reservation made.");
                    //displayCustomerBalance(id); I took this out because it will add to list display
                    return true;
                }
                else{  // room not available
                    UserView.speakInfo("Room type or dates not available.");
                    return false;                    
                }                
            } // end if statement for bookRoom reserve flag true
            
            else{  // customer is not making a reservation
                for(int i=0; i<purchaseList.size(); i++){  // check for prior reservations in purchase list
                    Purchase p = purchaseList.get(i);
                    if(p.getReserve()){  // look at reservations in purchase list & compare to input args
                        if(p.getRoomType().equals(roomType) && p.getDateIn().isEqual(in) && p.getDateOut().isEqual(out)){
                            p.setReserve(false);  // change reserve status of purchase
                            for(int j=0; j<cusList.size(); j++){  // find customer object for Room Manager
                                Customer c = cusList.get(j);
                                if(id==c.getId()){
                                    int r = roomMan.checkIn(roomType, c);
                                    p.setRoom(r);
                                    UserView.speakInfo("Removed prior reservation, checked it.");
                                    //displayCustomerBalance(id); took out because it will display to list
                                    return true;
                                }  // end if statement; compare given id to customer id
                            }  // end for loop; customer object search                                                        
                        }  // end if statement; compare reservation: room type, local date in | out                    
                    }  // end if statement; check purchase reserve flag                
                }  // end for loop; purchase list scan
                
                // previous for loop falls through; no reservations match input args, proceed with normal check-in
                for(int k=0; k<cusList.size(); k++){  // find customer for Room Manager
                    Customer c = cusList.get(k);
                    if(id==c.getId()){
                        roomMan.makeReserve(roomType, in, out);
                        int r = roomMan.checkIn(roomType, c);
                        Purchase pur = new Purchase();  // make new Purchase; mark as check-in
                        pur.setRoomType(roomType);
                        int numberOfDays = Days.daysBetween(in, out).getDays();
                        pur.setBalance(roomMan.getRoomPrice(roomType) * numberOfDays);
                        pur.setDateIn(in);
                        pur.setDateOut(out);
                        pur.setRoom(r);
                        purchaseList.add(pur);  // add to purchase list
                        UserView.speakInfo("Check-in made.");
                        //displayCustomerBalance(id); displays to list object
                        return true;                        
                    }  // end if statement; compare customer id to given id
                }  // end for loop; customer list search                                                                
            } // end else statement; customer not making reservation
            UserView.speakError("Error occurred during check-in.  Please try again.");
            return false;
        }  // end if statement; checkCustomerId
        else {
            UserView.speakError("Error with customer id.  Check id or try again.");
            return false;
        }  // end else statement; checkCustomerId   
    }  // end bookRoom
           
    
    /**
     * Remove booking or reservation for a customer.  Notifies Room Manager  
     * that the room can be placed back in to inventory or service.  Calls 
     * displayCustomerBalance to confirm that the changes were successful.
     * @param id is customer id number
     * @param index is numeric option shown when displayCustomerBalance runs (assumes integer)
     * @return boolean true if removal successful, false otherwise
     */
    public boolean removeBookRoom(int id, int index){
        index=index;  // account for zero based array list (zero index not shown to customer) ***had to remove -1 Everett
        if(checkCustomerId(id)){  // check customer id
            ArrayList<Purchase> purchaseList = cAccounts.get(id);
            if(purchaseList.size()>0){  // check purchase list size > 0
                try{  // test index out of bounds
                    Purchase p = purchaseList.get(index);  // find particular purchase to remove
                    // index marked as reservation to remove
                    if(p.getReserve()){roomMan.removeReservation(p.getRoomType(), p.getDateIn(), p.getDateOut());}
                    // index marked as regular check out
                    else{
                        roomMan.checkOut(p.getRoomType(), p.getRoom());
                        roomMan.removeReservation(p.getRoomType(), p.getDateIn(), p.getDateOut());
                    }                      
                    UserView.speakInfo("Customer: "+findCustomerName(id)+" | Room Type: "+p.getRoomType()+" | Removal successful.\n");
                    purchaseList.remove(p);
                    displayCustomerBalance(id);
                    return true;                                              
                }
                catch(IndexOutOfBoundsException e){
                    UserView.speakError("Option not available.");
                    return false;                                        
                }                
            }
            else{
                UserView.speakError("No reservations or bookings present.");
                return false;
            }                                  
        }
        UserView.speakError("Customer not found.  Please check id or try again.");
        return false;
    }
    
    
    /**
     * Display total Hotel reservations only.  Run displayTotalBalance to see total
     * Hotel balances (regular bookings plus reservations) or run displayCustomerBalance 
     * to see individual customer reservations and bookings.
     */
    public void displayReservations(){
        double sum=0;
        UserView.listspeak("\n<-- Hotel RESERVATIONS -->");
        for(HashMap.Entry<Integer, ArrayList<Purchase>> entry : cAccounts.entrySet()){
            int id = entry.getKey();  // call pointer for each id number
            ArrayList<Purchase> purchaseList = cAccounts.get(id);  // get pointer to each customer array list                        
            double sumt=0;   
            for(Purchase p : purchaseList){
                if(p.getReserve()){  // add reservations only
                    sumt+=p.getBalance();
                    sum+=p.getBalance();
                }                
            }  // only print reservations with balances
            if(sumt>0){UserView.listspeak("ID "+id+" Customer: "+findCustomerName(id)+" | Balance >> $"+sumt);}            
        }
        UserView.listspeak("\nReservation Total = $"+sum);                        
    }       
    
    
    
    /**
     * Add room in to service.  MANAGEMENT view access.
     * Call Room Manager.
     * @param type is room type
     * @param room is room number
     * @param price is room price
     */
    public void addRoom(String type, int room, double price){
        
        roomMan.addRoom(type, room, price);
    }
    
    /**
     * 
     * @param type
     * @param price 
     */
    public void editRoom(String type, double price) {
        roomMan.changePrice(type, price);
    }
    
    /**
     * Delete room from service.  MANAGEMENT view access.
     * Call Room Manager.
     * @param type is room type
     * @param room is room number
     */
    public void deleteRoom(String type, int room){
        
        roomMan.deleteRoom(type, room);
    }
    
    
    /**
     * Edit employee name and pay.  Takes employee id as argument.
     * MANAGEMENT view access function.
     * @param id is employee id
     * @param name is employee name
     * @param pay is employee pay
     * @return boolean true if changes succeed, false otherwise
     */
    public boolean editEmployee(int id, String name, double pay){
//        UserView.speakInfo("Inside HS editEmployee >> "+id+" "+name+" "+pay);  // testing
        if(checkEmployeeId(id)){
            for(int i=0; i<empList.size(); i++){
                Employee e = empList.get(i);
                if(id==e.getEmpId()){
                    e.setEmpName(name);
                    e.setPay(pay);
                }                
            }
//            UserView.speakInfo("Changes are successful.");
            return true;
        }
//        UserView.speakError("Error occurred.  Please try again or check id.");
        return false;        
    }
    
    
    /**
     * Edit customer name and payment type.  Take customer id as argument.
     * @param id is customer id
     * @param name is customer name
     * @param payment is customer payment type
     * @return boolean true if changes succeed, false otherwise
     */
    public boolean editCustomer(int id, String name, String payment){
        if(checkCustomerId(id)){
            for(int i=0; i<cusList.size(); i++){
                Customer c = cusList.get(i);
                if(id==c.getId()){
                    c.setName(name);
                    c.setPayment(payment);                    
                }
            }
//            UserView.speakInfo("Changes are successful.");
            return true;
        }
//        UserView.speakError("Error occurred. Please try again or check id.");
        return false;
    }       
    
    
    /**
     * Display total Hotel balances including reservations.
     * MANAGEMENT view access function.
     */
    public void displayTotalBalance(){
        double sum=0;
        UserView.listspeak("\n<-- Hotel Total Balance -->");
        for(HashMap.Entry<Integer, ArrayList<Purchase>> entry : cAccounts.entrySet()){
            int id = entry.getKey();  // call pointer for each id number
            ArrayList<Purchase> purchaseList = cAccounts.get(id);  // get pointer to each customer array list                        
            double sumt=0;   
            for(Purchase p : purchaseList){
                sumt+=p.getBalance();
                sum+=p.getBalance();
            }            
            UserView.listspeak("ID "+id+" Customer: "+findCustomerName(id)+" | Balance >> $"+sumt);
        }
        UserView.listspeak("\nHotel Grand Total = $"+sum);   
    }
    

    /**
     * Display single customer account balance (reservations and bookings).
     * CUSTOMER and EMPLOYEE view access.
     * @param id is customer id number
     */
    public void displayCustomerBalance(int id){
        double sum=0;
        if(checkCustomerId(id)){
            ArrayList<Purchase> purchaseList = cAccounts.get(id);
            //UserView.speak("\nCustomer: "+findCustomerName(id));
            for(int i=0; i<purchaseList.size(); i++){
                int j=i+1;  // offset to 'look normal' to customers
                Purchase p = purchaseList.get(i);
                if(p.getReserve()){  // add reservation tag if true
                    UserView.listspeak(j+". <RESERVATION> Room Type: "+p.getRoomType()+" >> Balance: $"+p.getBalance()+ " >> Dates Start: "+p.getDateIn() + " End: "+p.getDateOut());
                }
                else{  // only print non-reservations
                    UserView.listspeak(j+". Room Type: "+p.getRoomType()+" Room: "+p.getRoom()+" >> Balance: $"+p.getBalance()+ " >> Dates Start: "+p.getDateIn() + " End: "+p.getDateOut());
                }                
                sum+=p.getBalance();
            }
            UserView.listspeak("Total Balance: $"+sum);       
        }
        else {
            UserView.speakError("No records available.");            
        }
    }    
    
} // end HotelSystem class
