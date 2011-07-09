#include "panelwindow.h"
#include <boost/shared_ptr.hpp>

PanelWindow::PanelWindow() : QGraphicsView(), scene(), errorMessage(), itemFactory(&connection) {
    setScene(&scene);
    panelRotation = 0;
    editMode = false;
    resize(1024, 640);  
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(0,0,width(), height());
    menuButton = new MenuButton(this, panelItems, &itemFactory);
    connect(menuButton, SIGNAL(panelRotationChanged(int)), this, SLOT(panelRotationChanged(int)));
    connect(menuButton, SIGNAL(fullscreenChanged(bool)), this, SLOT(fullscreenChanged(bool)));
    connect(menuButton, SIGNAL(setServerAddress(QString)), this, SLOT(setServerAddress(QString)));
    connect(menuButton, SIGNAL(editModeChanged(bool)), this, SLOT(editModeChanged(bool)));
    menuButton->setPos(0,0);
    connect(menuButton, SIGNAL(itemAdded(PanelItem*)), this, SLOT(addItem(PanelItem*)));
    scene.addItem(menuButton);
    setBackgroundBrush(QBrush(Qt::black));
    connect(&connection, SIGNAL(connectionError(QString)), this, SLOT(connectionError(QString)));
    errorMessage.setDefaultTextColor(Qt::red);
    errorMessage.setPos(0,20);
    scene.addItem(&errorMessage);
    menuButton->loadPanel();

    connect(&blankingTimer, SIGNAL(timeout()), this, SLOT(disableBlanking()));
    blankingTimer.start(30000);

}

PanelWindow::~PanelWindow() {
    qDeleteAll(panelItems);
    panelItems.clear();
}

void PanelWindow::connectionError(QString txt) {
    errorMessage.setPlainText(txt);
}

void PanelWindow::itemDestroyed(QObject *obj) {
    PanelItem *g = qobject_cast<PanelItem*> (obj);
    qDebug() << Q_FUNC_INFO << obj << (const void*) g;
    if(g)
        panelItems.removeOne(g);
}

void PanelWindow::addItem(PanelItem *g) {
    connect(g, SIGNAL(destroyed(QObject*)), this, SLOT(itemDestroyed(QObject*)));
    g->setPos(width()/2, height()/2);
    g->setPanelRotation(panelRotation);
    g->setEditMode(editMode);
    scene.addItem(g);
    panelItems.append(g);
}

void PanelWindow::panelRotationChanged(int r) {
    panelRotation = r;
    foreach(PanelItem *i, panelItems)
        i->setPanelRotation(r);
}

void PanelWindow::fullscreenChanged(bool fs) {
    if(fs)
        showFullScreen();
    else
        showNormal();
}
void PanelWindow::resizeEvent(QResizeEvent * event) {
    QWidget::resizeEvent(event);
    setSceneRect(0,0,width(), height());
}

void PanelWindow::setServerAddress(QString host) {
    unsigned int port = 0;
    QStringList hostport = host.split(":");
    if(hostport.length()==2) {
        bool ok;
        port = hostport.value(1).toUInt(&ok);
    }

    if(port==0)
        port = 51000;
    connection.disconnectFromHost();
    qDebug() << Q_FUNC_INFO << hostport.value(0) << hostport.value(1);
    QHostAddress addr;
    addr.setAddress(hostport.value(0));
    connection.connectTo(QHostAddress(hostport.value(0)), port);
}

void PanelWindow::editModeChanged(bool em) {
    qDebug() << Q_FUNC_INFO << em;
    editMode = em;
    foreach(PanelItem *it, panelItems)
        it->setEditMode(em);
}
void PanelWindow::disableBlanking() {
#ifdef MAEMO
    QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH,MCE_REQUEST_IF, MCE_PREVENT_BLANK_REQ));
    blankingTimer.start(30000);
#endif
}
