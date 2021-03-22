#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTextDocument>
#include<QTextBlock>
#include<QString>
#include<QDebug>
#include<QFileDialog>
#include <QTextCodec>
#include<QMessageBox>
#include<vector>
#include<QWidget>
#include<QPainter>
#include<QPen>
#include<time.h>
#include"painter.h"
#define N 10005 //maximum number of textures
#define MIN 0x3f3f3f3f
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_fileimport_clicked();

    void on_btn_ffdh_clicked();

    void on_btn_nfdh_clicked();

    void on_btn_bfdh_clicked();

private:
    Ui::MainWindow *ui;
    int n,width;

    //the structure to save a level
    struct level
    {
        int h;	//height of the level
        int rem_w;	//remaining width
        int tot_h;	//total height
    };
    //the structure to save a rectangle
    struct Texture{
        int w;	  //width
        int h;	  //height
        int posw; //the width axis
        int posh; //the height axis
    };
    std::vector<Texture>texturelist;
    vector<level*>levellist;
    void out_position();
    void paint();
    int min_height;
    //First-Fit Decreasing Height algorithm
    int FFDH();
    int NFDH();
    int BFDH();


};



#endif // MAINWINDOW_H
