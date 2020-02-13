#include "hfx.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HFX w;
    w.show();
    return a.exec();
}
