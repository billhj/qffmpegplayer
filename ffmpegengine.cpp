#include "ffmpegengine.h"
#include "QtAudioPlayer.h"
#include <QDebug>

FFMpegEngine::FFMpegEngine()
{
    pFormatCtx = NULL;
    videoStream = -1;
    audioStream = -1;
    pVideoCodecCtx = NULL;
    pVideoCodec = NULL;
    pFrame = NULL;
    pFrameRGB = NULL;

    frameFinished = 1;
    buffer = NULL;
    sws_ctx = NULL;
    pAudioPlayer = NULL;
    pVideoPlayer = NULL;
    init();
}


void FFMpegEngine::init()
{
    av_register_all();
}

void FFMpegEngine::bindOutputPlayer(QtAudioPlayer* audioplayer, QtVideoPlayer* videoplayer)
{
    pAudioPlayer = audioplayer;
    pVideoPlayer = videoplayer;
}

int FFMpegEngine::open(const QString& file)
{
    //pFormatCtx = avformat_alloc_context();
    //get all info in the file
    if( avformat_open_input(&pFormatCtx, file.toStdString().c_str(), NULL, NULL) != 0 )
    {
        printf("can't open the file. \n");
        return -1;
    }
    if( avformat_find_stream_info(pFormatCtx, NULL ) < 0 )
    {
        printf("can't avformat_find_stream_info. \n");
        return -1;
    }

    av_dump_format(pFormatCtx, -1, file.toStdString().c_str(), 0);

    //////////////////////////////////////////////
    // find stream
    videoStream = -1;
    audioStream = -1;
    for(unsigned int i = 0; i < pFormatCtx->nb_streams; i++ )
    {
        if( pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
        if(pFormatCtx->streams[i]->codec->codec_type ==AVMEDIA_TYPE_AUDIO) {
            audioStream = i;
        }
    }

    if( videoStream != -1 )
    {
        ///////////////////////////////////////////
        //getcodec  context codec
        pVideoCodecCtx = pFormatCtx->streams[videoStream]->codec;
        pVideoCodec = avcodec_find_decoder(pVideoCodecCtx->codec_id);
        if( pVideoCodec == NULL )
        {
            printf("can't find videoCodec. \n");
            return -1;
        }

        if( avcodec_open2(pVideoCodecCtx, pVideoCodec, NULL) < 0 )
        {
            printf("can't open videoCodec. \n");
            return -1;
        }


        ////////////////////////////////////////////
        //init frames
        pFrame = av_frame_alloc();
        if( pFrame == NULL )
        {
            printf("can't alloc pFrame. \n");
            return -1;
        }

        pFrameRGB = av_frame_alloc();
        if( pFrameRGB == NULL )
        {
            printf("can't alloc pRGBFrame. \n");
            return -1;
        }

        numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pVideoCodecCtx->width,
                                      pVideoCodecCtx->height);

        buffer = (uint8_t*)av_malloc(numBytes);

        avpicture_fill( (AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24,
                        pVideoCodecCtx->width, pVideoCodecCtx->height);


        // initialize SWS context for software scaling
        sws_ctx = sws_getContext(pVideoCodecCtx->width,
                                 pVideoCodecCtx->height,
                                 pVideoCodecCtx->pix_fmt,
                                 pVideoCodecCtx->width,
                                 pVideoCodecCtx->height,
                                 AV_PIX_FMT_RGB24,
                                 SWS_BILINEAR,
                                 NULL,
                                 NULL,
                                 NULL
                                 );

    }
    if(audioStream != -1 )
    {
        ///////////////////////////////////////////
        //getcodec  context codec
        pAudioCodecCtx = pFormatCtx->streams[audioStream]->codec;
        pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
        if( pAudioCodec == NULL )
        {
            printf("can't find audioCodec. \n");
            return -1;
        }

        if( avcodec_open2(pAudioCodecCtx, pAudioCodec, NULL) < 0 )
        {
            printf("can't open audioCodec. \n");
            return -1;
        }
        if(pAudioPlayer != NULL)
        {
            QAudioFormat format;
            format.setSampleRate(pAudioCodecCtx->sample_rate);
            format.setChannelCount(pAudioCodecCtx->channels);
            format.setSampleSize(16);
            format.setCodec("audio/pcm");
            format.setByteOrder(QAudioFormat::LittleEndian);
            format.setSampleType(QAudioFormat::SignedInt);

            QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
            if (!info.isFormatSupported(format)) {
                qWarning()<<"raw audio format not supported by backend, cannot play audio.";
                format = info.nearestFormat(format);
            }
            pAudioPlayer->start(format);
        }

    }
    printf("deCoder ready. \n");
    return 1;
}

void FFMpegEngine::close()
{
    if(buffer != NULL)
        av_free(buffer);
    if(pFrameRGB != NULL)
        av_free(pFrameRGB);
    if(pFrame != NULL)
        av_free(pFrame);
    avcodec_close(pVideoCodecCtx);
    avformat_close_input(&pFormatCtx);
}

int FFMpegEngine::readFrame()
{
    int re = av_read_frame(pFormatCtx, &packet);
    if(re >= 0) {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStream)
        {
            // Decode video frame
            avcodec_decode_video2(pVideoCodecCtx, pFrame, &frameFinished, &packet);
            // Did we get a video frame?
            if(frameFinished)
            {
                // Convert the image from its native format to RGB
                sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                          pFrame->linesize, 0, pVideoCodecCtx->height,
                          pFrameRGB->data, pFrameRGB->linesize);

                // outputframe

                if(pVideoPlayer != NULL)
                {
                    QImage tmpImg((uchar *)buffer,pVideoCodecCtx->width,pVideoCodecCtx->height,QImage::Format_RGB32);
                    QImage image = tmpImg.copy();     //把图像复制一份 传递给界面显示
                    pVideoPlayer->updateImage(image);
                }
            }
        }

        if(packet.stream_index==audioStream)
        {


        }

        // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);
    }
    return re;
}
