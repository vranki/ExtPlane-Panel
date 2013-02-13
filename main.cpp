#include <QApplication>
#include <QFont>
#include "panelwindow.h"

/**
 * This is the main function for ExtPlane-panel
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("vranki");
    a.setOrganizationDomain("extplane.org");
    a.setApplicationName("ExtPlane-panel");

    PanelWindow win;
    win.show();
    return a.exec();
}
