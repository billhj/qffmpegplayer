#ifndef QWidgetPlayer_H
#define QWidgetPlayer_H
/**
* Copyright(C) 2009-2012
* @author Jing HUANG   matrixvis.cn
* @file QWidgetPlayer.h
* @brief the qt ui for updating video image
* @date 1/2/2017
*/
#include <QPainter>
#include <QImage>
#include "qevent.h"
#include <QWidget>

class QWidgetPlayer : public QWidget
{
     Q_OBJECT

public:
    explicit QWidgetPlayer();
    ~QWidgetPlayer();

    void updateFrame(const QImage& imga);      //接受视频解析出的图片

    void paintEvent(QPaintEvent *event);      //绘制函数
private:
     QImage qqImage;

};

#endif // QWidgetPlayer_H
