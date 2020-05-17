#ifndef MMCONVERT_H
#define MMCONVERT_H
#pragma once

#include <QMainWindow>
#include <QStringList>
#include <QLabel>


#include "tablemodel.h"

#include "mmconversioninfo.h"
#include "ffmpeginvoker.h"
#include "mmdirwatcher.h"

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QtMultimediaWidgets/QVideoWidget>




namespace Ui {
class MMConvert;
}

class MMConvert : public QMainWindow
{
    Q_OBJECT

public:
    explicit MMConvert(QWidget *parent = 0);
    ~MMConvert();


private:
    void processSelected();
    void setControls();
    void ProbeMovie();
    void addMM();


private slots:
    void setMMInfo(int index, MMConversionInfo* mmInfo);
    void on_btnProcess_clicked();
    void on_actionFind_Dupes_triggered();
    void on_actionLoad_MegaBoom_triggered();
    void on_actionCompare_triggered();
    void on_actionMM_Settings_triggered();
    void markProcess();
    void markDel();
    void markUseCustom();
    void changeNotice(QString fileName, MMDirWatcher::FileStatus, quint64 lastSize);

    void on_tvDB_customContextMenuRequested(const QPoint &pos);

    void on_actionPlex_DB_Scan_triggered();

public slots:
    void threadProcessUpdate(QString, MMConversionInfo *mmInfo);
    void processComplete(MMConversionInfo *mmInfo);

private:
    Ui::MMConvert *ui;
    TableModel* _mmConvertModel;
    MMConversionInfo _mmInfo;
    ffMpegInvoker* _ffMpegInvoker;
    QDialog *_popup;
    QLabel *_popupLabel;

    MMDirWatcher* _dirWatcher;
    bool _convertRunning;

    //Experimaental crap
    QMediaPlayer *_player;
    QVideoWidget *_videoWidget;
    QMediaPlaylist *_playlist;




protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);



    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
    void startDirWatch();
};



#endif // MMCONVERT_H
