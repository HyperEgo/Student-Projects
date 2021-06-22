/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hotelmgmt;
import java.io.Serializable;
/**
 * A Customer at a hotel
 * @author fc
 */
public class Customer implements Serializable{
    
    private String name;
    private int id;
    private String payment;
    
   /**
    * Creates an empty customer
    */
    public Customer(){
        // jazzy constructor
    }
    
    /**
     * Creates a customer
     * @param s the customer's name
     * @param d the customer's id
     * @param p the customer's payment
     */
    public Customer(String s, int d, String p){
        name = s;
        id = d;
        payment = p;
    }
    
    /**
     * Get the customer's name
     * @return the customer's name
     */
    public String getName(){
        return name;
    }
    
    /**
     * Get the customer's id
     * @return the customer's id
     */
    public int getId(){
        return id;
    }
    
    /**
     * Get the customer's payment
     * @return the customer's payment
     */
    public String getPayment(){
        return payment;
    }   
       
    /**
     * Set the customer's name
     * @param a the customer's new name
     */
    public void setName(String a){
        name = a;
    }
    
    /**
     * Set the customer's id
     * @param c the customer's new id
     */
    public void setId(int c){
        id = c;
    }
    
    /**
     * Set the customer's payment
     * @param p the customer's new payment
     */
    public void setPayment(String p){
        payment = p;
    }
    
}

