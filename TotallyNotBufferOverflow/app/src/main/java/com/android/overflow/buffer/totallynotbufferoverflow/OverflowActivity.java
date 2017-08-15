package com.android.overflow.buffer.totallynotbufferoverflow;

import android.animation.Animator;
import android.animation.ValueAnimator;
import android.support.v4.view.animation.FastOutSlowInInterpolator;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.transition.ChangeBounds;
import android.transition.Fade;
import android.transition.TransitionManager;
import android.transition.TransitionSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.LinearInterpolator;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import java.util.Random;

public class OverflowActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    private TextView mCanaryText;
    private TextView mHackDot;
    private TextView mHackTxt;
    private SeekBar mSeekBar;
    private Button mCanaryButton;
    private RelativeLayout mCanaryLayout;
    private String[] dots = new String[4];
    private int dotIncrementer;
    private int byteSize;
    private String myCanary;

    private int oldProgress;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_overflow);

        // Example of a call to a native method
        mCanaryText = (TextView) findViewById(R.id.canary_text);
        mCanaryButton = (Button) findViewById(R.id.canary_button);
        mSeekBar = (SeekBar) findViewById(R.id.hacking_seekbar);
        mHackDot = (TextView) findViewById(R.id.hacking_dot);
        mHackTxt = (TextView) findViewById(R.id.hacking_txt);
        mCanaryLayout = (RelativeLayout) findViewById(R.id.canary_layout);

        dots[0] = "   ";
        dots[1] = ".  ";
        dots[2] = ".. ";
        dots[3] = "...";

        mCanaryButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TransitionSet ts = new TransitionSet();
                ChangeBounds cb = new ChangeBounds();
                cb.setInterpolator(new FastOutSlowInInterpolator());
                ts.addTransition(cb);
                ts.addTransition(new Fade());
                TransitionManager.beginDelayedTransition((ViewGroup) mCanaryButton.getParent(), ts);
                mHackDot.setVisibility(View.VISIBLE);
                mHackTxt.setVisibility(View.VISIBLE);
                mCanaryButton.setVisibility(View.GONE);
                mSeekBar.setVisibility(View.VISIBLE);
                byteSize = 4;
                myCanary = "0x";
                dotIncrementer = 0;
                oldProgress = 0;
                mHackTxt.setText("HACKING IN PROGRESS");
                final String canary = findMyCanary();
                Log.d("Canary", canary);
                if (canary.length() > 10) {
                    RelativeLayout.LayoutParams lp = (RelativeLayout.LayoutParams) mCanaryLayout.getLayoutParams();
                    DisplayMetrics dm = new DisplayMetrics();
                    getWindowManager().getDefaultDisplay().getMetrics(dm);
                    lp.width = dm.widthPixels*3/4;
                    mCanaryLayout.setLayoutParams(lp);
                }
                ValueAnimator valueAnimator = ValueAnimator.ofInt(0, mSeekBar.getMax());
                valueAnimator.setDuration(5000);
                valueAnimator.setInterpolator(new LinearInterpolator());
                valueAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
                    @Override
                    public void onAnimationUpdate(ValueAnimator valueAnimator) {
                        int progress = (int) valueAnimator.getAnimatedValue();
                        if (progress != oldProgress) {
                            oldProgress = progress;
                            if (progress % (25 / 3) == 0) {
                                mHackDot.setText(dots[dotIncrementer]);
                                dotIncrementer++;
                                if (dotIncrementer == 4)
                                    dotIncrementer = 0;
                            }
                            if (byteSize != 0  && progress % (mSeekBar.getMax() / 4) == 0) {
                                byteSize--;
                                if (progress == mSeekBar.getMax()) {
                                    myCanary = canary;
                                    TransitionSet ts = new TransitionSet();
                                    ChangeBounds cb = new ChangeBounds();
                                    cb.setInterpolator(new FastOutSlowInInterpolator());
                                    ts.addTransition(cb);
                                    ts.addTransition(new Fade());
                                    TransitionManager.beginDelayedTransition((ViewGroup) mSeekBar.getParent(), ts);
                                    mHackDot.setVisibility(View.GONE);
                                    mHackTxt.setText("COMPLETE");
                                    mSeekBar.setVisibility(View.INVISIBLE);
                                    mCanaryButton.setVisibility(View.VISIBLE);
                                } else {
                                    myCanary = canary.substring(0,2+(2 * (4 - byteSize)));
                                }
                            }
                            if (byteSize > 0) {
                                byte[] b = new byte[byteSize];
                                new Random().nextBytes(b);
                                String bStr = "";
                                try {
                                    bStr = b.toString().substring(3, b.toString().length() - 2 * (4 - byteSize));
                                } catch (StringIndexOutOfBoundsException ex) {
                                    ex.printStackTrace();
                                }
                                String output = myCanary+bStr;
                                for (int i = output.length(); i < 8; i++)
                                    output += new Random().nextInt(9);
                                mCanaryText.setText(output);
                            } else {
                                mCanaryText.setText(canary);
                            }
                        }
                        mSeekBar.setProgress(progress);
                    }
                });
                valueAnimator.start();
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String findMyCanary();
}
