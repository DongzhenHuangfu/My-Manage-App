#include "hfx.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HFX w;
    w.setWindowTitle(QString("小熊加油哦！"));
    w.show();
    return a.exec();
}
