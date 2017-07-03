#ifndef QTVIDEOPLAYER_H
#define QTVIDEOPLAYER_H

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
