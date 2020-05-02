#include "mmconversioninfo.h"
#include "mmconvertcommon.h"
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVariant>
#include <QJsonObject>
#include "QMapIterator"
#include "mmsettings.h"

MMConversionInfo::MMConversionInfo(QObject *parent) : QObject(parent)
{
    clear();
}

MMConversionInfo::MMConversionInfo(const MMConversionInfo &mmInfo, QObject *parent) : QObject(parent)
{
    setConversionInfo(mmInfo);
}


void MMConversionInfo::setConversionInfo(const MMConversionInfo &mmInfo)
{
    setDbRecId(mmInfo.dbRecId());
    setAudioCodex(mmInfo.audioCodex());
    setContainer(mmInfo.container());
    setFileName(mmInfo.getFileName());
    setfilePath(mmInfo.getFilePath());
    setMaxStreamSize(mmInfo.maxStreamSize());
    setOutDir(mmInfo.OutDir());
    setOutFile(mmInfo.OutFile());
    setVideoCodex(mmInfo.videoCodex());
    setIsRemoved(false);
    setDeleteSource(mmInfo.deleteSource());
    setIsProcessed(mmInfo.isProcessed());
    setUseCustomSettings(mmInfo.useCustomSettings());
    setPreset(mmInfo.preset());
    setCrf(mmInfo.crf());
    setStripSubs(mmInfo.stripSubs());
    setStreamsMetaInfo(mmInfo.getStreamsMetaInfo());
    setFormatMetaInfo(mmInfo.getFormatMetaInfo());
    setConversionError(mmInfo.conversionError());
    setConversionErrorCode(mmInfo.conversionErrorCode());
    setFormatDuration(mmInfo.getFormatDuration());
    setProcessedBytes((mmInfo.processedBytes()));
    setProcess(mmInfo.getProcess());
    setStartFileSize(mmInfo.getStartFileSize());
    setEndFileSize(mmInfo.getEndFileSize());
    setCurrentProgressValue(mmInfo.getCurrentProgressValue());


    _streamMeta = mmInfo._streamMeta;

}


int MMConversionInfo::maxStreamSize() const
{
    return _maxStreamSize;
}

void MMConversionInfo::setMaxStreamSize(int maxStreamSize)
{
    _maxStreamSize = maxStreamSize;
}

QString MMConversionInfo::audioCodex() const
{
    return _audioCodex;
}

void MMConversionInfo::setAudioCodex(const QString &audioCodex)
{
    _audioCodex = audioCodex;
}

QString MMConversionInfo::videoCodex() const
{
    return _videoCodex;
}

void MMConversionInfo::setVideoCodex(const QString &videoCodex)
{
    _videoCodex = videoCodex;
}

QString MMConversionInfo::OutFile() const
{
    return _outFile;
}

void MMConversionInfo::setOutFile(const QString &OutFile)
{
    _outFile = OutFile;
}

QString MMConversionInfo::OutDir() const
{
    return _outDir;
}

void MMConversionInfo::setOutDir(const QString &OutDir)
{
    _outDir = OutDir;
}


void MMConversionInfo::setFullFile(const QString &inFile)
{
    QFileInfo fileInfo(inFile);
    _filePath = fileInfo.absolutePath();
    _fileName = fileInfo.fileName();
}


void MMConversionInfo::clear()
{
    _fileName = "";
    _filePath = "";
    _outFile = "";
    _outDir = MMSettings::getInstance()->getOutPath();
    _videoCodex = "";
    _audioCodex = "";
    _container = "";
    _maxStreamSize = 9000;
    _startFileSize = 0;
    _endFileSize = 0;
    _isRemoved = false;
    _isProcessed = false;
    _deleteSource = MMSettings::getInstance()->getDelete();
    _dbRecId = -1;
    _streamsList.clear();
    _secProcessing = 0;
    _ffmpgCmd = "";
    _preset = "medium";
    _crf = MMSettings::getInstance()->getCrf();
    _usrCustomSettings = false;
    _stripSubs = MMSettings::getInstance()->getStripSubs();
    _conversionError = false;
    _conversionErrorCode = 0;
    _formatDuration = 0;
    _processedBytes = 0;
    _fmtMeta.clear();
    _streamMeta.clear();
    _currentProgressValue = 0;
    _process = false;

}

QString MMConversionInfo::container() const
{
    return _container;
}

void MMConversionInfo::setContainer(const QString &container)
{
    _container = container;
}

int MMConversionInfo::numStreams() const
{
    return _streamMeta.keys().count();
}



QStringList MMConversionInfo::streamsList() const
{
    return _streamsList;
}

void MMConversionInfo::insertStreamItem(const QString& streamInfo)
{
    _streamsList.append(streamInfo);

}

bool MMConversionInfo::isRemoved() const
{
    return _isRemoved;
}

void MMConversionInfo::setIsRemoved(bool isRemoved)
{
    _isRemoved = isRemoved;
}

int64_t MMConversionInfo::dbRecId() const
{
    return _dbRecId;
}

void MMConversionInfo::setDbRecId(const int64_t &dbRecId)
{
    _dbRecId = dbRecId;
}

bool MMConversionInfo::isProcessed() const
{
    return _isProcessed;
}

void MMConversionInfo::setIsProcessed(bool isProcessed)
{
    _isProcessed = isProcessed;
}

int MMConversionInfo::secProcessing() const
{
    return _secProcessing;
}

void MMConversionInfo::setSecProcessing(int secProcessing)
{
    _secProcessing = secProcessing;
}

bool MMConversionInfo::deleteSource() const
{
    return _deleteSource;
}

void MMConversionInfo::setDeleteSource(bool deleteSource)
{
    _deleteSource = deleteSource;
}

QString MMConversionInfo::ffmpgCmd() const
{
    return _ffmpgCmd;
}

void MMConversionInfo::setFfmpgCmd(const QString &ffmpgCmd)
{
    _ffmpgCmd = ffmpgCmd;
}

bool MMConversionInfo::useCustomSettings() const
{
    return _usrCustomSettings;
}

void MMConversionInfo::setUseCustomSettings(bool userCustomSettings)
{
    _usrCustomSettings = userCustomSettings;
}

QString MMConversionInfo::preset() const
{
    return _preset;
}

void MMConversionInfo::setPreset(const QString &preset)
{
    _preset = preset;
}

int MMConversionInfo::crf() const
{
    return _crf;
}

void MMConversionInfo::setCrf(int crf)
{
    _crf = crf;
}

bool MMConversionInfo::stripSubs() const
{
    return _stripSubs;
}

void MMConversionInfo::setStripSubs(bool stripSubs)
{
    _stripSubs = stripSubs;
}


QString MMConversionInfo::getStreamsMetaInfo() const
{

    QVariantMap sMap;

    QVariantMap vMap;
    foreach(auto stream, _streamMeta.keys())
    {
        vMap.clear();
        foreach(auto key, _streamMeta[stream].keys())
        {

            QString value = _streamMeta[stream][key];
            vMap.insert(key, value);

        }
        sMap.insert(stream,vMap);
    }

    return QJsonDocument::fromVariant(sMap).toJson(QJsonDocument::Compact);

}

QString MMConversionInfo::getFormatMetaInfo() const
{

    QVariantMap vMap;
    foreach(auto key, _fmtMeta.keys())
    {
        vMap.clear();
        foreach(auto value, _fmtMeta[key])
        {

            vMap.insert(key, value);

        }

    }

    return QJsonDocument::fromVariant(vMap).toJson(QJsonDocument::Compact);

}



void MMConversionInfo::setStreamsMetaInfo(QString streamsInfo)
{

    QJsonParseError parsError;
    QJsonDocument jDoc = QJsonDocument::fromJson(streamsInfo.toUtf8(), &parsError);

    QJsonObject jsonObject = jDoc.object();
    foreach(auto stream, jsonObject.keys())
    {

        QJsonObject metaData = jsonObject[stream].toObject();

        foreach(auto key, metaData.keys())
        {
            QString value =  metaData[key].toString();
            addStreamMeta(stream, key, value);
        }
    }
}


void MMConversionInfo::setFormatMetaInfo(QString streamsInfo)
{

    QJsonParseError parsError;
    QJsonDocument jDoc = QJsonDocument::fromJson(streamsInfo.toUtf8(), &parsError);

    QJsonObject jsonObject = jDoc.object();
    foreach(auto stream, jsonObject.keys())
    {

        QJsonObject metaData = jsonObject[stream].toObject();

        foreach(auto key, metaData.keys())
        {
            QString value =  metaData[key].toString();
            addFmtMeta(key, value);
        }
    }
}



QStringList MMConversionInfo::getStreamMetaKeys()
{
    return QStringList(_streamMeta.keys());

}

bool MMConversionInfo::conversionError() const
{
    return _conversionError;
}

void MMConversionInfo::setConversionError(bool conversionError)
{
    _conversionError = conversionError;
}

int MMConversionInfo::conversionErrorCode() const
{
    return _conversionErrorCode;
}

void MMConversionInfo::setConversionErrorCode(int conversionErrorCode)
{
    _conversionErrorCode = conversionErrorCode;
}

unsigned long MMConversionInfo::getFormatDuration() const
{
    return _formatDuration;
}

void MMConversionInfo::setFormatDuration(unsigned long formatDuration)
{
    _formatDuration = formatDuration;
}

unsigned int MMConversionInfo::processedBytes() const
{
    return _processedBytes;
}

void MMConversionInfo::setProcessedBytes(unsigned int processedBytes)
{
    _processedBytes = processedBytes;
}

QMap<QString, QString> MMConversionInfo::fmtMeta() const
{
    return _fmtMeta;
}

void MMConversionInfo::setFmtMeta(const QMap<QString, QString> &fmtMeta)
{
    _fmtMeta = fmtMeta;
}

QMap<QString, QMap<QString, QString> > MMConversionInfo::streamMeta() const
{
    return _streamMeta;
}

void MMConversionInfo::setStreamMeta(const QMap<QString, QMap<QString, QString>> &streamMeta)
{
    _streamMeta = streamMeta;
}

void MMConversionInfo::addFmtMeta(const QString &key, const QString &value)
{
    _fmtMeta.insert(key, value);
}

void MMConversionInfo::addStreamMeta(const QString &streamName, const QString &key, const QString &value)
{
    QString normKey = key.split('-')[0];
    if(_streamMeta.contains(streamName))
    {
        _streamMeta[streamName].insert(normKey, value);
    }
    else
    {

        QMap<QString, QString> insert;
        insert.insert(normKey, value);
        _streamMeta.insert(streamName, insert);

    }
}


unsigned long MMConversionInfo::getCurrentProgressValue() const
{
    return _currentProgressValue;
}

void MMConversionInfo::setCurrentProgressValue(unsigned long currentProgressValue)
{
    _currentProgressValue = currentProgressValue;
}

bool MMConversionInfo::getProcess() const
{
    return _process;
}

void MMConversionInfo::setProcess(bool process)
{
    _process = process;
}

long MMConversionInfo::getStartFileSize() const
{
    return _startFileSize;
}

void MMConversionInfo::setStartFileSize(long startFileSize)
{
    _startFileSize = startFileSize;
}

long MMConversionInfo::getEndFileSize() const
{
    return _endFileSize;
}

void MMConversionInfo::setEndFileSize(long endFileSize)
{
    _endFileSize = endFileSize;
}

QString MMConversionInfo::getFileName() const
{
    return _fileName;
}

void MMConversionInfo::setFileName(const QString &infile)
{
    _fileName = infile;
}


QString MMConversionInfo::getFilePath() const
{
    return _filePath;
}

void MMConversionInfo::setfilePath(const QString &inFileDir)
{
    _filePath = inFileDir;
}


QString MMConversionInfo::inFullFileName() const
{
    QString ret;
    ret.append(_filePath);
    ret.append("/");
    ret.append(fileName());
    return ret;
}

QString MMConversionInfo::fileName() const
{
    return _fileName;
}

QStringList MMConversionInfo::getPresets()
{
    const char* preSetNames[] = {PRESET_TYPE(PRESET_TYPE_STRING)};
    QStringList presets;

    for(int counter = 0; counter < PRESET_TYPE_ENUM_COUNT; counter++)
        presets.append(preSetNames[counter]);

    return presets;
}
