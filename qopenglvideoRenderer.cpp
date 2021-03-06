/**
* Copyright(C) 2009-2012
* @author Jing HUANG   matrixvis.cn
* @file QOpenGLVideoRenderer.cpp
* @brief
* @date 1/2/2017
*/
#include "qopenglvideorenderer.h"

#include <windows.h>
#include "gl/GL.h"
#include "gl/GLU.h"
#include "gl/glut.h"
#include <QOpenGLTexture>
#include <QKeyEvent>
#include <iostream>
#include <QDir>
#include <QDebug>

#define printOpenGLError() printOglError(__FILE__, __LINE__)

/// Returns 1 if an OpenGL error occurred, 0 otherwise.
static int printOglError (const char * file, int line) {
  GLenum glErr;
  int    retCode = 0;
  glErr = glGetError ();
  while (glErr != GL_NO_ERROR) {
    printf ("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
    retCode = 1;
    glErr = glGetError ();
  }
  return retCode;
}

QOpenGLVideoRenderer::QOpenGLVideoRenderer(QWidget* parent) : QOpenGLWidget(parent)
{
    eye =  0.37;
    mode = 0;
	installEventFilter(this);
	setMouseTracking(true);
    shader = NULL;
    tx = NULL;
}

QOpenGLVideoRenderer::~QOpenGLVideoRenderer()
{
    if(shader!=NULL)
        delete shader;
    if(tx!=NULL)
        delete tx;
}

/**
* opengl init part
*/
void QOpenGLVideoRenderer::initializeGL()
{
    QString path = QDir::currentPath();
    glewInit();
    char *myargv [1];
    int myargc=1;
    myargv [0]=strdup ("Myappname");
    glutInit(&myargc, myargv);
    printOpenGLError();
    initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 0, 1.0);
    shader = new Shader();
    std::string currentpath = path.toStdString();
    shader->loadFromFile(currentpath+"/Shader.vert",currentpath+"/Shader.frag");
	tx = new QOpenGLTexture(QOpenGLTexture::Target2D);
	
}

/**
* draw primary
*/
void QOpenGLVideoRenderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    shader->bind();
	GLint location1 = shader->getUniLocation("eyeseparation");
	glUniform1f(location1, eye);
    GLint location2 = shader->getUniLocation("mode");
    glUniform1i(location2, mode);
    //qDebug()<<eye;
	GLint location = shader->getUniLocation("tex");
	if (location != -1)
	{
		glUniform1i(location, 0);
		glActiveTexture(GL_TEXTURE0);
		if(tx->isCreated())
		tx->bind();
	}


    glPushMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport( 0, 0, this->width(), this->height() );
    glOrtho(-1, 1, -1, 1, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);
    glVertex3d(1.0, -1.0, -1);
    glTexCoord2d(0,1);
    glVertex3d(-1.0, -1.0, -1);
    glTexCoord2d(0,0);
    glVertex3d(-1.0,  1.0, -1);
    glTexCoord2d(1,0);
    glVertex3d(1.0,  1.0, -1);
    glEnd();

    glPopMatrix();
    shader->unbind();

}

/**
* update texture
*/
void QOpenGLVideoRenderer::updateFrame(const QImage& img)
{
    tx->destroy();
    tx->create();
	tx->setData(img, QOpenGLTexture::DontGenerateMipMaps);
	update();
}

/**
* eye distance setup configuration for different person
*/
void QOpenGLVideoRenderer::setEyeSeparation(float v)
{
	eye = v;
}

/**
* modify eye distance for different eye fusion for different person
***/
void QOpenGLVideoRenderer::modifyEyeSeparation(float diff)
{
    eye += diff;
    qDebug()<<eye;
}


void QOpenGLVideoRenderer::resizeGL(int w, int h)
{

}

void QOpenGLVideoRenderer::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (this->isFullScreen() == false) {
		this->showFullScreen();
	}
	else 
	{
		this->showNormal();
	}
}

void QOpenGLVideoRenderer::mouseMoveEvent(QMouseEvent *event)
{
	//controller->setVisible(true);
}

void QOpenGLVideoRenderer::mouseReleaseEvent(QMouseEvent *event)
{

}

bool QOpenGLVideoRenderer::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_1)
		{
			eye = eye + 0.01;
			//qDebug() << "Up";
		}
			
		if (keyEvent->key() == Qt::Key_2)
		{
			eye = eye - 0.01;
			//qDebug() << "Down";
		}
			
	}
	return QObject::eventFilter(obj, event);
}

void QOpenGLVideoRenderer::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		std::cout << event->key() << std::endl;
	case Qt::Key_Escape:
		close();
		break;
	case Qt::UpArrow:
		eye = eye + 0.1;
		break;
	case Qt::DownArrow:
		eye = eye - 0.1;
		break;
	default:
		break;
	}

	event->accept();
}

void QOpenGLVideoRenderer::changeVRMode()
{
    if(mode == 0)
    {
       mode = 1;
       qDebug()<<"leftright";
    }
    else
    {
       mode = 0;
       qDebug()<<"updown";
    }
}
