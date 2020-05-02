#ifndef DIALOGMMPLAYBACK_H
#define DIALOGMMPLAYBACK_H

#include <QDialog>

namespace Ui {
class DialogMMPlayBack;
}

class DialogMMPlayBack : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMMPlayBack(QWidget *parent = 0);
    ~DialogMMPlayBack();

private:
    Ui::DialogMMPlayBack *ui;
};

#endif // DIALOGMMPLAYBACK_H
