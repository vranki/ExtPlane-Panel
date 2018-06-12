#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <extplaneclient.h>
#include <clientdataref.h>
#include <extplaneconnection.h>
#include <clientdatarefprovider.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("vranki");
    app.setOrganizationDomain("extplane.org");
    app.setApplicationName("ExtPlane-panel");
// "ClientDataRefProvider"
    qmlRegisterInterface<ClientDataRefProvider>("ClientDataRefProvider");
    qmlRegisterType<ExtPlaneConnection>("org.vranki.extplane", 1, 0, "ExtPlaneConnection");
    qmlRegisterType<ExtPlaneClient>("org.vranki.extplane", 1, 0, "ExtPlaneClient");
    qmlRegisterType<ClientDataRef>("org.vranki.extplane", 1, 0, "ClientDataRef");

    qmlRegisterType<ExtPlaneConnection>("org.vranki.extplane", 1, 0, "ExtPlaneConnection");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
