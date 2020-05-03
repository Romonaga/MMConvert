#ifndef DIALOGIMPORTMOVIES_H
#define DIALOGIMPORTMOVIES_H

#include <QDialog>

namespace Ui {
class DialogImportMovies;
}

class DialogImportMovies : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImportMovies(QWidget *parent = 0);
    ~DialogImportMovies();

private slots:
    void on_btnSelectFile_clicked();

    void on_btnDloads_clicked();

private:
    Ui::DialogImportMovies *ui;
};

#endif // DIALOGIMPORTMOVIES_H
