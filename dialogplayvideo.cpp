#include "dialogplayvideo.h"
#include "ui_dialogplayvideo.h"

#include <QtWidgets>
#include <QVideoWidget>
#include <QDebug>

DialogPlayVideo::DialogPlayVideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPlayVideo)
{
    ui->setupUi(this);
    ui->btnPlay->setEnabled(false);
    ui->btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->sliderPosition->setRange(0,0);

    mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);

    m_errorLabel = new QLabel;
    m_errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(ui->btnOpen);
    controlLayout->addWidget(ui->btnPlay);
    controlLayout->addWidget(ui->sliderPosition);
    controlLayout->setEnabled(true);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ui->videoPlayer);
    layout->addLayout(controlLayout);
    layout->addWidget(m_errorLabel);

    setLayout(layout);

    mediaPlayer->setVideoOutput(ui->videoPlayer);
    connect(mediaPlayer, &QMediaPlayer::stateChanged,
           this, &DialogPlayVideo::mediaStateChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &DialogPlayVideo::positionChanged);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &DialogPlayVideo::durationChanged);
    connect(mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
           this, &DialogPlayVideo::handleError);

showFullScreen();

}

DialogPlayVideo::~DialogPlayVideo()
{
    delete ui;
}


void DialogPlayVideo::setUrl(const QUrl &url)
{
    m_errorLabel->setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    mediaPlayer->setMedia(url);
    ui->btnPlay->setEnabled(true);

}


void DialogPlayVideo::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state)
    {

    case QMediaPlayer::PlayingState:
        ui->btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;

    case QMediaPlayer::PausedState:
        ui->btnPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }

    update();
}

void DialogPlayVideo::positionChanged(qint64 position)
{
    ui->sliderPosition->setValue(position);
}

void DialogPlayVideo::durationChanged(qint64 duration)
{
    ui->sliderPosition->setRange(0, duration);
}


void DialogPlayVideo::handleError()
{
    ui->btnPlay->setEnabled(false);
    const QString errorString = mediaPlayer->errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(mediaPlayer->error()));
    else
        message += errorString;
    m_errorLabel->setText(message);
}

void DialogPlayVideo::on_btnPlay_clicked()
{
    switch (mediaPlayer->state())
    {
    case QMediaPlayer::PlayingState:
        mediaPlayer->pause();
        break;
    default:
        mediaPlayer->play();
        break;
    }
}

void DialogPlayVideo::on_btnOpen_clicked()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    QStringList supportedMimeTypes = mediaPlayer->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty())
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        setUrl(fileDialog.selectedUrls().constFirst());
    else
        ui->btnPlay->setEnabled(false);
}

void DialogPlayVideo::on_sliderPosition_sliderMoved(int position)
{
    mediaPlayer->setPosition(position);
}

void DialogPlayVideo::closeEvent(QCloseEvent *event)
{
    if(event->Close)
    {
        mediaPlayer->stop();
    }
}

//QEvent(ActivationChange, 0x7fffffffdde0)
//QEvent(ActivationChange, 0x7fffffffd710)
void DialogPlayVideo::changeEvent(QEvent * event)
{
        if(event->type() ==  QEvent::ActivationChange &&  this->isActiveWindow() == false)
        {
            mediaPlayer->pause();
        }

}
