#ifndef DIALOGPLEXDBSCANNER_H
#define DIALOGPLEXDBSCANNER_H

#include <QDialog>
#include <QLabel>

#include "plexmediatablemodel.h"

namespace Ui {
class DialogPlexDBScanner;
}

class DialogPlexDBScanner : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPlexDBScanner(QWidget *parent = 0);
    ~DialogPlexDBScanner();

private:


private:
    Ui::DialogPlexDBScanner *ui;
    PlexMediaTableModel* _plexModel;


    QDialog *_popup;
    QLabel *_popupLabel;

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void on_btnCreateNewDirStructure_clicked();
};

#endif // DIALOGPLEXDBSCANNER_H
