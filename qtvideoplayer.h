#ifndef QTVIDEOPLAYER_H
#define QTVIDEOPLAYER_H
/**
* Copyright(C) 2009-2012
* @author Jing HUANG   matrixvis.cn
* @file QtVideoPlayer.h
* @brief videoplayer ui
* @date 1/2/2017
*/
#include <QWidget>
#include <QPainter>
#include <QImage>
#include "qevent.h"

class QtVideoPlayer : public QWidget
{
    Q_OBJECT
public:
    QtVideoPlayer();
    void updateImage(const QImage& img);
    void paintEvent(QPaintEvent *event);
protected:
    QImage renderImage;
};

#endif // QTVIDEOPLAYER_H
