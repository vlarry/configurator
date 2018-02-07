#include "configuratorwindow.h"
#include <QApplication>
//------------------------------
int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    QApplication a(argc, argv);

    ConfiguratorWindow w;
    w.show();
    
    return a.exec();
}
