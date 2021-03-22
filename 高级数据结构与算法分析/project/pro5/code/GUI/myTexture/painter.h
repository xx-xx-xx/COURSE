#ifndef PAINTER_H
#define PAINTER_H

#include <QDialog>
#include<QWidget>
#include<QPainter>
#include<QPen>
#include<QTime>
#include<QDebug>
#include"correctnesstest.h"
namespace Ui {
class Painter;
}

class Painter : public QDialog
{
    Q_OBJECT

public:
    explicit Painter(QWidget *parent = nullptr);
    ~Painter();

private:
    Ui::Painter *ui;

public:
    void paintEvent(QPaintEvent*);
    struct Texture{
        int w;	  //width
        int h;	  //height
        int posw; //the width axis
        int posh; //the height axis
    };
    std::vector<Texture>texturelist;
    void insertTexture(int,int,int,int);
    int tot_height;
    int tot_width;
private slots:
    void on_btn_test_clicked();
};

#endif // PAINTER_H
