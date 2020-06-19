#include "configuredialog.h"
#include <QApplication>

int
main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationDomain("ipcf.cnr.it");
    QCoreApplication::setOrganizationName("IPCF-CNR");
    QCoreApplication::setApplicationName("UPS-Alarm");
    QCoreApplication::setApplicationVersion("0.1");

    QApplication a(argc, argv);
    ConfigureDialog w;
    w.show();

    return a.exec();
}
