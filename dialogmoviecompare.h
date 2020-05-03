#ifndef DIALOGMOVIECOMPARE_H
#define DIALOGMOVIECOMPARE_H

#include <QDialog>

namespace Ui {
class DialogMovieCompare;
}

class DialogMovieCompare : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMovieCompare(QWidget *parent = 0);
    ~DialogMovieCompare();

private slots:
    void on_btnRun_clicked();

    void on_btnMega_clicked();

    void on_pushButton_clicked();

    void on_btnMegaDL_clicked();

private:
    Ui::DialogMovieCompare *ui;
};

#endif // DIALOGMOVIECOMPARE_H
