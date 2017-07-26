/**
* Copyright(C) 2009-2012
* @author Jing HUANG   matrixvis.cn
* @file QtAudioPlayer.cpp
* @brief
* @date 1/2/2017
*/
#include "qtaudioplayer.h"

QtAudioPlayer::QtAudioPlayer()
{
}


void QtAudioPlayer::start(const QAudioFormat& formati)
{
    format = formati;
    pOutput = new QAudioOutput(format);
    pDevice = pOutput->start();
}

void QtAudioPlayer::write(const char *data)
{
    pDevice->write(data);
}

void QtAudioPlayer::stop()
{
    pOutput->stop();
}
