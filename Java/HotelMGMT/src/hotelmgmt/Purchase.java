/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hotelmgmt;
import java.io.Serializable;
import org.joda.time.LocalDate;
/**
 * A Customer Purchase
 * @author fc
 */
public class Purchase implements Serializable{
    
    private double balance;
    private String roomType;
    private boolean reserve;
    
    // internal use variables
    private int room;    
    private LocalDate in; 
    private LocalDate out;
    
    /**
     * Creates an empty purchase
     */
    public Purchase(){
        reserve=false;  // default to false        
    }
    
    /**
     * Get the purchase balance
     * @return the purchase balance
     */
    public double getBalance(){return balance;}
    
    /**
     * Get the room type of the purchase
     * @return the room type of the purchase
     */
    public String getRoomType(){return roomType;}
            
    /**
     * Get flag status of purchase; reserve or normal check in
     * @return boolean true for room reservation, false otherwise
     */
    public boolean getReserve(){return reserve;}
    
    /**
     * Get the room number of purchase
     * @return room number
     */
    public int getRoom(){return room;}
    
    public LocalDate getDateIn(){return in;}
    
    public LocalDate getDateOut(){return out;}   
    
    /**
     * Set the purchase balance
     * @param b the purchase balance
     */
    public void setBalance(double b){balance = b;}    
    
    /**
     * Set the purchase room type
     * @param r the room type
     */
    public void setRoomType(String r){roomType = r;}
        
    /**
     * Set flag to indicate whether purchase is a reservation
     * @param flag set boolean for reservation
     */
    public void setReserve(boolean flag){reserve=flag;}
    
    /**
     * Set room number for purchase
     * @param r is room number
     */
    public void setRoom(int r){room=r;}
    
    /**
     * Set the check in date
     * @param a the check in date
     */
    public void setDateIn(LocalDate a){in=a;}
    
    /**
     * Set the check out date
     * @param b the check out date
     */
    public void setDateOut(LocalDate b){out=b;}    
    
}  // end Purchase class
