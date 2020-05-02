#include "ffmpeginvoker.h"
#include <iomanip>
#include <QDebug>
#include <QFileInfo>

extern "C"
{
    #include "libavutil/opt.h"
    #include "libavformat/avformat.h"

}


ffMpegInvoker::ffMpegInvoker(QObject *parent) : QObject(parent)
{
    _mmFpeg = new QProcess(this);
    _mmFpeg->setProcessChannelMode(QProcess::MergedChannels);
    av_register_all();
    avcodec_register_all();

    connect(_mmFpeg , SIGNAL(started()), this, SLOT(processStarted()));
    connect(_mmFpeg ,SIGNAL(readyReadStandardOutput()),this,SLOT(readyReadStandardOutput()));
    connect(_mmFpeg ,SIGNAL(readyRead()),this,SLOT(readyReadStandardOutput()));
    connect(_mmFpeg ,SIGNAL(readyReadStandardError()),this,SLOT(readyReadStandardOutput()));
    connect(_mmFpeg , SIGNAL(finished(int)), this, SLOT(encodingFinished()));
    _lastDiff = 0;
}

ffMpegInvoker::~ffMpegInvoker()
{
    _mmFpeg->kill();
    delete _mmFpeg;
}

void ffMpegInvoker::setMmInfo(MMConversionInfo *mmInfo)
{
    _lastDiff = 0;
    _mmInfo = mmInfo;
    _outFileName.str("");
    _outFileName << _mmInfo->OutDir().toStdString().c_str() << "/" <<
        _mmInfo->fileName().toStdString().c_str();
}

void ffMpegInvoker::start()
{
    if(nullptr != _mmInfo)
        conversion();
}

void ffMpegInvoker::waitForCompletion()
{
    _mmFpeg->waitForFinished();
}


void ffMpegInvoker::start(MMConversionInfo *mmInfo)
{
    _mmInfo = mmInfo;
    setMmInfo(mmInfo);
    start();
}


void ffMpegInvoker::readyReadStandardOutput()
{


    int frames = 0;
    Q_UNUSED(frames);

    double fps = 0;
    double q = 0;
    double size = 0;
    QString time;
    QString bitRate;
    QString speed;
    QString strTime;
    QString value;

    std::stringstream msg;

     QString bytes = _mmFpeg->readAll();
     if(bytes.length() > 0)
     {
        bytes.replace("=  ","=");
        bytes.replace("= ","=");
        QStringList slist= bytes.split(' ', QString::SkipEmptyParts);
        if(slist.length() == 8)
        {

            for(int counter = 0; counter < slist.length(); counter++)
            {
                value = slist[counter];
                if(value.split('=').length()  == 1) break;
                value = slist[counter].split('=')[1];

                switch(counter)
                {
                case 0:
                    frames = value.toInt();
                    break;

                case 1:
                    fps = value.toDouble();
                    break;

                case 2:
                    q = value.toDouble();
                    break;

                case 3:
                    size = value.split('k')[0].toDouble();
                    _mmInfo->setProcessedBytes(size);
                    break;

                case 4:
                    time = value;
                    strTime = time;
                    break;

                case 5:
                    bitRate = value;
                    break;

                case 6:
                    speed = value;
                    break;

                }
            }


            strTime.replace('.',":");
            QStringList durrationArray = strTime.split(':');

            unsigned long nduration = 0;
            for(int counter = 0; counter < durrationArray.length(); counter++)
            {
                QString du = durrationArray[counter];
                switch(counter)
                {

                case 0:
                        nduration += (du.toInt() * 3600000);
                    break;

                case 1:
                        nduration += (du.toInt() * 60000);
                    break;

                case 2:
                        nduration += du.toInt() * 1000;
                    break;


                }

            }

            _mmInfo->setCurrentProgressValue(nduration);
            msg << "Transcoding Time: " << time.toStdString().c_str() << " Speed: " << speed.toStdString().c_str() << " FPS: " << fps
                << " q: " << q << " File: ";

            processUpdate(msg.str().c_str());
        }
     }
     else
     {
         if(bytes.length() > 0)
            qDebug() << bytes;
     }




}


void ffMpegInvoker::processUpdate(QString statusSource)
{
    std::stringstream statusMsg;
    int sec = 0;
    double diff = 0;
    int min = 0;

    time(&_end);
    diff = difftime(_end, _start);
    _mmInfo->setSecProcessing(diff);
    min = (int)diff / 60;
    sec = (int)diff % 60;
    if(diff > _lastDiff)
    {
        statusMsg << statusSource.toStdString().c_str();
        statusMsg  << " " << _mmInfo->fileName().toStdString().c_str() <<  " Elapsed Time " << std::setfill('0') << std::setw(2) << min << " Minutes And " <<
              std::setfill('0') << std::setw(2) << sec <<" Seconds.\r";

        emit processUpdate(QString(statusMsg.str().c_str()), _mmInfo);
        _lastDiff = diff;
    }
}

void ffMpegInvoker::processStarted()
{

}


void ffMpegInvoker::encodingFinished()
{

    QFileInfo fileInfo(_outFileName.str().c_str());
    _mmInfo->setEndFileSize(fileInfo.size());
    _mmInfo->setIsProcessed(true);


    processUpdate("Processing Complete:");
    emit processComplete(_mmInfo);

}




void ffMpegInvoker::conversion()
{

    std::stringstream mmCmd;

    mmCmd << "ffmpeg -y";

    if(_mmInfo->container().contains("matroska,webm") && _mmInfo->useCustomSettings())
        mmCmd << " -hwaccel cuvid -c:v h264_cuvid";

    if(_mmInfo->useCustomSettings())
        mmCmd << " -vsync 0";

    mmCmd << " -i \"" << _mmInfo->inFullFileName().toStdString().c_str() <<"\"";

    if(_mmInfo->stripSubs())
        mmCmd << " -sn";

    if(_mmInfo->useCustomSettings())
    {
        mmCmd << " -c:a copy -c:v h264_nvenc -crf " << _mmInfo->crf();

        if (_mmInfo->crf() == 0)
            mmCmd << " -preset veryslow";
        else
            mmCmd << " -preset " << _mmInfo->preset().toStdString().c_str();

        mmCmd << " -b:v 5M";
    }
    else
    {
        mmCmd << " -c:v copy -c:a copy";
    }

    mmCmd << " \"" << _outFileName.str().c_str() << "\"";

    _mmInfo->setFfmpgCmd(mmCmd.str().c_str());
    time(&_start);
    _mmFpeg->start(mmCmd.str().c_str());

    _mmFpeg->waitForStarted(-1);

}


void ffMpegInvoker::probeMM(MMConversionInfo *mmInfo)
{

    AVFormatContext *ifmt_ctx = NULL;
    std::stringstream streamInfo;
    int ret;

    av_register_all();
    avcodec_register_all();

    if ((ret = avformat_open_input(&ifmt_ctx,  mmInfo->inFullFileName().toStdString().c_str(), 0, 0)) < 0)
    {
        fprintf(stderr, "Could not open input file '%s'",  mmInfo->inFullFileName().toStdString().c_str());
        return;
    }

    QFileInfo fileInfo(mmInfo->inFullFileName().toStdString().c_str());
    mmInfo->setStartFileSize(fileInfo.size());


    AVDictionaryEntry *dicentry = NULL;
    while ( (dicentry = av_dict_get(ifmt_ctx->metadata, "", dicentry, AV_DICT_IGNORE_SUFFIX) ))
    {
        mmInfo->addFmtMeta(dicentry->key, dicentry->value);
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) == 0)
    {

        mmInfo->setContainer(ifmt_ctx->iformat->name);
        mmInfo->addFmtMeta("probeScore", QString::number(ifmt_ctx->probe_score));
        mmInfo->addFmtMeta("extensions", ifmt_ctx->iformat->extensions);
        mmInfo->addFmtMeta("longname", ifmt_ctx->iformat->long_name);
        mmInfo->setFormatDuration((ifmt_ctx->duration <= 0) ? 1 : ifmt_ctx->duration / 1000);

        for (unsigned int counter = 0; counter < ifmt_ctx->nb_streams; counter++)
        {

            AVStream *in_stream = ifmt_ctx->streams[counter];
            AVCodecParameters *in_codecpar = in_stream->codecpar;

            if (in_codecpar->codec_type == AVMEDIA_TYPE_AUDIO || in_codecpar->codec_type == AVMEDIA_TYPE_VIDEO ||
                in_codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE)
            {

               char codexName[1024] = {};

               char profile[50] = {};
               const char* cname = avcodec_profile_name(in_stream->codecpar->codec_id, 0);
               if(!cname)
                   cname = "";

               strcpy(profile,cname);
               avcodec_string(codexName, sizeof(codexName), in_stream->codec, 0);

               streamInfo.str("");
               streamInfo << "Index[" << counter << "] Name[" << codexName << "] Profile[" << profile << "]";

               mmInfo->addStreamMeta(streamInfo.str().c_str(), "index", QString::number(counter));
               mmInfo->addStreamMeta(streamInfo.str().c_str(), "codexName", codexName);
               mmInfo->addStreamMeta(streamInfo.str().c_str(), "profile", profile);
               mmInfo->addStreamMeta(streamInfo.str().c_str(), "codecType", QString::number(in_codecpar->codec_type));

               while ( (dicentry = av_dict_get(in_stream->metadata, "", dicentry, AV_DICT_IGNORE_SUFFIX)) )
               {
                   mmInfo->addStreamMeta(streamInfo.str().c_str(), dicentry->key, dicentry->value);
               }

            }
        }


    }
    else
    {
        //error condition report
    }

    avformat_close_input(&ifmt_ctx);

}
