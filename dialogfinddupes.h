#ifndef DIALOGFINDDUPES_H
#define DIALOGFINDDUPES_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class DialogFindDupes;
}

class DialogFindDupes : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFindDupes(QWidget *parent = 0);
    ~DialogFindDupes();

    void runDupeCheck();

private slots:
    void on_btnRun_clicked();

    void on_btnDelete_clicked();

    void on_treeDupes_itemExpanded(QTreeWidgetItem *item);

private:
    Ui::DialogFindDupes *ui;
    QString _dirToScan;
};

#endif // DIALOGFINDDUPES_H
