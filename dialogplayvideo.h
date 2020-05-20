#ifndef DIALOGPLAYVIDEO_H
#define DIALOGPLAYVIDEO_H

#include <QDialog>
#include <QtWidgets>
#include <QMediaPlayer>

namespace Ui {
class DialogPlayVideo;
}

class DialogPlayVideo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPlayVideo(QWidget *parent = 0);
    ~DialogPlayVideo();

    void setUrl(const QUrl &url);

public slots:

private slots:
       void mediaStateChanged(QMediaPlayer::State state);
       void positionChanged(qint64 position);
       void durationChanged(qint64 duration);
       void handleError();

       void on_btnPlay_clicked();

       void on_btnOpen_clicked();

       void on_sliderPosition_sliderMoved(int position);

private:
       QMediaPlayer* mediaPlayer;
       QLabel *m_errorLabel;


private:
    Ui::DialogPlayVideo *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGPLAYVIDEO_H
