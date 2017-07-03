#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaType>
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include <QDebug>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    //=======QDesktopWidget=======
    QDesktopWidget *desktop=QApplication::desktop();
    bool shi=desktop->isVirtualDesktop();
    int num=desktop->numScreens();       //显示屏数
    int nump=desktop->primaryScreen();   //主要的屏幕
    int nnn=desktop->screenNumber();     //屏幕数

    if(num > 0)
    {
        //widget_ext = NULL;
        //widget_ext = new QOpenGLVideoRenderer();
        widget_ext = new QWidgetPlayer;

    }

    //=============================

    av_register_all(); //初始化FFMPEG  调用了这个才能正常使用编码器和解码器

    mPlayer = new VideoThread;    //解析音视频类对象
    connect(mPlayer,SIGNAL(sig_GetOneFrame(QImage)),this,SLOT(slotGetOneFrame(QImage)));
    connect(mPlayer,SIGNAL(sig_TotalTimeChanged(qint64)),this,SLOT(slotTotalTimeChanged(qint64)));
    //connect(mPlayer,SIGNAL(sig_StateChanged(VideoThread::PlayerState)),this,SLOT(slotStateChanged(VideoThread::PlayerState)));

    mTimer = new QTimer;    //定时器-获取当前视频时间
    connect(mTimer,SIGNAL(timeout()),this,SLOT(slotTimerTimeOut()));
    mTimer->setInterval(500);

    //按钮
    connect(ui->pushButton_open,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->pushButton_play,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->pushButton_pause,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->pushButton_stop,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->pushButton_two,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotSliderMoved(int)));

    connect(this, SIGNAL(sig_fullScreen()), this, SLOT(slot_fullScreen()));
    connect(this, SIGNAL(sig_subWindow()), this, SLOT(slot_subWindow()));
    //widget_ext->show();
    //widget_ext = new QOpenGLVideoRenderer();
    //widget_ext->show();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *mouseEvent){

    gIndex = ~gIndex;//每次翻转一次
    if( mouseEvent->buttons() == Qt::LeftButton){ //判断是否左键双击
    if(gIndex & 0x1){
        emit sig_fullScreen(); //发送全屏信号
    }
    else{
        emit sig_subWindow();} //退出全屏信号
   }
}

void MainWindow::slot_fullScreen(){   //全屏

    MainWindow::showFullScreen();
}

void MainWindow::slot_subWindow(){  //正常屏

    MainWindow::showNormal();
}

void MainWindow::paintEvent(QPaintEvent *event){  //绘制
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

    if (mImage.size().width() <= 0) return;

    //将图像按比例缩放成和窗口一样大小
    QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x,y),img); //画出图像

}

void MainWindow::slotGetOneFrame(QImage img){
    mImage = img;
    update(); //调用update将执行 paintEvent函数
    if(widget_ext != NULL)
        widget_ext->updateFrame(mImage);   //将QImage传递给另一个屏幕

}

void MainWindow::slotTotalTimeChanged(qint64 uSec){       //视频的总时间
    qint64 Sec = uSec/1000000;

    ui->horizontalSlider->setRange(0,Sec);

    //时分秒，小时未加
//  QString hStr = QString("00%1").arg(Sec/3600);
    QString mStr = QString("00%1").arg(Sec/60);
    QString sStr = QString("00%1").arg(Sec%60);

    QString str = QString("%1:%2").arg(mStr.right(2)).arg(sStr.right(2));
    ui->label_totaltime->setText(str);

}

void MainWindow::slotSliderMoved(int value){      //slider变化
    if (QObject::sender() == ui->horizontalSlider){
        mPlayer->seek((qint64)value * 1000000);
    }
}

void MainWindow::slotTimerTimeOut(){      //当前视频的正在播放的时间
    if (QObject::sender() == mTimer){

        qint64 Sec = mPlayer->getCurrentTime();
        blockSignals(true);
        ui->horizontalSlider->setValue(Sec);
        blockSignals(false);
        //时分秒，只显示了分秒
    //  QString hStr = QString("00%1").arg(Sec/3600);
        QString mStr = QString("00%1").arg(Sec/60);
        QString sStr = QString("00%1").arg(Sec%60);

        QString str = QString("%1:%2").arg(mStr.right(2)).arg(sStr.right(2));
        ui->label_currenttime->setText(str);        //显示时间的label
    }
}

void MainWindow::slotBtnClick(){         //按钮监听事件
    if (QObject::sender() == ui->pushButton_play){   //播放按钮
        mPlayer->play();
    }
    else if (QObject::sender() == ui->pushButton_pause){  //暂停按钮
        mPlayer->pause();
    }
    else if (QObject::sender() == ui->pushButton_two){    //显示另一个屏播放按钮
        if(widget_ext != NULL)
            widget_ext->show();
    }
    else if (QObject::sender() == ui->pushButton_stop){   //停止播放按钮
        mPlayer->stop(true);
    }
    else if (QObject::sender() == ui->pushButton_open){    //打开文件按钮
        QString s=QFileDialog::getOpenFileName(this,tr("open the movie"),"/",tr("mov files(*.mov *.flv *.rmvb *.mp4 *.avi);;all files(*)"));
        if (!s.isEmpty()){
            s.replace("/","\\");
            mPlayer->stop(true);      //如果在播放则先停止
            mPlayer->setFileName(s);
            mTimer->start();
        }
    }
}


void MainWindow::slotStateChanged(VideoThread::PlayerState mPlayState){   //播放状态改变时槽，可以加一些变化隐藏等动作

}
