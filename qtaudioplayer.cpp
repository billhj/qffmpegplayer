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
