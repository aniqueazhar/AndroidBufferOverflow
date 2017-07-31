package com.example.wil.helloworld;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    public String buffer = "";
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method

        final EditText et = (EditText) findViewById(R.id.buffer_text);
        buffer = et.getText().toString();

        final TextView tv = (TextView) findViewById(R.id.sample_text);

        et.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
                    buffer = et.getText().toString();
                    et.setText("");
                    Log.d("debug", "Made it here~!");
                    Log.d("debug", "STR = " + buffer);
                    Log.d("debug", "" + getBufferForNative(buffer));
                    tv.setText(getBufferForNative(buffer));
                }
                return false;
            }
        });


    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String getBufferForNative(String buf);
}
