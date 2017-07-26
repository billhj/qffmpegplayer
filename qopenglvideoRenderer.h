#ifndef QOPENGLVIDEOPLAYER_H
#define QOPENGLVIDEOPLAYER_H
/**
* Copyright(C) 2009-2012
* @author Jing HUANG   matrixvis.cn
* @file QOpenGLVideoRenderer.h
* @brief  the opengl video player using glsl/ which has the vr setup
* @date 1/2/2017
*/
#include "Shader.h"
#include <QOpenGLWidget>
#include <QVideoFrame>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>

class QOpenGLVideoRenderer : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    QOpenGLVideoRenderer(QWidget* parent = NULL);
    ~QOpenGLVideoRenderer();

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void updateFrame(const QImage& img);
	void setEyeSeparation(float v);
    void modifyEyeSeparation(float diff);
    void changeVRMode();
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
    Shader* shader;
	QOpenGLTexture* tx;
	float eye;
    int mode;
};

#endif // QOPENGLVIDEOPLAYER_H
