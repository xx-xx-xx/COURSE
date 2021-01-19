package com.example.treehole.database;

import android.content.Context;

import androidx.room.Database;
import androidx.room.Room;
import androidx.room.RoomDatabase;

@Database(entities = {ShowListEntity.class}, version = 1)
public abstract class ShowListDatabase extends RoomDatabase {
    private static volatile ShowListDatabase INSTANCE;

    public abstract ShowListDao todoListDao();

    public ShowListDatabase() {

    }

    public static ShowListDatabase inst(Context context) {
        if (INSTANCE == null) {
            synchronized (ShowListDatabase.class) {
                if (INSTANCE == null) {
                    INSTANCE = Room.databaseBuilder(context.getApplicationContext(), ShowListDatabase.class, "todo.db").build();
                }
            }
        }
        return INSTANCE;
    }
}
