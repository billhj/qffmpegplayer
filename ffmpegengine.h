#ifndef FFMPEGENGINE_H
#define FFMPEGENGINE_H
#include <string>
#include "qtaudioplayer.h"
#include "qtvideoplayer.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
//#include "SDL/SDL.h"
};

class FFMpegEngine
{
public:
    FFMpegEngine();
    void bindOutputPlayer(QtAudioPlayer* audioplayer, QtVideoPlayer* videoplayer);
    int open(const QString& file);
    int readFrame();
    AVFrame * getCurrentFrame(){return pFrameRGB;}
    void close();
protected:
    void init();

private:
    AVFormatContext *pFormatCtx;
    int videoStream;
    int audioStream;
    AVCodecContext  *pVideoCodecCtx;
    AVCodec         *pVideoCodec;
    AVCodecContext  *pAudioCodecCtx;
    AVCodec         *pAudioCodec;
    AVFrame         *pFrame;
    AVFrame         *pFrameRGB;
    AVPacket        packet;
    int             frameFinished;
    int             numBytes;
    uint8_t         *buffer;
    SwsContext *sws_ctx;
    QtAudioPlayer *pAudioPlayer;
    QtVideoPlayer *pVideoPlayer;
};

#endif // FFMPEGENGINE_H
