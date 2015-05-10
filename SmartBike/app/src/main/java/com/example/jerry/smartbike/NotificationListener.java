package com.example.jerry.smartbike;

import android.app.Notification;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.service.notification.NotificationListenerService;
import android.service.notification.StatusBarNotification;
import java.io.*;
import android.util.Log;

import static java.lang.Integer.parseInt;
import static java.lang.Integer.toString;

/**
 * Created by Jerry on 09/05/2015.
 */
public class NotificationListener extends NotificationListenerService {

    private String TAG = this.getClass().getSimpleName();
    private NLServiceReceiver nlservicereciver;

    @Override
    public void onCreate() {
        super.onCreate();
        nlservicereciver = new NLServiceReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction(".NOTIFICATION_LISTENER_SERVICE_EXAMPLE");
        registerReceiver(nlservicereciver,filter);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        unregisterReceiver(nlservicereciver);
    }

    @Override
    public void onNotificationPosted(StatusBarNotification sbn) {
/*
        Log.d(TAG,"**********  onNotificationPosted");
        Log.d(TAG,"ID :" + sbn.getId() + "t" + sbn.getNotification().tickerText + "t" + sbn.getPackageName());
        Intent i = new  Intent(".NOTIFICATION_LISTENER_EXAMPLE");
        i.putExtra("notification_event","onNotificationPosted :" + sbn.getPackageName() + "n");
        sendBroadcast(i);*/

    }

    @Override
    public void onNotificationRemoved(StatusBarNotification sbn) {
        /*Log.d(TAG,"********** onNOtificationRemoved");
        Log.d(TAG,"ID :" + sbn.getId() + "t" + sbn.getNotification().tickerText +"t" + sbn.getPackageName());
        Intent i = new  Intent(".NOTIFICATION_LISTENER_EXAMPLE");
        i.putExtra("notification_event","onNotificationRemoved :" + sbn.getPackageName() + "n");

        sendBroadcast(i);*/
    }


    class NLServiceReceiver extends BroadcastReceiver {

        boolean toSend = false;
        @Override
        public void onReceive(Context context, Intent intent) {
            for (StatusBarNotification sbn : NotificationListener.this.getActiveNotifications()) {
                if (sbn.getPackageName().equals("com.google.android.apps.maps")) {
                    Notification n = sbn.getNotification();
                    String nText = n.extras.getCharSequence(Notification.EXTRA_TEXT).toString();
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
                        
                    }
                }
            }
        }
    }
}
