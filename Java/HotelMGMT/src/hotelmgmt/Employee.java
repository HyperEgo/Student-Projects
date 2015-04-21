/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hotelmgmt;
import java.io.Serializable;
/**
 * An Employee at a hotel
 * @author fc
 */

public class Employee implements Serializable{
    
    private String name;
    private int id;
    private double pay;
    
    /**
     * Creates an empty employee
     */
    public Employee(){
        // jazzy constructor        
    }    
    
    /**
     * Creates an employee
     * @param s the employee's name
     * @param d the employee's id
     * @param p the employee's pay
     */
    public Employee(String s, int d, double p){
        name = s;
        id = d;
        pay = p;
    }
    
    /**
     * Get the employee's name
     * @return the name of the employee
     */
    public String getEmpName(){
        return name;        
    }
    
    /**
     * Get the employee's id
     * @return the id of the employee
     */
    public int getEmpId() {
        return id;        
    }
    
    /**
     * Get the employee's pay
     * @return the pay of the employee
     */
    public double getEmpPay(){
        return pay;        
    }
    
    /**
     * Set the name of the employee
     * @param g the employee's new name
     */
    public void setEmpName(String g){
        name = g;
    }
    
    /**
     * Set the id of the employee
     * @param d the new id of the employee 
     */
    public void setId(int d){
        id = d;
    }
    
    /**
     * Set the employee's pay
     * @param d the new pay of the employee
     */
    public void setPay(double d){
        pay = d;
    }
    
}
