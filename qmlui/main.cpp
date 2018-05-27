#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <extplaneconnection.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("vranki");
    app.setOrganizationDomain("extplane.org");
    app.setApplicationName("ExtPlane-panel");

    qmlRegisterType<ExtPlaneConnection>("org.vranki.extplane", 1, 0, "ExtPlaneConnection");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
