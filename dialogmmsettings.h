#ifndef DIALOGMMSETTINGS_H
#define DIALOGMMSETTINGS_H

#include <QDialog>

namespace Ui {
class DialogMMSettings;
}

class DialogMMSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMMSettings(QWidget *parent = 0);
    ~DialogMMSettings();

private slots:
    void on_btnSave_clicked();
    void on_cboxDelete_clicked(bool checked);
    void on_cboxStrip_clicked(bool checked);
    void on_spinAutoCustom_valueChanged(int arg1);
    void on_spinCrf_valueChanged(int arg1);
    void on_btnSelPath_clicked();

    void on_cbAutoProcess_clicked(bool checked);

    void on_btnDirToMonitor_clicked();

    void on_buttonBox_accepted();

private:
    Ui::DialogMMSettings *ui;
};

#endif // DIALOGMMSETTINGS_H
