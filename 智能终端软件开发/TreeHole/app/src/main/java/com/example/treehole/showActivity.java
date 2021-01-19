package com.example.treehole;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.example.treehole.database.ShowListDao;
import com.example.treehole.database.ShowListDatabase;
import com.example.treehole.database.ShowListEntity;

import java.util.Date;
import java.util.List;

public class showActivity extends AppCompatActivity implements showListAdapter.IOnItemClickListener {

    private RecyclerView recyclerView;
    private showListAdapter mAdapter;
    private static final int REQUEST_CODE_1 = 1357;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_show);

        recyclerView = findViewById(R.id.recycler);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        mAdapter = new showListAdapter();
        mAdapter.setOnItemClickListener(this);
        recyclerView.setAdapter(mAdapter);
        final Handler h = new Handler();
        Runnable r = new Runnable() {
            @Override
            public void run() {//定时刷新
                loadFromDatabase();
                h.postDelayed(this,1000);
            }
        };
        new Thread(r).start();
    }

    private void loadFromDatabase() {
        new Thread() {
            @Override
            public void run() {
                ShowListDao dao = ShowListDatabase.inst(showActivity.this).todoListDao();
                final List<ShowListEntity> entityList = dao.loadAll();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mAdapter.setData(entityList);
                    }
                });
            }
        }.start();
    }

    @Override
    public void onItemCLick(int position, ShowListEntity data) {
        //创建意图对象
        Intent intent = new Intent(this, PlayActivity.class);
        //设置传递键值对
        String str = data.getFilepath();
        intent.putExtra("filepath",str);
        //激活意图
        startActivity(intent);
    }
}
