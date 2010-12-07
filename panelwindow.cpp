#include "panelwindow.h"

GaugeWindow::GaugeWindow() : QGraphicsView(), scene(), errorMessage(), itemFactory(&connection) {
    setScene(&scene);
    resize(1024, 768);
    setSceneRect(0,0,width(), height());
    menuButton = new MenuButton(this, panelItems, &itemFactory);
    menuButton->setPos(0,0);
    connect(menuButton, SIGNAL(itemAdded(PanelItem*)), this, SLOT(addItem(PanelItem*)));
    scene.addItem(menuButton);
    setBackgroundBrush(QBrush(Qt::black));
    connection.connectTo(QHostAddress::LocalHost, 51000);
    connect(&connection, SIGNAL(connectionError(QString)), this, SLOT(connectionError(QString)));
    errorMessage.setDefaultTextColor(Qt::red);
    errorMessage.setPos(width()/2 - errorMessage.textWidth()/2,0);
    scene.addItem(&errorMessage);
    menuButton->loadPanel();
}

GaugeWindow::~GaugeWindow() {
    qDeleteAll(panelItems);
    panelItems.clear();
}

void GaugeWindow::connectionError(QString txt) {
    errorMessage.setPlainText(txt);
}

void GaugeWindow::itemDestroyed(QObject *obj) {
    qDebug() << Q_FUNC_INFO << obj;
    PanelItem *g = qobject_cast<PanelItem*> (obj);
    //Q_ASSERT(g);
    if(g)
        panelItems.removeOne(g);
}

void GaugeWindow::addItem(PanelItem *g) {
    connect(g, SIGNAL(destroyed(QObject*)), this, SLOT(itemDestroyed(QObject*)));
    g->setPos(width()/2, height()/2);
    scene.addItem(g);
    panelItems.append(g);
}
