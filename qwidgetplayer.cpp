#include "QWidgetPlayer.h"

QWidgetPlayer::QWidgetPlayer(){}

QWidgetPlayer::~QWidgetPlayer(){}

void QWidgetPlayer::updateFrame(const QImage& imga){      //接受视频解析出的图片
    qqImage=imga;
    update();
}

void QWidgetPlayer::paintEvent(QPaintEvent *event){    //绘制函数

    QPainter painter(this); 
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色
    if (qqImage.size().width() <= 0) return;
    //将图像按比例缩放成和窗口一样大小
    QImage img = qqImage.scaled(this->size(),Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

     x /= 8;
     y /= 2;
    painter.drawImage(QPoint(x,y),img); //画出图像
}

