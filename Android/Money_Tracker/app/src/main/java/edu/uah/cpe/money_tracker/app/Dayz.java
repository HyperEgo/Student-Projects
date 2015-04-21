package edu.uah.cpe.money_tracker.app;

import android.content.Context;
import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.io.Serializable;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

/**
 * Created by ninja_boy on 11/19/14.
 */
public class Dayz implements Serializable{  // implement parcelable to save data on screen rotate


    private String dateTitle;
    private ArrayList<Purchase> purchaseList;


    public Dayz() {

        purchaseList = new ArrayList<Purchase>();  // initialize arraylist

        // get date & time
        Date now = new Date();
//        SimpleDateFormat simpleFormatter = new SimpleDateFormat("E hh:mm:ss a 'on' yyyy.MM.dd zzz");  // SimpleDateFormat
        SimpleDateFormat simpleFormatter = new SimpleDateFormat("E hh:mm:ss a '['MM-dd-yyyy']'");  // SimpleDateFormat
        String tmpDate = simpleFormatter.format(now);
        // check date obj for validity
        if (this.isDateValid(tmpDate)) {dateTitle = tmpDate;}
        else {dateTitle = "Date Not Found";}
    }


    //  ******* BEGIN Required Parcelable functions  ********
//    public int describeContents() {  // req parcelable; works for screen rotate
//        return 0;
//    }
//
//
//    private Dayz(Parcel in) {  // req parcelable; works for screen rotate
//
//        this();
//
////        purchaseList = new ArrayList<Purchase>();
//        readFromParcel(in);
//
////        dateTitle = in.readString();
////        purchaseList = in.readArrayList(Purchase.class.getClassLoader());  // not working; not reading arraylist correctly
//    }
//
//
//    public void readFromParcel(Parcel in) {
//
//        dateTitle = in.readString();
//
////        if (purchaseList == null) {
////
////            purchaseList = new ArrayList<Purchase>();
////        }
//        in.readTypedList(purchaseList, Purchase.CREATOR);
//    }
//
//
//    public void writeToParcel(Parcel out, int flags) {  // req parcelable; works for screen rotate
//
//        out.writeString(dateTitle);
//        out.writeTypedList(purchaseList);
//    }
//
//
//    public static final Parcelable.Creator<Dayz> CREATOR = new Parcelable.Creator<Dayz>() {  // req parcelable; works for screen rotate
//        public Dayz createFromParcel(Parcel in) {
//            return new Dayz(in);
//        }
//
//        public Dayz[] newArray(int size) {
//            return new Dayz[size];
//        }
//    };
    //  ******** END Required Parcelable functions  ********


    public boolean isDateValid(String str) {  // test for valid date string
        if (str==null) {return false;}
        else {return true;}
    }

    public void setDateTitle(String dat) {dateTitle = dat;}

    public String getDateTitle() {return dateTitle;}

    public int getPurchaseListSize() {return purchaseList.size();}  // maybe not?

    public ArrayList<Purchase> getPurchaseList() {return purchaseList;}

    public String getPurchaseNodeName(int z) {

        Purchase pu = this.purchaseList.get(z);
        return pu.getTitle();
    }


    public String getPurchaseNodePrice(int y) {

        DecimalFormat e = new DecimalFormat("##00.00");
        e.setMinimumFractionDigits(2);

        Purchase pu = this.purchaseList.get(y);
        return e.format(pu.getPrice());  // show correct number format in editor window
    }


    public void setPurchaseNodeName(String s, int z) {

        Purchase pu = this.purchaseList.get(z);
        if(s.isEmpty()) {s = "Blank Purchase";}  // account for empty entry
        pu.setTitle(s);
        this.purchaseList.set(z, pu);
    }


    public void setPurchaseNodePrice(String d, int y) {

        Purchase pu = this.purchaseList.get(y);
        if(d.isEmpty()) {d = "0.00";}  // account for empty entry
        pu.setPrice(Double.parseDouble(d));
        this.purchaseList.set(y, pu);
    }


    public void addPurchase(String name, String price){

        Purchase p = new Purchase();
        if(name.isEmpty()) {name = "Blank Purchase";}  // account for empty entry
        if(price.isEmpty()) {price = "0.00";}  // account for empty entry
        p.setTitle(name);
        p.setPrice(Double.parseDouble(price));
        this.purchaseList.add(p);
    }


    public void removeLastPurchase() {

        if(purchaseList.size()>0) {  // check list size before proceeding
            this.purchaseList.remove(purchaseList.size()-1);
        }
    }


    public void clearPurchaseList() {

        if(purchaseList.size()>0) {  // check list size before proceeding
            purchaseList.clear();
        }
    }


    public double getPurchaseTotal() {  // function working correctly

        double sum = 0;
        for (Purchase p : purchaseList) {
            sum = sum + p.getPrice();
        }
        return sum;
    }


    public String toString() {

        DecimalFormat f = new DecimalFormat("##00.00");
        f.setMinimumFractionDigits(2);
        return (dateTitle+"  Total =  $ "+f.format(getPurchaseTotal()));
    }

}  // end Dayz class
