package com.example.treehole.database;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;

import java.util.List;

@Dao
public interface ShowListDao {
    @Query("SELECT * FROM todo")
    List<ShowListEntity> loadAll();

    @Query("DELETE FROM todo")
    void deleteAll();

    @Insert
    long addTodo(ShowListEntity entity);

    @Delete()
    void deleteTodo(ShowListEntity entity);
}
