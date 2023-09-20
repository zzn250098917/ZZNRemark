#include "ZZNRemark.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZZNRemark w;
    QFont appFont("Microsoft YaHei",10);
    a.setFont(appFont);
    w.show();
    return a.exec();
}
