package edu.uah.cpe.weathermonkey.app;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.concurrent.TimeUnit;


public class SecondActivity extends Activity {

    String str1, outPrep;
    double llat, llong;
    boolean sFlag;

    MyClientTask mt;
    ProgressBar progress;

    public static final int APRS_PORT = 14580;
    public static final int  UAH_PORT = 20154;
    public static final String RADIUS = "700";
    public static final String APRS_SERVER = "rotate.aprs.net";
    public static final String UAH_SERVER = "blackhawk.eng.uah.edu";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_second);


        str1 = getIntent().getStringExtra("callsign");
        llat = getIntent().getDoubleExtra("llat",0);
        llong = getIntent().getDoubleExtra("llong",0);
        sFlag = getIntent().getBooleanExtra("servFlag",false);

        // build string to for AsyncTask
//        String outMsg = "user Skyfall pass -1 vers testsoftware 1.0_05 filter r/34/-86/700";
        outPrep = "user "+str1+" pass -1 vers testsoftware 1.0_05 filter r/";
        outPrep = outPrep+llat+"/"+llong+"/"+RADIUS+"\n";

        progress = (ProgressBar)findViewById(R.id.progBar);
        progress.setVisibility(View.INVISIBLE);

    }



    void clearFields() {
        // clear all TextViews

        TextView txtReport = (TextView)findViewById(R.id.txtReport);
        TextView txtTime = (TextView)findViewById(R.id.txtTime);
        TextView txtTemp = (TextView)findViewById(R.id.txtTemp);
        TextView txtWGust = (TextView)findViewById(R.id.txtWGust);
        TextView txtWSpeed = (TextView)findViewById(R.id.txtWSpeed);
        TextView txtWDirection = (TextView)findViewById(R.id.txtWDirection);
        TextView txtBPressure = (TextView)findViewById(R.id.txtBPressure);
        TextView txtHumidity = (TextView)findViewById(R.id.txtHumidity);
        TextView txtRainHour = (TextView)findViewById(R.id.txtRainHour);
        TextView txtRainNight = (TextView)findViewById(R.id.txtRainNight);

        txtReport.setText("");
        txtTime.setText("");
        txtTemp.setText("");
        txtWGust.setText("");
        txtWSpeed.setText("");
        txtWDirection.setText("");
        txtBPressure.setText("");
        txtHumidity.setText("");
        txtRainHour.setText("");
        txtRainNight.setText("");

    }



    void parseUAH(String pstr) {
        // parse UAH receive string

        TextView txtReport = (TextView)findViewById(R.id.txtReport);
        TextView txtTime = (TextView)findViewById(R.id.txtTime);
        TextView txtTemp = (TextView)findViewById(R.id.txtTemp);
        TextView txtWGust = (TextView)findViewById(R.id.txtWGust);
        TextView txtWSpeed = (TextView)findViewById(R.id.txtWSpeed);
        TextView txtWDirection = (TextView)findViewById(R.id.txtWDirection);
        TextView txtBPressure = (TextView)findViewById(R.id.txtBPressure);
        TextView txtHumidity = (TextView)findViewById(R.id.txtHumidity);
        TextView txtRainHour = (TextView)findViewById(R.id.txtRainHour);
        TextView txtRainNight = (TextView)findViewById(R.id.txtRainNight);

        int i;
        String dum, yum;

        try {

            if(pstr.contains(":@")) {
                i=pstr.indexOf(":@");
                txtReport.setText(pstr.substring(1,8));

                i=pstr.indexOf("z");
                i=i+6;
                dum = pstr.substring(i,i+3);
                i=i+10;
                yum = pstr.substring(i,i+3);
                txtTime.setText(dum+"  |  "+yum);

//                // getting erroneous values
//                if(pstr.contains("c")) {
//                    i=pstr.indexOf("c");
//                    dum = pstr.substring(i+1,i+4);
//                    txtWDirection.setText(dum+" degrees");
//                }
//
//                // getting erroneous values
//                if(pstr.contains("s")) {
//                i=pstr.indexOf("s");
//                dum = pstr.substring(i+1,i+4);
//                txtWSpeed.setText(dum+" mph");
//                }


                if(pstr.contains("g")) {
                    i=pstr.indexOf("g");
                    dum = pstr.substring(i+1,i+4);
                    txtWGust.setText(dum+" mph");
                }

                if(pstr.contains("t")) {
                    i=pstr.indexOf("t");
                    dum = pstr.substring(i+1,i+4);
                    txtTemp.setText(dum+" F");
                }

                if(pstr.contains("b")) {
                    i=pstr.indexOf("b");
                    dum = pstr.substring(i+1,i+6);
                    txtBPressure.setText(dum+" millibars / hPascal");
                }

                if(pstr.contains("h")) {
                    i=pstr.indexOf("h");
                    dum = pstr.substring(i+1,i+3);
                    txtHumidity.setText(dum+" %");
                }

                if(pstr.contains("r")) {
                    i=pstr.indexOf("r");
                    dum = pstr.substring(i+1,i+4);
                    txtRainHour.setText(dum+"  1/100 inch");
                }

                if(pstr.contains("p")) {
                    i=pstr.indexOf("p");
                    dum = pstr.substring(i+1,i+4);
                    txtRainNight.setText(dum+"  1/100 inch");
                }

            }

        }
        catch (StringIndexOutOfBoundsException e) {
            e.printStackTrace();
            Toast.makeText(SecondActivity.this, "String input oddity - Please Restart", Toast.LENGTH_SHORT).show();
        }

    }


    void parseAPRS(String pstr) {
        // parse APRS net receive string

        TextView txtReport = (TextView)findViewById(R.id.txtReport);
        TextView txtTime = (TextView)findViewById(R.id.txtTime);
        TextView txtTemp = (TextView)findViewById(R.id.txtTemp);
        TextView txtWGust = (TextView)findViewById(R.id.txtWGust);
        TextView txtWSpeed = (TextView)findViewById(R.id.txtWSpeed);
        TextView txtWDirection = (TextView)findViewById(R.id.txtWDirection);
        TextView txtBPressure = (TextView)findViewById(R.id.txtBPressure);
        TextView txtHumidity = (TextView)findViewById(R.id.txtHumidity);
        TextView txtRainHour = (TextView)findViewById(R.id.txtRainHour);
        TextView txtRainNight = (TextView)findViewById(R.id.txtRainNight);

        int i;
        String dum;

        try {

            if(pstr.contains(":_")) {
                i=pstr.indexOf(":_");
                txtReport.setText(pstr.substring(1, 8));

                i=i+2;
                dum = pstr.substring(i, i + 8);
                dum = "Date  "+dum.substring(0,2)+"/"+dum.substring(2,4)+"  Time  "+dum.substring(4,6)+":"+dum.substring(6,8)+" (24hr)";
                txtTime.setText(dum);

                if(pstr.contains("c")) {
                    i=pstr.indexOf("c");
                    dum = pstr.substring(i + 1, i + 4);
                    txtWDirection.setText(dum + " degrees");
                }

                if(pstr.contains("s")) {
                    i=pstr.indexOf("s");
                    dum = pstr.substring(i + 1, i + 4);
                    txtWSpeed.setText(dum + " mph");
                }

                if(pstr.contains("g")) {
                    i=pstr.indexOf("g");
                    dum = pstr.substring(i + 1, i + 4);
                    txtWGust.setText(dum + " mph");
                }

                if(pstr.contains("t")) {
                    i=pstr.indexOf("t");
                    dum = pstr.substring(i + 1, i + 4);
                    txtTemp.setText(dum + " F");
                }

                if(pstr.contains("b")) {
                    i=pstr.indexOf("b");
                    dum = pstr.substring(i + 1, i + 6);
                    txtBPressure.setText(dum + " millibars / hPascal");
                }

                if(pstr.contains("h")) {
                    i=pstr.indexOf("h");
                    dum = pstr.substring(i + 1, i + 3);
                    txtHumidity.setText(dum + " %");
                }

                if(pstr.contains("r")) {
                    i=pstr.indexOf("r");
                    dum = pstr.substring(i + 1, i + 4);
                    txtRainHour.setText(dum + "  1/100 inch");
                }

                if(pstr.contains("p")) {
                    i=pstr.indexOf("p");
                    dum = pstr.substring(i + 1, i + 4);
                    txtRainNight.setText(dum + "  1/100 inch");
                }

            }

        }
        catch (StringIndexOutOfBoundsException e){
            e.printStackTrace();
            Toast.makeText(SecondActivity.this, "String input oddity - Please Restart", Toast.LENGTH_SHORT).show();
        }

    }


    public void onConnect (View v) {
        Button btnConnect = (Button)findViewById(R.id.btnConnect);
        switch (v.getId()) {
            case R.id.btnConnect:
                clearFields();
                mt = new MyClientTask();
                mt.setInputString(outPrep);
//                Toast.makeText(SecondActivity.this, outPrep, Toast.LENGTH_SHORT).show();
                if(sFlag){
                    mt.setServer(APRS_SERVER);
                    mt.setPort(APRS_PORT);
                }
                else {
                    mt.setServer(UAH_SERVER);
                    mt.setPort(UAH_PORT);
                }
                mt.execute();
                btnConnect.setEnabled(false);
                progress.setVisibility(View.VISIBLE);
                break;
            case R.id.btnCancel:
                cancelTask();
                btnConnect.setEnabled(true);
                progress.setVisibility(View.INVISIBLE);
                break;
            case R.id.btnBack:
                cancelTask();
                Intent i = new Intent(getApplicationContext(), main.class);
                startActivity(i);
                progress.setVisibility(View.INVISIBLE);
            default:
                break;
        }
    }


    private void cancelTask() {
        if(mt==null)
            return;
        Log.d("LOG_TAG", "cancel result" + mt.cancel(false));
    }


    public class MyClientTask extends AsyncTask<Void, String, Void> {

        String dstAddress;
        int dstPort;
        String response, send;

        Socket socket;
        BufferedReader in;
        BufferedWriter out;


        void setPort(int port){dstPort = port;}

        void setServer(String addr){dstAddress = addr;}

        void setInputString(String str){send = str;}


        @Override
        protected void onPreExecute() {
            // blarg

        }

        @Override
        protected Void doInBackground(Void... parms) {

            try {
                socket = new Socket(dstAddress, dstPort);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

//                String outMsg = "user Skyfall pass -1 vers testsoftware 1.0_05 filter r/34/-86/700";
//                outMsg=outMsg+"\n";
//                out.write(outMsg);

                out.write(send);
                out.flush();

                while ((response = in.readLine()) != null) {
//                    TimeUnit.SECONDS.sleep(1);
                    TimeUnit.MILLISECONDS.sleep(555);
                    if(isCancelled()) return null;
                    Log.d("Received String", response);
//                    Log.d("LOG_TAG", "is Cancelled : " + isCancelled());
                    publishProgress(response);
                }

                socket.close();  // close connection

            }
            catch (UnknownHostException e) {
                Log.d("LOG_TAG","Unknown Host");
                e.printStackTrace();
            }
            catch (IOException e) {
                Log.d("LOG_TAG","I/O Exception");
                e.printStackTrace();
            }
            catch (InterruptedException e) {
                Log.d("LOG_TAG","Interrupted");
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            Toast.makeText(SecondActivity.this, "Background Thread End", Toast.LENGTH_SHORT).show();
            Log.d("LOG_TAG", "End - close");
            progress.setVisibility(View.INVISIBLE);
            super.onPostExecute(result);
        }


        @Override
        protected void onProgressUpdate(String... progress) {
            if(sFlag){parseAPRS(Arrays.toString(progress));}
            else {parseUAH(Arrays.toString(progress));}
            super.onProgressUpdate(progress);
//            Toast.makeText(SecondActivity.this, Arrays.toString(progress), Toast.LENGTH_SHORT).show();
        }


        @Override
        protected void onCancelled(Void aVoid) {
//            super.onCancelled(aVoid);
            progress.setVisibility(View.INVISIBLE);
//            Toast.makeText(SecondActivity.this, "Connection End", Toast.LENGTH_SHORT).show();
            Log.d("LOG_TAG", "OnCancelled fired");
        }

    }



    @Override
    protected void onDestroy() {
        // blarg
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.second, menu);
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