#ifndef MMDirWatcher_H
#define MMDirWatcher_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QFileSystemWatcher>




class MMDirWatcher : public QThread
{

    Q_OBJECT
    Q_ENUMS(FileStatus)

public:
    enum  FileStatus{Added, Changed, Deleted};


    explicit MMDirWatcher(const QString& path, QObject *parent = nullptr);
    ~MMDirWatcher();
    void stop();


private:
    QString _path;
    QMap<QString, qint64> _fileMap;
    QFileSystemWatcher* _fileWatcher;
    bool _running;


public:

signals:
    void fileChangeNotice(QString, MMDirWatcher::FileStatus, quint64);


private slots:
    void fileChanged(QString fileName);

    // QThread interface
protected:
    void run();
};

Q_DECLARE_METATYPE(MMDirWatcher::FileStatus);

#endif // MMDirWatcher_H
//qRegisterMetaType<MMDirWatcher::FileStatus>();
