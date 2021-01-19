package com.example.treehole;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.MainThread;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.treehole.database.ShowListEntity;

import java.util.ArrayList;
import java.util.List;

public class showListAdapter extends RecyclerView.Adapter<showListItemHolder> {
    private List<ShowListEntity> mDatas;
    private IOnItemClickListener  mItemClickListener;

    public showListAdapter() {
        mDatas = new ArrayList<>();
    }
    @NonNull
    @Override
    public showListItemHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new showListItemHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.item_layout, parent, false));
    }

    public void setOnItemClickListener(IOnItemClickListener listener) {
        mItemClickListener = listener;
    }

    @Override
    public void onBindViewHolder(@NonNull showListItemHolder holder, final int position) {
        holder.bind(mDatas.get(position));
        holder.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mItemClickListener != null) {
                    mItemClickListener.onItemCLick(position, mDatas.get(position));
                }
            }
        });
    }

    @Override
    public int getItemCount() {
        return mDatas.size();
    }

    @MainThread
    public void setData(List<ShowListEntity> list) {
        mDatas = list;
        notifyDataSetChanged();
    }

    public interface IOnItemClickListener {
        void onItemCLick(int position, ShowListEntity data);
    }
}
