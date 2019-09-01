#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <extplaneclient.h>
#include <dataref.h>
#include <extplaneconnection.h>
#include <clientdatarefprovider.h>
#include <paintedpanelitem.h>
#include <extplaneutilities.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("vranki");
    app.setOrganizationDomain("extplane.org");
    app.setApplicationName("ExtPlane-panel-qmlui");

    qmlRegisterInterface<ClientDataRefProvider>("ClientDataRefProvider");
    qmlRegisterType<ExtPlaneConnection>("org.vranki.extplane", 1, 0, "ExtPlaneConnection");
    qmlRegisterType<ExtPlaneClient>("org.vranki.extplane", 1, 0, "ExtPlaneClient");
    qmlRegisterType<DataRef>("org.vranki.extplane", 1, 0, "DataRef");
    qmlRegisterType<PaintedPanelItem>("org.vranki.extplane", 1, 0, "PaintedPanelItem");
    qmlRegisterType<ExtplaneUtilities>("org.vranki.extplane", 1, 0, "ExtplaneUtilities");

    QQmlApplicationEngine engine;
    ExtPlaneClient epc(nullptr, "ExtPlaneClient", true);
    epc.createClient();
    engine.rootContext()->setContextProperty("extplaneClient", &epc);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
