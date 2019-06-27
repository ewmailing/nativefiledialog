package net.playcontrol.blurrrcore.testapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity
{

	private final String TAG = "MainActivity";

	// Used to load the 'native-lib' library on application startup.
	static {
		System.loadLibrary("openal");
		System.loadLibrary("ALmixer");
		System.loadLibrary("TestAppLib");
	}

	public native void doALmixerInit();
	public native void doALmixerQuit();
	public native String stringFromJNI();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);


		doALmixerInit();

		// Example of a call to a native method
		TextView tv = (TextView) findViewById(R.id.sample_text);
		tv.setText(stringFromJNI());
//		tv.setText("Boo");
	}

	@Override
	protected void onDestroy()
	{
		doALmixerQuit();
		super.onDestroy();
	}
}

