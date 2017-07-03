#include "qtvideoplayer.h"
QtVideoPlayer::QtVideoPlayer() : QWidget()
{

}

void QtVideoPlayer::updateImage(const QImage& image)
{
    renderImage = image;
    this->update();
}

void QtVideoPlayer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height());
    if (renderImage.size().width() <= 0) return;

    QImage img = renderImage.scaled(this->size(),Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 8;
    y /= 2;
    painter.drawImage(QPoint(x,y),img); //画出图像
}
