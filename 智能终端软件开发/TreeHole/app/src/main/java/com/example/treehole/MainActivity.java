package com.example.treehole;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

public class MainActivity extends AppCompatActivity{//implements View.OnClickListener {

    private FloatingActionButton fab_show;
    private FloatingActionButton fab_record;
    private Button btnLogout;
    private int PERMISSION_REQUEST_CODE = 1028;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
    }

    private void initView(){
        fab_record = findViewById(R.id.record);
        fab_show = findViewById(R.id.show);
        btnLogout=findViewById(R.id.bt_main_logout);
        //获取权限
        get_permissions();
        //设置响应函数
        fab_show.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,showActivity.class);
                startActivity(intent);
            }
        });
        fab_record.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,recordActivity.class);
                startActivity(intent);
            }
        });

        btnLogout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,loginActivity.class);
                startActivity(intent);
            }
        });
//       // 初始化控件对象
//        Button mBtMainLogout = findViewById(R.id.bt_main_logout);
//        // 绑定点击监听器
//        mBtMainLogout.setOnClickListener(this);
    }

    private void get_permissions()//申请所有需要的权限
    {
        String[] permissions_camera = new String[]{Manifest.permission.RECORD_AUDIO,Manifest.permission.READ_EXTERNAL_STORAGE,Manifest.permission.WRITE_EXTERNAL_STORAGE};
        ActivityCompat.requestPermissions(this,permissions_camera,PERMISSION_REQUEST_CODE);
    }

    public void onClick(View view) {
        if (view.getId() == R.id.bt_main_logout) {
            Intent intent = new Intent(this, loginActivity.class);
            startActivity(intent);
            finish();
        }
    }
}
