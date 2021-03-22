#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<bits/stdc++.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dataWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//平均分配列宽
    ui->position_widgt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//平均分配列宽
    ui->iterat_times->setText("1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileimport_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("选择文件"),"D:\\");
    //qDebug()<<filename;
    //std::string s=filename.toStdString();
    char* ch;
    QByteArray ba=filename.toLatin1();
    ch=ba.data();
    std::freopen(ch,"r",stdin);

    std::cin>>n>>width;
    qDebug()<<width;
    ui->fixW->setText(QString::number(width));
    for(int i=0;i<n;i++){
        START:
            int tmpwidth,tmpheight;
            std::cin>>tmpwidth>>tmpheight;
            ui->dataWidget->setRowCount(i+1);
            ui->dataWidget->setItem(i,0,new QTableWidgetItem(QString::number(tmpwidth)));
            ui->dataWidget->setItem(i,1,new QTableWidgetItem(QString::number(tmpheight)));
            struct Texture newtexture;
            newtexture.w=tmpwidth;
            newtexture.h=tmpheight;
            texturelist.push_back(newtexture);
            if(tmpwidth>width){
                QMessageBox::warning(this,tr("error"),tr("wrong input data"));
                goto START;
            }
    }
    //std::sort(texturelist.begin(),texturelist.end(),cmp);
    for(int i=0;i<texturelist.size()-1;i++){
        for(int j=0;j<texturelist.size()-1-i;j++){
            if(texturelist[j].h<texturelist[j+1].h){
                struct Texture tmp;
                tmp=texturelist[j];
                texturelist[j]=texturelist[j+1];
                texturelist[j+1]=tmp;
            }
        }
    }
}

void MainWindow::on_btn_ffdh_clicked()
{
    int begin,end;
    int run_times=ui->iterat_times->text().toInt();
    begin=clock();
    for(int i=0;i<run_times;i++){
        min_height=FFDH();
    }
    end=clock();
    ui->outTime->setText(QString::number(end-begin));
    double pertime=(end-begin)*1.0/run_times;
    ui->timeper->setText(QString("%1").arg(pertime));
    //qDebug()<<min_height;
    ui->outHeight->setText(QString::number(min_height));
    out_position();
    paint();
}

void MainWindow::on_btn_nfdh_clicked()
{
    int begin,end;
    int run_times=ui->iterat_times->text().toInt();
    begin=clock();
    for(int i=0;i<run_times;i++){
        min_height=NFDH();
    }
    end=clock();
    ui->outTime->setText(QString::number(end-begin));
    double pertime=(end-begin)*1.0/run_times;
    ui->timeper->setText(QString("%1").arg(pertime));
    //qDebug()<<min_height;
    ui->outHeight->setText(QString::number(min_height));
    out_position();
    paint();
}

void MainWindow::on_btn_bfdh_clicked()
{
    int begin,end;
    int run_times=ui->iterat_times->text().toInt();
    begin=clock();
    for(int i=0;i<run_times;i++){
        min_height=BFDH();
    }
    end=clock();
    ui->outTime->setText(QString::number(end-begin));
    double pertime=(end-begin)*1.0/run_times;
    ui->timeper->setText(QString("%1").arg(pertime));
    //qDebug()<<min_height;
    ui->outHeight->setText(QString::number(min_height));
    out_position();
    paint();
}


void MainWindow::out_position(){
    ui->position_widgt->clearContents();
    ui->position_widgt->setRowCount(texturelist.size());
    for(int i=0;i<texturelist.size();i++){
        ui->position_widgt->setItem(i,0,new QTableWidgetItem(QString::number(texturelist[i].w)));
        ui->position_widgt->setItem(i,1,new QTableWidgetItem(QString::number(texturelist[i].h)));
        ui->position_widgt->setItem(i,2,new QTableWidgetItem(QString::number(texturelist[i].posw)));
        ui->position_widgt->setItem(i,3,new QTableWidgetItem(QString::number(texturelist[i].posh)));
    }
}



//First-Fit Decreasing Height algorithm
int MainWindow::FFDH(){
    levellist.clear();
    for(int i=0;i<n;i++){
        int flag=0;//flag=1 if the texture should be added
        //check each level
        for(int j=0;j<levellist.size();j++){
            //if the texture can be put into level j
            if(levellist[j]->rem_w>=texturelist[i].w){
                flag=1;
                //calculate the position of texture in the strip
                texturelist[i].posw = width - levellist[j]->rem_w;
                texturelist[i].posh = levellist[j]->tot_h;
                //update the remaining width
                levellist[j]->rem_w -= texturelist[i].w;
                goto NEXT;
            }
        }
    NEXT:
        //create a new level
        if (!flag)
        {
            level *p = new level;
            //the new level's height=current texture's height
            p->h = texturelist[i].h;
            //calculate the total height of the new level
            p->tot_h = (levellist.size() == 0) ? 0 : levellist[levellist.size() - 1]->tot_h + levellist[levellist.size() - 1]->h;
            //update the remaining width
            p->rem_w = width - texturelist[i].w;
            //add p to l
            levellist.push_back(p);
            //calculate the position of new texture in the strip
            texturelist[i].posw = 0;
            texturelist[i].posh = p->tot_h;
        }
    }
    return ((levellist.size()==0)?0:levellist[levellist.size()-1]->tot_h+levellist[levellist.size()-1]->h);
}

//Next-Fit Decreasing Height algorithm
int MainWindow::NFDH(){
    levellist.clear();
    for (int i = 0; i < n; i++)
        {
            int flag = 0;	//flag=1 if the texture should be added
            //check the last level
            for (int j=levellist.size()-1; j < levellist.size(); j++)
            {
                //if the texture can be put into level j
                if (levellist[j]->rem_w >= texturelist[i].w)
                {
                    flag = 1;
                    //calculate the position of texture in the strip
                    texturelist[i].posw = width - levellist[j]->rem_w;
                    texturelist[i].posh = levellist[j]->tot_h;
                    //update the remaining width
                    levellist[j]->rem_w -= texturelist[i].w;
                    goto NEXT;
                }
            }
        NEXT:
            //create a new level
            if (!flag)
            {
                level *p = new level;
                //the new level's height=current texture's height
                p->h = texturelist[i].h;
                //calculate the total height of the new level
                p->tot_h = (levellist.size() == 0) ? 0 : levellist[levellist.size() - 1]->tot_h + levellist[levellist.size() - 1]->h;
                //update the remaining width
                p->rem_w = width - texturelist[i].w;
                //add p to l
                levellist.push_back(p);													//calculate the position of new texture in the strip
                texturelist[i].posw = 0;
                texturelist[i].posh = p->tot_h;
            }
        }
        return ((levellist.size()==0)?0:levellist[levellist.size()-1]->tot_h+levellist[levellist.size()-1]->h);
}

//Best-Fit Decreasing Height algorithm
int MainWindow::BFDH(){
    levellist.clear();
    //find the best fit level whose residual width is minimum
        for (int i = 0; i < n; i++)
        {
            int min_num = -1;	//num of min level
            int min_w=MIN;	//min width
            //check each level
            for (int j = 0; j < levellist.size(); j++)
            {
                //if the texture can be put into level j
                if (levellist[j]->rem_w >= texturelist[i].w &&(min_w>levellist[j]->rem_w - texturelist[i].w))
                {
                    //update the min num and width
                    min_num = j;
                    min_w=levellist[j]->rem_w - texturelist[i].w;
                }
            }
            //if there exists a best level
            if (min_num>=0)
            {
                //calculate the position of new texture in the strip
                texturelist[i].posw = width-levellist[min_num]->rem_w;
                texturelist[i].posh = levellist[min_num]->tot_h;
                //update the remaining width
                levellist[min_num]->rem_w-=texturelist[i].w;
            }
            //else create a new level
            else{
                level *p = new level;
                //the new level's height=current texture's height
                p->h = texturelist[i].h;
                //calculate the total height of the new level
                p->tot_h = (levellist.size() == 0) ? 0 : levellist[levellist.size() - 1]->tot_h + levellist[levellist.size() - 1]->h;
                //update the remaining width
                p->rem_w = width - texturelist[i].w;
                //add p to l
                levellist.push_back(p);													//calculate the position of new texture in the strip
                texturelist[i].posw = 0;
                texturelist[i].posh = p->tot_h;
            }
        }
        return ((levellist.size()==0)?0:levellist[levellist.size()-1]->tot_h+levellist[levellist.size()-1]->h);
}

void MainWindow::paint()
{
    Painter *newpaint=new Painter;
    for(int i=0;i<texturelist.size();i++){
        newpaint->insertTexture(texturelist[i].w,texturelist[i].h,texturelist[i].posw,texturelist[i].posh);
    }
    newpaint->tot_height=min_height;
    newpaint->tot_width=width;
    newpaint->show();
}


