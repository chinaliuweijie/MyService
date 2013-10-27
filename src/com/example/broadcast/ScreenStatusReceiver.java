package com.example.broadcast;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;


/**
 * ��Ļ״̬�㲥
 * @author yuegy
 *
 */
public class ScreenStatusReceiver extends BroadcastReceiver {
	String SCREEN_ON = "android.intent.action.USER_PRESENT";
	
	@Override
	public void onReceive(Context context, Intent intent) {
		
		Log.e("ScreenStatusReceiver", SCREEN_ON);
		
		// ��Ļ����
		if(SCREEN_ON.equals(intent.getAction())){
			Log.e("ScreenStatusReceiver", SCREEN_ON);
		}
	}

		
}
