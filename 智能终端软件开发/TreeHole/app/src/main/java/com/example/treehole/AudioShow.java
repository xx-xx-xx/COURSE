package com.example.treehole;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

import com.example.treehole.R;

public class AudioShow extends View {
    private Paint paint_150 = new Paint();
    private Paint paint_255 = new Paint();
    private double height, wide, radius;
    private Handler handler = new Handler();
    double db;

    public AudioShow(Context context, recordActivity rA) {
        super(context);
        init();
    }
    public AudioShow(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }
    public AudioShow(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }
    public void setDB(double db){
        this.db = db;
    }
    private void init() {
        paint_150.setAntiAlias(true);
        paint_150.setColor(Color.rgb(96,187,144));
        paint_150.setAlpha(150);
        paint_150.setStrokeWidth(10);
        paint_150.setAntiAlias(true);// 设置画笔的锯齿效果
        paint_150.setStrokeCap(Paint.Cap.ROUND);
        paint_150.setStyle(Paint.Style.FILL);

        paint_255.setAntiAlias(true);
        paint_255.setColor(Color.rgb(96,187,144));
        paint_255.setAlpha(255);
        paint_255.setStrokeWidth(10);
        paint_255.setAntiAlias(true);// 设置画笔的锯齿效果
        paint_255.setStrokeCap(Paint.Cap.ROUND);
        paint_255.setStyle(Paint.Style.FILL);
    }
    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        configWhenLayoutChanged();
    }

    private void configWhenLayoutChanged() {
        double newRadius = Math.min(getWidth(), getHeight()) / 2f;
        radius = newRadius;
        wide = getWidth();
        height = getHeight();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        double radio = db/93.0;
        RectF oval2 = new RectF();// 设置个新的长方形，扫描测量
        oval2.set((int)wide/3,(int)height/3,(int)wide/3*2,(int)height/3*2);
        canvas.drawRoundRect(oval2, 100, 100, paint_150);

        //不用地方高           原不用地方高
        int top =(int) (height/3.0*(1.0-radio)+height/3.0);
        oval2.set((int)wide/3, top,(int)wide/3*2,(int)height/3*2);
        canvas.drawRoundRect(oval2, 100, 100, paint_255);
    }
}