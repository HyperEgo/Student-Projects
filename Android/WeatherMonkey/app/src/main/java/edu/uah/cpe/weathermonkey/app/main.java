package edu.uah.cpe.weathermonkey.app;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.media.Image;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewConfiguration;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.List;


public class main extends Activity {

    TextView txtStart;
    EditText txtCallsign;
    LinearLayout pLayout;
    Spinner spin;
    RadioGroup radgrp;
    RadioButton rad1, rad2;
    int intFlag = 0;
    Boolean servFlag = false;
    Location locz;


    private void setLoc(Location locale) {
        locz = locale;
    }


    private boolean isEmpty(EditText etText) {  // return true if empty
        return etText.getText().toString().trim().length() == 0;
    }


    // soft keyboard input manager
    public static void hideSoftKeyboard(Activity act) {
        InputMethodManager inputMethodManager = (InputMethodManager)  act.getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(act.getCurrentFocus().getWindowToken(), 0);
    }


    // listener for enter button - main activity background
    public void setupUI(View view) {
        final TextView txtLocale = (TextView)findViewById(R.id.txtLocale);
        //Set up touch listener for non-text box views to hide keyboard.
        if(!(view instanceof EditText)) {
            view.setOnTouchListener(new View.OnTouchListener() {
                public boolean onTouch(View v, MotionEvent event) {
                    txtLocale.setFocusable(true);
                    txtLocale.setFocusableInTouchMode(true);  // add this line
                    txtLocale.requestFocus();
                    hideSoftKeyboard(main.this);
                    return false;
                }
            });
        }
        //If a layout container, iterate over children and seed recursion.
        if (view instanceof ViewGroup) {
            for (int i = 0; i < ((ViewGroup) view).getChildCount(); i++) {
                View innerView = ((ViewGroup) view).getChildAt(i);
                setupUI(innerView);
            }
        }
    }


    // listener for enter button - soft keyboard
    public void addKeyListener() {
        final TextView txtLocale = (TextView)findViewById(R.id.txtLocale);

        // add a keylistener to keep track user input
        EditText txtCallsign = (EditText)findViewById(R.id.txtCallsign);
        txtCallsign.setOnKeyListener(new View.OnKeyListener() {
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                // if keydown and "enter" is pressed
                if ((event.getAction() == KeyEvent.ACTION_DOWN)
                        && (keyCode == KeyEvent.KEYCODE_ENTER)) {
                    // display a floating message
//                    Toast.makeText(main.this, "Enter key pressed", Toast.LENGTH_SHORT).show();
                    txtLocale.setFocusable(true);
                    txtLocale.setFocusableInTouchMode(true);  // add this line
                    txtLocale.requestFocus();
                    hideSoftKeyboard(main.this);
                    return true;
                }
                return false;
            }
        });
    }


    public void addItemsToSpinner() {

//        List<String> list = new ArrayList<String>();
//        list.add("hi there");
//        list.add("howdy");
//        list.add("welcome");
//        ArrayAdapter<String> dataAdapter = new ArrayAdapter<String>(this,
//                android.R.layout.simple_spinner_item, list);

        List<Location> list = new ArrayList<Location>();
        list.add(new Location("Huntsville, AL", 34.73, -86.58, 1));
        list.add(new Location("Birmingham, AL", 33.52, -86.81, 2));
        list.add(new Location("Mobile, AL", 30.69, -88.04, 3));
        list.add(new Location("Nashville, TN", 36.16, -86.78, 4));
        list.add(new Location("Seattle, WA", 47.60, -122.33, 5));
        list.add(new Location("Spokane, WA", 47.65, -117.42, 6));
        list.add(new Location("Las Vegas, NV", 36.12, -115.17, 7));
        list.add(new Location("other", 0, 0, 99));

        ArrayAdapter<Location> dataAdapter = new ArrayAdapter<Location>(this, R.layout.spinner_layout, list);

        dataAdapter.setDropDownViewResource(R.layout.spinner_drop_down);
        spin.setAdapter(dataAdapter);
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Location locky = new Location("Huntsville, AL", 34.73, -86.58, 1);  // initialize Location
        setLoc(locky);

        txtStart = (TextView)findViewById(R.id.txtStart);
        txtStart.setFocusable(true);
        txtStart.setFocusableInTouchMode(true);  // add this line
        txtStart.requestFocus();

        radgrp = (RadioGroup)findViewById(R.id.rdbGp1);
        rad1 = (RadioButton)findViewById(R.id.rdb1);
        rad2 = (RadioButton)findViewById(R.id.rdb2);
        radgrp.check(rad1.getId());

        spin = (Spinner)findViewById(R.id.spinLocale);
        pLayout = (LinearLayout)findViewById(R.id.parent);
        setupUI(pLayout);
        addKeyListener();
        addItemsToSpinner();


        spin.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                if(intFlag>0) {
                    final Location loct = (Location)adapterView.getItemAtPosition(i);  // cast spinner obj to Location obj
                    if (loct.getKey()==99) {  // "other" option selected

                        LayoutInflater li = LayoutInflater.from(getApplicationContext());
                        View promptsView = li.inflate(R.layout.alert_map, null);

                        AlertDialog.Builder alertDia = new AlertDialog.Builder(main.this);
                        alertDia.setView(promptsView);
                        alertDia.setCancelable(false);

                        //get onclick for image view
                        final EditText inLat = (EditText)promptsView.findViewById(R.id.txtLat);
                        final EditText inLong = (EditText)promptsView.findViewById(R.id.txtLong);
                        ImageView img = (ImageView)promptsView.findViewById(R.id.imgMap);
                        img.setFocusable(true);
                        img.setFocusableInTouchMode(true);  // add this line
                        img.requestFocus();

                        img.setOnTouchListener(new View.OnTouchListener() {
                            @Override
                            public boolean onTouch(View view, MotionEvent motionEvent) {
                                final int x = (int)motionEvent.getX();
                                final int y = (int)motionEvent.getY();

                                // get x | y cursor position (motion event), do transformations to lat, long *HERE*
                                // hardcoded map picture dimensions
                                LLfromXY util = new LLfromXY();
                                util.setMapValues(420, 760, 50, 24, -126, -66);
                                double userLat = util.getLatitude(y);
                                double userLon = util.getLongitude(x);

                                inLat.setText(String.valueOf(userLat));
                                inLong.setText(String.valueOf(userLon));
                                return false;
                            }
                        });


                        // set negative button - spinner alert
                        alertDia.setNegativeButton("Close", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                Location dummy = new Location();
                                String str2 = "34.73";
                                String str3 = "-86.58";
                                dummy.setLocale("Blank");
                                dummy.setLatit(Double.parseDouble(str2));
                                dummy.setLongit(Double.parseDouble(str3));
                                setLoc(dummy);
                                Toast.makeText(main.this, dummy.toString(), Toast.LENGTH_SHORT).show();
//                                dialogInterface.dismiss();
                            }
                        });

                        // set positive button - spinner alert
                        alertDia.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                Location dummy = new Location();
                                String str2 = inLat.getText().toString();
                                String str3 = inLong.getText().toString();

                                if(str2.isEmpty() || str3.isEmpty()) {
                                    str2 = "34.73";
                                    str3 = "-86.58";
                                    dummy.setLocale("Blank");
                                }
                                else {
                                    // error checking HERE if need be;
                                    // set str2, str3 based on some bounds

                                    dummy.setLocale("Custom");
                                }

                                dummy.setLatit(Double.parseDouble(str2));
                                dummy.setLongit(Double.parseDouble(str3));
                                setLoc(dummy);
                                Toast.makeText(main.this, dummy.toString(), Toast.LENGTH_SHORT).show();
                            }
                        });

                        AlertDialog alertz = alertDia.create();
                        alertz.show();

                    }  // end other option selected

                    else {
                        setLoc(loct);
                    }

                }
                intFlag = intFlag+1;
                // Toast.makeText(adapterView.getContext(),"Locale : " + adapterView.getItemAtPosition(i).toString(),Toast.LENGTH_SHORT).show();
            }
            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });  // end spinner setOnItemSelected function

    }  // end onCreate


    // ok Button send info to second activity
    public void onClick(View view){ // get values from ui: callsign, lat | long & server choice

//        Toast.makeText(main.this,"OK key pressed", Toast.LENGTH_SHORT).show();
        String str;
        double llat, llong;

        txtCallsign = (EditText)findViewById(R.id.txtCallsign);
        if(isEmpty(txtCallsign)) {str = "K9S3X";}
        else {str = txtCallsign.getText().toString();}

        llat = locz.getLatit();
        llong = locz.getLongit();
//        Toast.makeText(main.this, Double.toString(llat),Toast.LENGTH_SHORT).show();
//        Toast.makeText(main.this, Double.toString(llong),Toast.LENGTH_SHORT).show();

        rad1 = (RadioButton)findViewById(R.id.rdb1);
        if(rad1.isChecked()){servFlag=true;}
        else {servFlag=false;}
//        Toast.makeText(main.this,Boolean.toString(servFlag),Toast.LENGTH_SHORT).show();

        Intent i = new Intent(getApplicationContext(), SecondActivity.class);
        i.putExtra("callsign", str);

        i.putExtra("llat", llat);
        i.putExtra("llong", llong);

        i.putExtra("servFlag", servFlag);
        startActivity(i);  // start second activity
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
