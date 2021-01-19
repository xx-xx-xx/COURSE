package com.example.treehole;

import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.text.format.DateFormat;
import android.util.Log;
import android.view.View;
import android.widget.CheckBox;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.example.treehole.database.ShowListDao;
import com.example.treehole.database.ShowListDatabase;
import com.example.treehole.database.ShowListEntity;
import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.io.File;
import java.io.IOException;
import java.util.Date;
import java.util.Locale;
import java.util.Calendar;

public class recordActivity extends AppCompatActivity {
    FloatingActionButton fab;
    TextView tv;
    TextView time;
    Boolean begin;
    MediaRecorder mMediaRecorder;
    String fileName, filePath;
    File save_file;
    private Handler mHandler_db = new Handler();
    private Handler mHandler_time = new Handler();
    double db = 0;
    AudioShow as;
    CheckBox cb;
    int hour;
    int min;
    int second;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_record);
        initView();
    }
    private void initView(){
        cb = findViewById(R.id.checkBox);
        as = findViewById(R.id.audioshow);
        fab = findViewById(R.id.record2);
        tv = findViewById(R.id.txt_record2);
        time = findViewById(R.id.time);
        begin = true;
        hour = min = second = 0;

        //定时更新分贝值并重绘音量大小变化的自定义view
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                updateMicStatus();
                as.setDB(db);
                as.invalidate();
                mHandler_db.postDelayed(this, 50);// 间隔取样时间为0.05秒
            }
        };
        mHandler_db.postDelayed(runnable, 50);

        //定时更新录制时长
        final Runnable runnable_time = new Runnable() {
            @Override
            public void run() {
                if(second+1==60){
                    if(min+1==60){
                        hour++;
                        min=0;
                    }else {
                        min++;
                        second = 0;
                    }
                }else
                    second++;
                String str = String.format("%02d:%02d:%02d",hour,min,second);
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
                    tv.setText("结束");
                    hour = min = second = 0;
                    mHandler_time.postDelayed(runnable_time, 1000);
                    startRecord();
                }
                else{
                    begin = !begin;
                    LogUtil.d("Record:","end!");
                    tv.setText("开始");
                    mHandler_time.removeCallbacks(runnable_time);
                    stopRecord();
                }
            }
        });
    }
    //准备录制器，决定文件保存路径，开始录制
    public void startRecord() {
        // 开始录音
        // ①Initial：实例化MediaRecorder对象
        if (mMediaRecorder == null)
            mMediaRecorder = new MediaRecorder();
        try {
            //②setAudioSource/setVedioSource
            mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);// 设置麦克风
            // ②设置输出文件的格式：THREE_GPP/MPEG-4/RAW_AMR/Default THREE_GPP(3gp格式
            // ，H263视频/ARM音频编码)、MPEG-4、RAW_AMR(只支持音频且音频编码要求为AMR_NB)
            mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
            //②设置音频文件的编码：AAC/AMR_NB/AMR_MB/Default 声音的（波形）的采样
            mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);

            filePath = getOutputMediaPath();
            LogUtil.d("Record:","filepath:"+filePath);
            /* ③准备 */
            mMediaRecorder.setOutputFile(filePath);
            mMediaRecorder.prepare();
            /* ④开始 */
            mMediaRecorder.start();
        } catch (IllegalStateException e) {
            LogUtil.i("Record:","call startAmr(File mRecAudioFile) failed!" + e.getMessage());
        } catch (IOException e) {
            LogUtil.i("Record:","call startAmr(File mRecAudioFile) failed!" + e.getMessage());
        }
    }
    //获取文件保存路径
    private String getOutputMediaPath() {
        File mediaStorageDir = getExternalFilesDir(Environment.DIRECTORY_MUSIC);
        fileName = DateFormat.format("yyyyMMdd_HHmmss", Calendar.getInstance(Locale.CHINA)) + ".m4a";
        File mediaFile = new File(mediaStorageDir, fileName);
        if (!mediaFile.exists()) {
            mediaFile.getParentFile().mkdirs();
        }
        return mediaFile.getAbsolutePath();
    }
    //结束录制，释放资源
    public void stopRecord() {
        try {
            mMediaRecorder.stop();
            mMediaRecorder.release();
            mMediaRecorder = null;
            if(cb.isChecked()){//删除已保持文件
                File file = new File(filePath);
                if (file.exists())
                    file.delete();
                Toast.makeText(recordActivity.this,"录制完成，已录后即焚！",Toast.LENGTH_LONG)
                        .show();
            }else{//提示用户已经录制完成，保存了文件
                //插入数据库
                new Thread() {
                    @Override
                    public void run() {//存入数据库
                        ShowListDao dao = ShowListDatabase.inst(recordActivity.this).todoListDao();
                        dao.addTodo(new ShowListEntity(filePath, fileName));
                    }
                }.start();
                Toast.makeText(recordActivity.this,"录制完成！已保存！",Toast.LENGTH_LONG)
                        .show();
            }
            filePath = "";
        } catch (RuntimeException e) {//出现异常就删掉文件
            LogUtil.e(e);
            mMediaRecorder.reset();
            mMediaRecorder.release();
            mMediaRecorder = null;

            File file = new File(filePath);
            if (file.exists())
                file.delete();
            filePath = "";
        }
    }
    //计算并更新分贝值
    private void updateMicStatus() {
        if (mMediaRecorder != null) {
            double ratio = (double) mMediaRecorder.getMaxAmplitude() / 1;   // 参考振幅为 1
            if (ratio > 1) {
                db = 20 * Math.log10(ratio);
            }
            Log.d("Record:", "计算分贝值 = " + db + "dB");
        }
    }
}
