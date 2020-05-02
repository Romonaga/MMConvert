#ifndef FFMPEGINVOKER_H
#define FFMPEGINVOKER_H

#include <QObject>
#include <QProcess>

#include <sstream>
#include "mmconversioninfo.h"


class ffMpegInvoker : public QObject
{
    Q_OBJECT
public:
    explicit ffMpegInvoker(QObject *parent = nullptr);
    ~ffMpegInvoker();
    void setMmInfo(MMConversionInfo *mmInfo);
    void start(MMConversionInfo* mmInfo);
    void start();
    void waitForCompletion();
    static void probeMM(MMConversionInfo* mmInfo);

private:
    void conversion();
    void processUpdate(QString statusSource);

private:
    QProcess*   _mmFpeg;
    time_t _start;
    time_t _end;
    std::stringstream _outFileName;
    double _lastDiff;
    MMConversionInfo* _mmInfo;



signals:
    void processUpdate(QString, MMConversionInfo*);					//SIGNAL to show that some process have been done
    void processComplete(MMConversionInfo*);


private slots:
    void readyReadStandardOutput();
    void processStarted();
    void encodingFinished();

};

#endif // FFMPEGINVOKER_H
