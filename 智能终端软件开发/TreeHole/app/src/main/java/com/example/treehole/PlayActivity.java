package com.example.treehole;

import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.CheckBox;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.io.File;

public class PlayActivity extends AppCompatActivity {
    FloatingActionButton fab;
    TextView tv;
    TextView time;
    Boolean begin;
    String filePath;
    private MediaPlayer mediaPlayer = new MediaPlayer();

    private Handler mHandler_db = new Handler();
    private Handler mHandler_time = new Handler();
    double db = 0;
    AudioShow as;
    CheckBox cb;
    int hour;
    int min;
    int second;
    String Duration;

    private SeekBar sb_normal;
    private Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play);
        mContext = PlayActivity.this;
        initView();
    }
    private void initView(){
        cb = findViewById(R.id.checkBox);
        as = findViewById(R.id.audioshow);
        fab = findViewById(R.id.record2);
        tv = findViewById(R.id.txt_record2);
        time = findViewById(R.id.time);
        sb_normal = (SeekBar) findViewById(R.id.sb_normal);
        begin = true;
        hour = min = second = 0;

        // 获取意图对象
        Intent intent = getIntent();
        //获取传递的值
        filePath = intent.getStringExtra("filepath");
        //初始化播放器
        initMediaPlayer();

        final File file = new File(filePath);
        //定时更新播放时长
        final Runnable runnable_time = new Runnable() {
            @Override
            public void run() {
                    sb_normal.setProgress(sb_normal.getProgress()+1000);
                    second = sb_normal.getProgress()/1000;
                    //second=mediaPlayer.getCurrentPosition()/1000;
                    hour=second/3600;
                    min=(second%3600)/60;
                    second=second%60;
                    String str=String.format("%02d:%02d:%02d",hour,min,second);
                    if(file.exists())
                        str = str +"/" + Duration;
                    else
                        str = "文件已删除";
                    time.setText(str);
                    mHandler_time.postDelayed(this, 1000);// 间隔取样时间为1秒
            }
        };
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(begin) {
                    begin = !begin;
                    LogUtil.d("Record:","begin!");
                    tv.setText("暂停");
                    hour = min = second = 0;
                    mHandler_time.postDelayed(runnable_time, 1000);
                    mediaPlayer.start();
                }
                else{
                    begin = !begin;
                    LogUtil.d("Record:","end!");
                    tv.setText("开始");
                    mHandler_time.removeCallbacks(runnable_time);
                    mediaPlayer.pause();
                }
            }
        });

    }

    private void initMediaPlayer() {
        try {
            mediaPlayer.setDataSource(filePath);//指定音频文件路径
            mediaPlayer.setLooping(false);//设置为循环播放
            mediaPlayer.prepare();//初始化播放器MediaPlayer
            sb_normal.setMax(mediaPlayer.getDuration());
        } catch (Exception e) {
            e.printStackTrace();
        }
        int duration= mediaPlayer.getDuration()/1000;
        Duration =String.format("%02d:%02d:%02d",duration/3600,(duration%3600)/60,duration%60);
        time.setText("00:00:00/"+Duration);

        sb_normal.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                second=seekBar.getProgress()/1000;
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                Toast.makeText(mContext, "触碰SeekBar", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Toast.makeText(mContext, "放开SeekBar", Toast.LENGTH_SHORT).show();
                mediaPlayer.seekTo(second * 1000);
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();
        if(cb.isChecked()){
            File file = new File(filePath);
            if(file.exists())
                file.delete();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(cb.isChecked()){
            File file = new File(filePath);
            if(file.exists())
                file.delete();
        }
        mediaPlayer.reset();//重置MediaPlayer
        mediaPlayer.release();//释放MediaPlayer

    }
}
