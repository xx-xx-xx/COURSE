#include "painter.h"
#include "ui_painter.h"

Painter::Painter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Painter)
{
    texturelist.clear();
    ui->setupUi(this);
}

Painter::~Painter()
{
    delete ui;
}


void Painter::paintEvent(QPaintEvent *){
    int window_width=this->x();
    int window_height=this->y();
    qDebug()<<texturelist.size();
    for(int i=0;i<texturelist.size();i++){
        QPainter painter(this);
        //qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
        int r[7]={237,251,96,1,179,17,167};
        int g[7]={222,178,143,77,214,63,220};
        int b[7]={139,23,159,103,110,61,224};
       QColor tmpcolor(r[i%7],g[i%7],b[i%7]);
        //QColor tmpcolor(100,100,100);
        QPen pen(tmpcolor);
        painter.setPen(tmpcolor);
        QBrush brush(tmpcolor,Qt::SolidPattern);
        painter.setBrush(brush);
        QRectF rectangle2(texturelist[i].posw*window_width/tot_width+30,texturelist[i].posh*2*window_height/tot_height+window_height*0.4,texturelist[i].w*window_width/tot_width,texturelist[i].h*2*window_height/tot_height);
        painter.drawRect(rectangle2);
    }


}

void Painter::insertTexture(int w,int h,int pos_w,int pos_h){
    struct Texture newt;
    newt.w=w;
    newt.h=h;
    newt.posw=pos_w;
    newt.posh=pos_h;
    texturelist.push_back(newt);
}


void Painter::on_btn_test_clicked()
{
    CorrectnessTest myTest;
    myTest.Texturelst.clear();
    qDebug()<<myTest.Texturelst.size();
    myTest.tot_width=tot_width;
    myTest.tot_height=tot_height;
    for(int i=0;i<texturelist.size();i++){
        if(!myTest.testTexture(texturelist[i].w,texturelist[i].h,texturelist[i].posw,texturelist[i].posh)){
            ui->result->setText("ERROR!");
            return;
        }
    }
    if(!myTest.testTot()){
        ui->result->setText("ERROR!");
        return;
    }
    ui->result->setText("Correct!");
    return;

}
