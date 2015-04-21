package edu.uah.cpe.weathermonkey.app;

/**
 * Created by ninja_boy on 6/29/14.
 */
public class Location {

    private double latit;
    private double longit;
    private String locale;
    private int key;

    public void setLatit(double latit) {
        this.latit = latit;
    }

    public void setLongit(double longit) {
        this.longit = longit;
    }

    public void setLocale(String locale) {
        this.locale = locale;
    }

    public void setKey(int keyy) {this.key = keyy;}

    public Location(String name, double lat, double lng, int ky) {
        // location name & coordinates
        locale = name;
        latit = lat;
        longit = lng;
        key = ky;
    }

    public Location() {
        // blarg
    }

    public int getKey() {return key;}

    public double getLatit() {
        return latit;
    }

    public double getLongit() {
        return longit;
    }

    public String getLocale() {
        return locale;
    }

    public String toString() {

        if (latit == 0 && longit == 0) {
            return (locale);
        } else {
            return (locale+"  Lat = "+latit+"  Long = "+longit);
        }
    }

}
