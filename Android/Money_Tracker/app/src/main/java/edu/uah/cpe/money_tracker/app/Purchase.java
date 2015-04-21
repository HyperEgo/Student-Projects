package edu.uah.cpe.money_tracker.app;

import android.os.Parcel;
import android.os.Parcelable;

import java.io.Serializable;
import java.text.DecimalFormat;

/**
 * Created by ninja_boy on 11/18/14.
 */
public class Purchase implements Serializable{

    private String title;
    private double price;


    public Purchase() {

        // blarg
    }


    //  ******* BEGIN Required Parcelable functions  ********
//    public int describeContents() {
//        return 0;
//    }
//
//
//    private Purchase(Parcel in) {
//
//        this();
//
//        readFromParcel(in);
//    }
//
//
//    public void readFromParcel(Parcel in) {
//
//        title = in.readString();
//        price = in.readDouble();
//    }
//
//
//    public void writeToParcel(Parcel out, int flags) {
//
//        out.writeString(title);
//        out.writeDouble(price);
//    }
//
//
//    public static final Parcelable.Creator<Purchase> CREATOR = new Parcelable.Creator<Purchase>() {
//        public Purchase createFromParcel(Parcel in) {
//            return new Purchase(in);
//        }
//
//        public Purchase[] newArray(int size) {
//            return new Purchase[size];
//        }
//    };
    //  ******** END Required Parcelable functions  ********


    public void setTitle(String tit) {title = tit;}

    public void setPrice(double pr) {price = pr;}

    public String getTitle() {return title;}

    public double getPrice() {return price;}

    public String toString() {

        DecimalFormat f = new DecimalFormat("##0.00");
        f.setMinimumFractionDigits(2);
        return (title+"  =  $ "+f.format(price));
    }

}  // end Purchase class
