package com.example.treehole;

import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.treehole.database.ShowListDao;
import com.example.treehole.database.ShowListDatabase;
import com.example.treehole.database.ShowListEntity;

import java.io.File;

public class showListItemHolder extends RecyclerView.ViewHolder {
    private TextView filename;
    private Button btn_x;
    View contentView;

    public showListItemHolder(@NonNull View itemView) {
        super(itemView);
        contentView = itemView;
        filename = itemView.findViewById(R.id.tv_content);
        btn_x = itemView.findViewById(R.id.btn_x);
    }

    public void setOnClickListener(View.OnClickListener listener) {
        if (listener != null) {
            contentView.setOnClickListener(listener);
        }
    }

    public void bind(final ShowListEntity entity) {
        filename.setText(entity.getFilename());

        btn_x.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(final View view) {
                new Thread() {
                    @Override
                    public void run() {
                        ShowListDao dao = ShowListDatabase.inst(view.getContext()).todoListDao();
                        File file = new File(entity.getFilepath());
                        if(file.exists()) {
                            if (file.delete())
                                dao.deleteTodo(entity);
                            else
                                Toast.makeText(view.getContext(), "删除失败", Toast.LENGTH_LONG).show();
                        }else
                            dao.deleteTodo(entity);
                    }
                }.start();
                filename.setText("");
                Toast.makeText(view.getContext(), "删除成功", Toast.LENGTH_LONG).show();
            }
        });
    }
}


