#include "panelwindow.h"

PanelWindow::PanelWindow() : QGraphicsView(), scene(), errorMessage(), itemFactory(&connection) {
    setScene(&scene);
    panelRotation = 0;
    resize(1024, 768);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(0,0,width(), height());
    menuButton = new MenuButton(this, panelItems, &itemFactory);
    connect(menuButton, SIGNAL(panelRotationChanged(int)), this, SLOT(panelRotationChanged(int)));
    connect(menuButton, SIGNAL(fullscreenChanged(bool)), this, SLOT(fullscreenChanged(bool)));
    connect(menuButton, SIGNAL(setServerAddress(QString)), this, SLOT(setServerAddress(QString)));
    menuButton->setPos(0,0);
    connect(menuButton, SIGNAL(itemAdded(PanelItem*)), this, SLOT(addItem(PanelItem*)));
    scene.addItem(menuButton);
    setBackgroundBrush(QBrush(Qt::black));
    connect(&connection, SIGNAL(connectionError(QString)), this, SLOT(connectionError(QString)));
    errorMessage.setDefaultTextColor(Qt::red);
    errorMessage.setPos(width()/2 - errorMessage.textWidth()/2,0);
    scene.addItem(&errorMessage);
    menuButton->loadPanel();
}

PanelWindow::~PanelWindow() {
    qDeleteAll(panelItems);
    panelItems.clear();
}

void PanelWindow::connectionError(QString txt) {
    errorMessage.setPlainText(txt);
    errorMessage.setPos(width()/2 - errorMessage.textWidth()/2,0);
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
    errorMessage.setPos(width()/2 - errorMessage.textWidth()/2,0);
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
