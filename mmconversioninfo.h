#ifndef MMCONVERSIONINFO_H
#define MMCONVERSIONINFO_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>


/*
#define PRESET_TYPE(X)                                                  \
            X(0, ultrafast, "ultrafast","Small Files, crap quality"),                           \
            X(1, superfast, "superfast","Small Files, Better than crap quiality"),                              \
            X(2, veryfast, "veryfast","Nice small files, yet quality is much better"),                                    \
            X(3, faster, "faster","Fast is relative, but quaility is much better"),                        \
            X(4, fast, "fast","Fast seems to be the best between size and quaility"),                          \
            X(5, medium, "medium","This is the default, about 1.5X encoding speed"),                    \
            X(6, slow, "slow","Files getting larger, quality gains smaller"),                    \
            X(7, slower, "slower","Larger files, slower time, quaility gains almost gone"),                   \
            X(8, veryslow, "veryslow","Very Large files, near lossless, Very Slow, forget about noticing the diff"),               \
            X(9, placebo, "placebo","I dont even know what to say about this setting")               \
*/

#define PRESET_TYPE(X)                                                  \
            X(0, none, "none","No preset is the same as medium."),                          \
            X(1, fast, "fast","Fast seems to be the best between size and quaility"),                          \
            X(2, medium, "medium","This is the default, about 1.5X encoding speed"),                    \
            X(3, slow, "slow","Files getting larger, quality gains smaller")                    \

#define PRESET_TYPE_ENUM(type, name, str, description) name = type
#define PRESET_TYPE_STRING(type, name, str, description) str
#define PRESET_TYPE_DESC(type, name, str, description) description

typedef enum {
    PRESET_TYPE(PRESET_TYPE_ENUM),
    PRESET_TYPE_ENUM_COUNT
} PresetType;



class MMConversionInfo : public QObject
{

    Q_OBJECT
public:

    explicit MMConversionInfo(QObject *parent = nullptr);
    explicit MMConversionInfo(const MMConversionInfo& mmInfo, QObject *parent = nullptr);

    void setConversionInfo(const MMConversionInfo& mmInfo);
    static QStringList getPresets();


    QString InFile() const;
    void setFullFile(const QString &InFile);

    QString OutDir() const;
    void setOutDir(const QString &OutDir);

    QString OutFile() const;
    void setOutFile(const QString &OutFile);

    QString videoCodex() const;
    void setVideoCodex(const QString &videoCodex);

    QString audioCodex() const;
    void setAudioCodex(const QString &audioCodex);

    int maxStreamSize() const;
    void setMaxStreamSize(int maxStreamSize);

    void clear();

    QString container() const;
    void setContainer(const QString &container);

    int numStreams() const;


    QStringList streamsList() const;
    void insertStreamItem(const QString& streamInfo);


    QString inFullFileName() const;


    QString fileName() const;

    bool isRemoved() const;
    void setIsRemoved(bool isRemoved);

    int64_t dbRecId() const;
    void setDbRecId(const int64_t &dbRecId);

    bool isProcessed() const;
    void setIsProcessed(bool isProcessed);

    int secProcessing() const;
    void setSecProcessing(int secProcessing);

    bool deleteSource() const;
    void setDeleteSource(bool deleteSource);

    QString ffmpgCmd() const;
    void setFfmpgCmd(const QString &ffmpgCmd);

    bool useCustomSettings() const;
    void setUseCustomSettings(bool useCustomSettings);

    QString preset() const;
    void setPreset(const QString &preset);

    int crf() const;
    void setCrf(int crf);

    bool stripSubs() const;
    void setStripSubs(bool stripSubs);

    QStringList getStreamMetaKeys();

    QString getFormatMetaInfo() const;
    void setFormatMetaInfo(QString streamsInfo);

    QString getStreamsMetaInfo() const;
    void setStreamsMetaInfo(QString streamsInfo);

    bool conversionError() const;
    void setConversionError(bool conversionError);

    int conversionErrorCode() const;
    void setConversionErrorCode(int conversionErrorCode);

    unsigned long getFormatDuration() const;
    void setFormatDuration(unsigned long formatDuration);

    unsigned int processedBytes() const;
    void setProcessedBytes(unsigned int processedBytes);

    QMap<QString, QString> fmtMeta() const;
    void setFmtMeta(const QMap<QString, QString> &fmtMeta);

    QMap<QString, QMap<QString, QString>> streamMeta() const;
    void setStreamMeta(const QMap<QString, QMap<QString, QString>> &streamMeta);

    void addFmtMeta(const QString& key, const QString& value);
    void addStreamMeta(const QString& streamName, const QString& key, const QString& value);


    unsigned long getCurrentProgressValue() const;
    void setCurrentProgressValue(unsigned long currentProgressValue);

    bool getProcess() const;
    void setProcess(bool process);

    long getStartFileSize() const;
    void setStartFileSize(long startFileSize);

    long getEndFileSize() const;

    void setEndFileSize(long endFileSize);


    QString getFileName() const;
    void setFileName(const QString &infile);

    QString getFilePath() const;
    void setfilePath(const QString &inFileDir);

private:

    QString _fileName;
    QString _filePath;
    QString _outDir;
    QString _outFile;
    QStringList _streamsList;
    QString _container;
    QString _videoCodex;
    QString _audioCodex;
    int _maxStreamSize;
    bool _isRemoved;
    bool _isProcessed;
    bool _deleteSource;

    int _secProcessing;

    int64_t _dbRecId;

    QString _ffmpgCmd;
    bool _usrCustomSettings;
    QString _preset;
    bool _stripSubs;

    int _crf;
    bool _conversionError;
    int _conversionErrorCode;
    unsigned long  _formatDuration;
    unsigned long  _processedBytes;
    QMap<QString, QString> _fmtMeta;
    QMap<QString, QMap<QString,QString>> _streamMeta;
    unsigned long _currentProgressValue;

    bool _process;

    long _startFileSize;
    long _endFileSize;


signals:


};

#endif // MMCONVERSIONINFO_H
