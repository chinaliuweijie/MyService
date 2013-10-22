package com.example.myservice;

import com.example.broadcast.BootBroadcast;
import com.example.service.MyService;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {

	private Button startBtn;
	private Button stopBtn;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		startBtn = (Button)findViewById(R.id.start);
		stopBtn = (Button)findViewById(R.id.stop);
		bindEvent();
		
		Intent intent2 = new Intent(MainActivity.this, BootBroadcast.class);
		intent2.setAction("com.example.broad.action");
		sendBroadcast(intent2);
	}

	private void bindEvent(){
		startBtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				Log.i("MainActivity-->startbtn", "1");
				Intent intent = new Intent(MainActivity.this, MyService.class);
				startService(intent);
				
			}
		});
		
		stopBtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Log.i("MainActivity-->stopbtn", "1");
				Intent intent = new Intent(MainActivity.this, MyService.class);
				stopService(intent);
			}
		});
	}

}
