#include <QApplication>
#include "panelwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PanelWindow win;
    win.show();
    return a.exec();
}
