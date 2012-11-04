#include <QtGui/QApplication>
#include "ui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UI w;
    w.show();
    
    return a.exec();
}
