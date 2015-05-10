package com.example.jerry.smartbike;

import android.app.Activity;
import android.app.Notification;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.CountDownTimer;
import android.os.Handler;
import android.provider.AlarmClock;
import android.service.notification.NotificationListenerService;
import android.service.notification.StatusBarNotification;
import java.io.*;
import java.util.Set;
import java.util.UUID;

import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.LatLng;

import static java.lang.Integer.parseInt;
import static java.lang.Integer.toString;

/**
 * Created by Jerry on 09/05/2015.
 */
public class NotificationListener extends NotificationListenerService {

    private String TAG = this.getClass().getSimpleName();
    private NLServiceReceiver nlservicereciver;
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothDevice bluetoothDevice;
    private BluetoothSocket bluetoothSocket;
    private OutputStream outputStream;
    private InputStream inputStream;
    private boolean replyReceived;
    private boolean createNewTimer;

    @Override
    public void onCreate() {
        super.onCreate();
        nlservicereciver = new NLServiceReceiver();
            IntentFilter filter = new IntentFilter();
        filter.addAction(".NOTIFICATION_LISTENER_SERVICE_EXAMPLE");
        registerReceiver(nlservicereciver, filter);

        //attach bluetooth
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        Set<BluetoothDevice> devices = bluetoothAdapter.getBondedDevices();

        for(BluetoothDevice d : devices){
            if(d.getName().equals("HC-06")){
                bluetoothDevice = d;
                Log.d("BLUETOOTH","CONNECTED");
                break;
            }
        }
        try {
            UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"); //Standard SerialPortService ID
            bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(uuid);
            bluetoothSocket.connect();
            outputStream = bluetoothSocket.getOutputStream();
            inputStream = bluetoothSocket.getInputStream();
            beginListenForData();
        }
        catch(IOException e){}

        replyReceived = false;

        final Handler check = new Handler();
        check.postDelayed(new Runnable() {
            public void run() {
                try {
                    sendData("C");
                    replyReceived = false;
                    new CountDownTimer(5000, 1000) {

                        public void onTick(long millisUntilFinished) {
                        }
                        public void onFinish() {
                            if(!replyReceived){
                                Log.d("BLUETOOTH","DISCONNECTED");
                            }
                        }
                    }.start();
                }
                catch(Exception e){}
                check.postDelayed(this,10000);
            }
        }, 1000);

    }

    //variables
    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    volatile boolean stopWorker;

    //code for bt listening
    void beginListenForData() {
        final Handler handler = new Handler();
        final byte delimiter = 10; //This is the ASCII code for a newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        workerThread = new Thread(new Runnable()
        {
            public void run()
            {
                while(!Thread.currentThread().isInterrupted() && !stopWorker)
                {
                    try
                    {
                        int bytesAvailable = inputStream.available();

                        if(bytesAvailable > 0)
                        {
                            replyReceived = true;
                            byte[] packetBytes = new byte[bytesAvailable];
                            inputStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++)
                            {
                                byte b = packetBytes[i];
                                if(b == delimiter)
                                {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;

                                    Log.d("input",data);
                                    if (data.contains("u")){
                                        Toast.makeText(getApplicationContext(),"Bike has been tampered with! Go check on it!",Toast.LENGTH_LONG).show();

                                    }
                                    if(data.contains("x")){
                                        closeBT();
                                    }
                                }
                                else
                                {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException ex)
                    {
                        stopWorker = true;
                    }
                }
            }
        });
        workerThread.start();
    }

    void sendData(String msg) throws IOException
    {
        Log.d("Sent",msg);
        msg += "\n";
        byte[] msgBuffer = msg.getBytes();
        try{
            outputStream.write(msgBuffer);
        }
        catch (IOException e) {}
    }

    void closeBT() throws IOException
    {
        stopWorker = true;
        outputStream.close();
        inputStream.close();
        bluetoothSocket.close();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        try {
            closeBT();
        }
        catch (IOException e) {}
        unregisterReceiver(nlservicereciver);
    }


    class NLServiceReceiver extends BroadcastReceiver {

        boolean toSend = false;
        @Override
        public void onReceive(Context context, Intent intent) {

            for (StatusBarNotification sbn : NotificationListener.this.getActiveNotifications()) {
                if (sbn.getPackageName().equals("com.google.android.apps.maps")) {
                    Notification n = sbn.getNotification();
                    String nText = n.extras.getCharSequence(Notification.EXTRA_TEXT).toString();

                    //for testing
                    //nText = "120 m - Turn left at Queen";

                    Log.d("TEXT",nText);
                    int dis = ~0;

                    int index =((nText.indexOf("m")) == -1 ? 1: nText.indexOf("m")) - 1;

                    Log.d("INDEX",Integer.toString(index));
                    Log.d("SUBSTRING",nText.substring(0,index));
                    char dir;

                    try {
                        dis = Integer.parseInt(nText.substring(0, index));
                        toSend = true;
                    }
                    catch(Exception e){
                        toSend = false;
                    }

                    if(toSend)
                        Log.d("TRUE",Integer.toString(dis));
                    else
                        Log.d("FALSE","");

                    char lr = '0';
                    if(nText.contains("right")){
                        lr = 'R';
                    }
                    else if (nText.contains("left")){
                        lr = 'L';
                    }
                    else {
                        toSend = false;
                    }
                    Log.d("L/R: ",Character.toString(lr));

                    if(toSend && dis <= 30){
                        try {
                            sendData(Character.toString(lr));
                        }
                        catch(IOException e){}
                    }
                }
                break;
            }
        }
    }
}
