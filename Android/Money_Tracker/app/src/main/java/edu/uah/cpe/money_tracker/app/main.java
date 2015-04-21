package edu.uah.cpe.money_tracker.app;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.drawable.Drawable;
import android.provider.Settings;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.StringWriter;
import java.io.Writer;
import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


public class main extends Activity {

    private Button btnDel;
    private Button btnDelLast;
    private Button btnAdd;

    private TextView textView2;
    private TextView textView4;

    private ListView listz;

    private ArrayList<Dayz> dayList;
    private ArrayAdapter<Dayz> dayArrayAdapter;

    private static final int MENU_ITEM1=0;
    private static final int MENU_ITEM2=1;

//    boolean dFlg=true;

    private static final String TAG = "TAG_MyActivity";
    private static final String FILENAME = "CURRENT";
    private static final String FILENAME2 = "CURRENT2";
    private static final String FILENAME3 = "CURRENT3";


    public void singleAlert(String title, String message, Drawable icon) {  // single alert dialog; only ok button

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle(title)
                .setIcon(icon)
                .setMessage(message)
                .setCancelable(false)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        //do things
                    }
                });
        AlertDialog alert = builder.create();
        alert.show();
    }


    public String getDateTotal() {  // sum up total of individual dates

//        DecimalFormat f = new DecimalFormat("##000.00");
        DecimalFormat f = new DecimalFormat();
        f.setMinimumFractionDigits(2);

        double sum = 0;
        for (Dayz d : dayList) {
            sum = sum + d.getPurchaseTotal();
        }
        return f.format(sum);
//        return sum;
    }


    // *START* - displayPurchase function
    public void displayPurchase(final Dayz date) {

        final ListView listItems;

        textView4 = (TextView)findViewById(R.id.textView4);

        final Button btnDelLast2;
        final Button btnDel2;

        LayoutInflater la = LayoutInflater.from(getApplicationContext());  // create layout & inflate for purchase arraylist listview
        final View pView = la.inflate(R.layout.nest_list, null);
        listItems = (ListView)pView.findViewById(R.id.nestyList);

//        listItems.setAdapter(new ArrayAdapter<Purchase>(main.this, R.layout.list_layout, R.id.textViewItem, date.getPurchaseList()));  // REFERENCE; using default textview with no Linear Layout
        listItems.setAdapter(new ArrayAdapter<Purchase>(main.this, R.layout.list_layout2, date.getPurchaseList()));

        String tmpTit = date.getDateTitle();
        tmpTit = tmpTit+"\n\tPurchase List"; // "Purchase List" appended to Dayz obj date string

        // *CLICK EVENT* - user sees purchase arraylist in listview; select Add | Done; other[?]
        AlertDialog.Builder alertDialog = new AlertDialog.Builder(main.this);
        alertDialog.setIcon(R.drawable.money_bag_brown);
        alertDialog.setTitle(tmpTit);
        alertDialog.setView(pView);



//        /************  *START* Delete Last | Delete All button listeners - Purchase List  *********************************************

        btnDelLast2 = (Button)pView.findViewById(R.id.btnDelLast2);
        btnDelLast2.setOnClickListener(new View.OnClickListener() {  // delete last entry in Purchase List
            @Override
            public void onClick(View view) {
                if(date.getPurchaseListSize()<=0) {  // notification to user
//                    btnDelLast2.setVisibility(pView.GONE);
                    btnDelLast2.setError("List to Small");
                    btnDelLast2.setEnabled(false);
                }
                date.removeLastPurchase();  // error checking for purchase list size done internally; list will not be clear if size < 0
                ((BaseAdapter)listItems.getAdapter()).notifyDataSetChanged();
            }
        });


        btnDel2 = (Button)pView.findViewById(R.id.btnDel2);
        btnDel2.setOnClickListener(new View.OnClickListener() {  // delete entire Purchase List
            @Override
            public void onClick(View view) {
                if(date.getPurchaseListSize()<=0) {  // notification to user
//                    btnDel2.setVisibility(pView.GONE);
                    btnDel2.setError("List to Small");
                    btnDel2.setEnabled(false);
                }
                date.clearPurchaseList();  // error checking for purchase list size done internally; list will not be clear if size < 0
                ((BaseAdapter)listItems.getAdapter()).notifyDataSetChanged();
            }
        });

//         *************  *END* Delete Last | Delete All button listeners - Purchase List ************************************************/



        // Done Button - update main activity listview with purchase arraylist total; dismiss dialog
        alertDialog.setNegativeButton("Done", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                dialogInterface.dismiss();
//                String dName = new String("su");  // testing
//                String dPrice = new String("9.99");
//                date.addPurchase(dName,dPrice);

//                Toast.makeText(getApplicationContext(), "sum = "+ toString().valueOf(date.getPurchaseTotal()), Toast.LENGTH_SHORT).show();  // testing

                /*  *** KEY NOTES *** List View & Array Adapter explanation[!]
                >  CANNOT use single changing object nested multiple times inside inner functions - java imposes keyword "final"
                >  must create object on the fly [using "new" keyword] whenever object changes and data needs to be updated
                >  must call listview set adapter method once, create new adapter obj on the fly, then invoke listview function getAdapter where ever appropriate inside nested functions
                */

//                (new ArrayAdapter<Purchase>(main.this, R.layout.list_layout, R.id.textViewItem, date.getPurchaseList())).notifyDataSetChanged();  // does not work consistently - REFERENCE ONLY
//                (new ArrayAdapter<Purchase>(main.this, R.layout.list_layout2, date.getPurchaseList())).notifyDataSetChanged();  // does not work consistently - REFERENCE ONLY
                ((BaseAdapter)listItems.getAdapter()).notifyDataSetChanged();  // this is working consistently

                textView4.setText(getDateTotal());  // report ea Dayz obj total to grand total

                dayArrayAdapter.notifyDataSetChanged();  // notify main activity listview of Dayz Purchase list total update
            }
        });

        // Add Button - to override
        alertDialog.setPositiveButton("Add", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                dialogInterface.dismiss();
                // button overridden to keep dialog open for user additions
            }
        });

        AlertDialog alert = alertDialog.create();  // alert dialog for *CLICK EVENT*
        alert.show();

        // Add Positive Button override - pop up alert dialog to add Purchase obj
        alert.getButton(AlertDialog.BUTTON_POSITIVE).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                LayoutInflater li = LayoutInflater.from(getApplicationContext());
                View promptsView = li.inflate(R.layout.alert_dialog_text, null);

                AlertDialog.Builder alertDia = new AlertDialog.Builder(main.this);
                alertDia.setView(promptsView);
                alertDia.setCancelable(false);

                final EditText inTitlez = (EditText)promptsView.findViewById(R.id.ALERT_DIALOG_EDIT_TEXT1);
                final EditText inPricez = (EditText)promptsView.findViewById(R.id.ALERT_DIALOG_EDIT_TEXT2);

                // Cancel Purchase obj add
                alertDia.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        // cancel add; make no changes to purchase arraylist
                    }
                });

                // Add Purchase obj
                alertDia.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {

//                        btnDelLast2.setVisibility(pView.VISIBLE);
//                        btnDel2.setVisibility(pView.VISIBLE);
                        btnDelLast2.setEnabled(true);
                        btnDel2.setEnabled(true);
                        btnDelLast2.setError(null);
                        btnDel2.setError(null);

                        String sTitlez = inTitlez.getText().toString();
                        String sPricez = inPricez.getText().toString();
                        date.addPurchase(sTitlez, sPricez);

                        ((BaseAdapter)listItems.getAdapter()).notifyDataSetChanged();  // working consistently
                    }
                });
                AlertDialog alertz = alertDia.create();
                alertz.show();
            }  // end add button onclick
        });  // end add positive button override function


        // *EDIT* Purchase obj - update purchase arraylist & listview
        listItems.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {

                final int pos = i;  // access arraylist element by inner listener function properly

                LayoutInflater li = LayoutInflater.from(getApplicationContext());
                View promptsView = li.inflate(R.layout.alert_edit_text, null);

                AlertDialog.Builder alertDia = new AlertDialog.Builder(main.this);
                alertDia.setView(promptsView);
                alertDia.setCancelable(false);

                final EditText inTitle = (EditText)promptsView.findViewById(R.id.ALERT_DIALOG_EDIT_TEXT1);
                final EditText inPrice = (EditText)promptsView.findViewById(R.id.ALERT_DIALOG_EDIT_TEXT2);
                inTitle.setText(date.getPurchaseNodeName(pos));
                inPrice.setText(date.getPurchaseNodePrice(pos));

                // Cancel Button - cancel edit Purchase obj
                alertDia.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        // cancel edit; make no changes to purchase arraylist
                    }
                });

                // Ok Button - edit Purchase obj, update purchase arraylist & listview
                alertDia.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        String sTitle = inTitle.getText().toString();
                        String sPrice = inPrice.getText().toString();
                        date.setPurchaseNodeName(sTitle, pos);
                        date.setPurchaseNodePrice(sPrice, pos);

                        ((BaseAdapter)listItems.getAdapter()).notifyDataSetChanged();  // working consistently
                    }
                });
                AlertDialog alertz = alertDia.create();
                alertz.show();
            }  // end onClick for ListView
        });  //  end setOnItemClickListener for ListView
    }  // end displayPurchase function




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView2 = (TextView)findViewById(R.id.textView2);
        textView4 = (TextView)findViewById(R.id.textView4);

        try {  // read from rotate OR onPause (ONLY
            ObjectInputStream objin = null;
            InputStream ins = getApplicationContext().openFileInput(FILENAME3);  // store daylist object in file
            objin = new ObjectInputStream(ins);
            dayList = (ArrayList<Dayz>)objin.readObject();  // cast daylist object from read

//            Toast.makeText(getApplicationContext(), "test obj size "+dayList.size(), Toast.LENGTH_SHORT).show();  // testing
//            if(dayList==null) {  // testing daylist for null
//                Toast.makeText(getApplicationContext(), "test obj null ", Toast.LENGTH_SHORT).show();  // testing
//            }

        }
        catch (Exception e) {
            e.printStackTrace();
        }

        /**********************  REFERENCE   ***************************************************************************
         FileInputStream fin;
         byte[] input;
         String in;
         Toast.makeText(getApplicationContext(), "try block executed", Toast.LENGTH_SHORT).show();

         fin = openFileInput(FILENAME);
         input = new byte[fin.available()];
         while (fin.read(input) != -1) {}
         in = new String(input);
         textView2.setText(in);
         fin.close();
         **********************  REFERENCE   ***************************************************************************/


        textView2.setText(toString().valueOf(dayList.size()));
        textView4.setText(getDateTotal());
        dayArrayAdapter = new ArrayAdapter<Dayz>(main.this, R.layout.list_layout2, dayList);



        //  *SHORT CLICK* - GO TO purchase list
        listz = (ListView)findViewById(R.id.listviewz);
        listz.setAdapter(dayArrayAdapter);
        listz.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                Dayz dia = (Dayz)adapterView.getItemAtPosition(i);
                displayPurchase(dia);  // Dayz arraylist & adapter updated inside this function
            }
        });


        // *LONG CLICK* - EDIT Dayz list - new code[!]
        listz.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> adapterView, View view, int i, long l) {

                final Dayz dia = (Dayz)adapterView.getItemAtPosition(i);
                final int pos = i;

                LayoutInflater li = LayoutInflater.from(getApplicationContext());
                View promptsView = li.inflate(R.layout.alert_course, null);

                AlertDialog.Builder alertDia = new AlertDialog.Builder(main.this);
                alertDia.setView(promptsView);
                alertDia.setCancelable(false);

                final EditText inDay = (EditText)promptsView.findViewById(R.id.ALERT_DIALOG_EDIT_TEXT1);
                String indy = dia.getDateTitle();
                inDay.setText(indy);

                alertDia.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        // user cancels action, close dialog, do nothing
                    }
                });

                alertDia.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        String gDay = inDay.getText().toString();
                        if(gDay.isEmpty()) {gDay = "Blank Date";}
                        dia.setDateTitle(gDay);
                        dayList.set(pos, dia);
                        dayArrayAdapter.notifyDataSetChanged();
                    }
                });
                AlertDialog alertz = alertDia.create();
                alertz.show();
                return true;
            }  // end onItemLongClick listener
        });  // end setOnItemLongClick lister


        // *ADD* new date to main activity list view - new code[!]
        btnAdd = (Button)findViewById(R.id.btnAdd);
        btnAdd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                btnDelLast.setEnabled(true);  // unlock btnDelLast
                btnDel.setEnabled(true);  // unlock btnDel

                dayList.add(new Dayz());
                textView2.setText(Integer.toString(dayList.size()));
                dayArrayAdapter.notifyDataSetChanged();
            }  // end onClick listener btnAdd
        });  // end setOnClickListener btnAdd


        // *DELETE LAST* btnDelLast Action; no warning - new code[!]
        btnDelLast = (Button)findViewById(R.id.btnDelLast);
        btnDelLast.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if(dayList.size()<=0) {
                    Toast.makeText(getApplicationContext(), "List to Small", Toast.LENGTH_SHORT).show();
                    btnDelLast.setEnabled(false);
                }
                else {
                    dayList.remove(dayList.size()-1);
                    textView2.setText(Integer.toString(dayList.size()));
                    textView4.setText(getDateTotal());
                    dayArrayAdapter.notifyDataSetChanged();
                }
            }  // end onClick listener btnDelLast
        });  // end setOnClickListener btnDelLast


        // *DELETE ALL* btnDel Action; user notification - "Delete All, sure?" - new code[!]
        btnDel = (Button)findViewById(R.id.btnDel);
        btnDel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                LayoutInflater li = LayoutInflater.from(getApplicationContext());
                final View promptsView = li.inflate(R.layout.delete_layout, null);

                final AlertDialog.Builder alertDia = new AlertDialog.Builder(main.this);
                alertDia.setView(promptsView);
                alertDia.setCancelable(false);

                alertDia.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        // user closes dialog, do nothing
                        dialogInterface.dismiss();
                    }  // end onClick setNegativeButton
                });  // end setNegativeButton config

                alertDia.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {

                        if(dayList.size() <= 0) {
                            Toast.makeText(getApplicationContext(), "List to Small", Toast.LENGTH_SHORT).show();
                            btnDel.setEnabled(false);
                            dialogInterface.dismiss();
                        }
                        else {
                            dayList.clear();
                            textView2.setText(Integer.toString(dayList.size()));
                            textView4.setText(getDateTotal());
                            dayArrayAdapter.notifyDataSetChanged();

                            ObjectOutputStream objout = null;
                            try {  // write empty daylist obj to file
                                OutputStream out = getApplicationContext().openFileOutput(FILENAME3, Context.MODE_PRIVATE);  // write serialized obj to file
                                objout = new ObjectOutputStream(out);
                                objout.writeObject(dayList);
                                objout.close();
                            }
                            catch(Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }  // end onClick positive button alert dialog
                });  // end setPositiveButton function alert dialog
                final AlertDialog alertz = alertDia.create();
                alertz.show();  // end alert dialog - "Delete All, sure?"
            }  // end onClick listener btnDel
        });  // end setOnClickListener btnDel

    }  // end onCreate


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);

        menu.add(Menu.NONE, MENU_ITEM1, Menu.NONE, "Help");
        menu.add(Menu.NONE, MENU_ITEM2, Menu.NONE, "About");
        return super.onCreateOptionsMenu(menu);
    }  // end onCreateOptionsMenu


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Drawable draw;
        String ts;
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
//        int id = item.getItemId();
//        if (id == R.id.action_settings) {
//            return true;
//        }

        switch (item.getItemId()) {
            case MENU_ITEM1:
                draw = getResources().getDrawable(R.drawable.help_circle_su);
                ts = getResources().getString(R.string.help_alert);
                singleAlert("Help", ts, draw);
                break;
            case MENU_ITEM2:
                draw = getResources().getDrawable(R.drawable.duck_inquiry_su);
                ts = getResources().getString(R.string.about_alert);
                singleAlert("About", ts, draw);
                break;
            case R.id.action_settings:
                draw = getResources().getDrawable(R.drawable.settings_green_su);
                ts = getResources().getString(R.string.settings_alert);
                singleAlert("Settings", ts, draw);
                break;
        }  // end switch
        return super.onOptionsItemSelected(item);
    }  // end onOptionsItemSelected



    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {  // save user data on screen rotate
        super.onSaveInstanceState(savedInstanceState);

        ObjectOutputStream objout = null;
        try {

            OutputStream out = getApplicationContext().openFileOutput(FILENAME3, Context.MODE_PRIVATE);  // write serialized obj to file
            objout = new ObjectOutputStream(out);
            objout.writeObject(dayList);
            objout.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        /**********************  REFERENCE   ***************************************************************************
        textView2 = (TextView)findViewById(R.id.textView2);
        textView4 = (TextView)findViewById(R.id.textView4);
        savedInstanceState.putString("ITEM_QTY", textView2.getText().toString());
        savedInstanceState.putString("GND_TTL", textView4.getText().toString());

        savedInstanceState.putSerializable("DAYLIST_OBJ", dayList);  // save main activity arraylist object
        **********************  REFERENCE   ***************************************************************************/

    }  // end onSaveInstanceState



    @Override
    public void onPause() {  // save user data on activity destruction
        super.onPause();

//        textView2 = (TextView)findViewById(R.id.textView2);

        ObjectOutputStream objout = null;
        try {

            /**********************  REFERENCE   ***************************************************************************
            String tString = textView2.getText().toString();
            FileOutputStream fos;
            fos = openFileOutput(FILENAME, Context.MODE_PRIVATE);
            fos.write(tString.getBytes());  // write string to file
            fos.close();
            **********************  REFERENCE   ***************************************************************************/

            OutputStream out = getApplicationContext().openFileOutput(FILENAME3, Context.MODE_PRIVATE);  // write serialized obj to file
            objout = new ObjectOutputStream(out);
            objout.writeObject(dayList);
//            Log.d(TAG, "debugging daylist read = "+dayList);
            objout.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }  // end onPause


}  // end Main Activity