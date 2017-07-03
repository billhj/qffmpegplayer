#ifndef QTAUDIOPLAYER_H
#define QTAUDIOPLAYER_H

#include <QAudioOutput>

class QtAudioPlayer
{
public:
    QtAudioPlayer();
    void write(const char *data);
    void start(const QAudioFormat&);
    void stop();

    QAudioOutput* getQAudioOutput(){return pOutput;}
    QIODevice* getQIODevice(){return pDevice;}
    QAudioFormat& getQAudioFormat(){return format;}

protected:
    void init();
private:
    QAudioOutput* pOutput;
    QIODevice* pDevice;
    QAudioFormat format;
};

#endif // QTAUDIOPLAYER_H
