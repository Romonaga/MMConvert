#include "mmconvert.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MMConvert w;
    w.show();

    return a.exec();
}
