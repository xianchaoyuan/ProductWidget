#include "ProductWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProductWidget w;
    w.show();
    return a.exec();
}
