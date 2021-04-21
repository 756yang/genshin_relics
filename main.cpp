#include "genshin_relics.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    genshin_relics w;
    w.show();
    return a.exec();
}
