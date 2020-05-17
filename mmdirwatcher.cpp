#include "mmdirwatcher.h"

#include <QDebug>
#include <QFileInfo>
#include <QDirIterator>
#include <QDateTime>
#include <QStringList>


MMDirWatcher::MMDirWatcher(const QString &path, QObject *parent) : _path(path), QThread(parent)
{
    qRegisterMetaType<MMDirWatcher::FileStatus>("FileStatus");
    _running = false;
    _fileWatcher = new QFileSystemWatcher(this);
    _fileWatcher->startTimer(1000);
    connect(_fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));

}

MMDirWatcher::~MMDirWatcher()
{

}

void MMDirWatcher::stop()
{
    _running = false;
}


void MMDirWatcher::fileChanged(QString fileName)
{
    QFileInfo fInfo(fileName);

    if(fInfo.exists() == false)
    {
        _fileWatcher->removePath(fileName);
        _fileMap.remove(fileName);
        emit fileChangeNotice(fileName, FileStatus::Deleted, 0);
    }
    else
    {
        emit fileChangeNotice(fInfo.absoluteFilePath(), FileStatus::Changed, fInfo.size());
    }
}

void MMDirWatcher::run()
{

    QStringList list;
    list << "*.mkv" << "*.avi" << "*.mov" << "*mp4";// << "*.txt";

    _running = true;
    while(_running)
    {
        QDirIterator dirWalk(_path, list, QDir::Files | QDir::Readable, QDirIterator::NoIteratorFlags);

        while(dirWalk.hasNext())
        {
            QFileInfo file(dirWalk.next());
            if(file.exists() && file.isFile())
            {
                if(_fileMap.contains(file.absoluteFilePath()) == false)
                {
                    _fileMap.insert(file.absoluteFilePath(), file.size());
                    _fileWatcher->addPath(file.absoluteFilePath());
                    emit fileChangeNotice(file.absoluteFilePath(), FileStatus::Added, file.size());
                }
                else
                {
                    quint64 lm = file.size();
                    quint64 st = _fileMap.value(file.absoluteFilePath() ) ;
                    if(lm < st )
                    {
                        _fileWatcher->removePath(file.absoluteFilePath());
                        _fileWatcher->addPath(file.absoluteFilePath());
                        _fileMap.value(file.absoluteFilePath(), file.size());
                        emit fileChangeNotice(file.absoluteFilePath(), FileStatus::Added, file.size());
                    }
                }
            }
        }
        msleep(1000);
    }
}



