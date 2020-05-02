#ifndef DIALOGDEEPPROBE_H
#define DIALOGDEEPPROBE_H

#include <QDialog>
#include <QString>
#include "ffmpeginvoker.h"


namespace Ui {
class DialogDeepProbe;
}

class DialogDeepProbe : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeepProbe(MMConversionInfo* mmInfo, QWidget *parent = 0);
    ~DialogDeepProbe();

private:
    Ui::DialogDeepProbe *ui;
    MMConversionInfo* _mmInfo;
};

#endif // DIALOGDEEPPROBE_H
