#include <QInputDialog>

#include "panelwindow.h"

PanelWindow::PanelWindow() : QGraphicsView(), scene(), errorMessage() {
    // Init
    appSettings = NULL;
    panelSettings = NULL;
    settingsDialog = NULL;
    itemFactory = NULL;
    editItemDialog = 0;
    panelRotation = 0;
    editMode = false;

    // Load settings
    appSettings = new QSettings("org.vranki", "extplane-panel", this);

    // Create connection and item factory
    appSettings->beginGroup("settings");
    connection = appSettings->value("simulate", false).toBool() ? new SimulatedExtPlaneConnection() : new ExtPlaneConnection();
    appSettings->endGroup();
    itemFactory = new PanelItemFactory(connection);

    // Setup window
    setScene(&scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(0, 0, width(), height());
    setBackgroundBrush(QBrush(Qt::black));

    // Settings dialog
    settingsDialog = new SettingsDialog(this, appSettings);
    connect(settingsDialog, SIGNAL(rotationChanged(int)), this, SLOT(panelRotationChanged(int)));
    connect(settingsDialog, SIGNAL(fullscreenChanged(bool)), this, SLOT(fullscreenChanged(bool)));
    connect(settingsDialog, SIGNAL(setServerAddress(QString)), this, SLOT(setServerAddress(QString)));
    settingsDialog->setModal(false);
    settingsDialog->hide();

    // Menu button
    menuButton = new MenuButton(this);
    connect(menuButton, SIGNAL(panelRotationChanged(int)), this, SLOT(panelRotationChanged(int)));
    connect(menuButton, SIGNAL(fullscreenChanged(bool)), this, SLOT(fullscreenChanged(bool)));
    connect(menuButton, SIGNAL(setServerAddress(QString)), this, SLOT(setServerAddress(QString)));
    //connect(menuButton, SIGNAL(editModeChanged(bool)), this, SLOT(editModeChanged(bool)));
    menuButton->setPos(0,0);
    connect(menuButton, SIGNAL(itemAdded(PanelItem*)), this, SLOT(addItem(PanelItem*)));
    scene.addItem(menuButton);

    // Error message
    connect(connection, SIGNAL(connectionError(QString)), this, SLOT(connectionError(QString)));
    errorMessage.setDefaultTextColor(Qt::red);
    errorMessage.setPos(0,20);
    scene.addItem(&errorMessage);

    // Load last-loaded panel
    this->settingsDialog->loadSettings(); // this will trigger signals to start connection
    this->loadPanel();

    // Tell maemo os not to blank out
#ifdef MAEMO
    connect(&blankingTimer, SIGNAL(timeout()), this, SLOT(disableBlanking()));
    blankingTimer.start(30000);
#endif

    connect(&tickTimer, SIGNAL(timeout()), this, SLOT(tick()));
    tickTimer.setInterval(64);
    tickTimer.setSingleShot(false);
    tickTimer.start();
    totalTime.start();
    time.start();

    connect(this, SIGNAL(tickTime(double,int)), connection, SLOT(tickTime(double,int)));

#ifdef MOBILE_DEVICE
    showFullScreen();
#else
    resize(1024, 768);
#endif
}

PanelWindow::~PanelWindow() {
    qDeleteAll(panelItems);
    panelItems.clear();
    if(itemFactory) delete itemFactory;
    if(connection) delete connection;
    if(appSettings) delete appSettings;
    if(panelSettings) delete panelSettings;
}

void PanelWindow::connectionError(QString txt) {
    qDebug() << Q_FUNC_INFO << txt;
    errorMessage.setPlainText(txt);
}

void PanelWindow::itemDestroyed(QObject *obj) {
    // Delete from panelItems
    foreach(PanelItem *listItem, panelItems) {
        QObject *listItemQo = static_cast<QObject*>(listItem);
        if(listItemQo == obj) {
            panelItems.removeOne(listItem);
        }
    }
}

void PanelWindow::addItem(PanelItem *g) {
    connect(g, SIGNAL(destroyed(QObject*)), this, SLOT(itemDestroyed(QObject*)));
    connect(g, SIGNAL(editPanelItem(PanelItem*)), this, SLOT(editItem(PanelItem*)));
    g->setPos(width()/2, height()/2);
    g->setPanelRotation(panelRotation);
    g->setEditMode(editMode);
    scene.addItem(g);
    panelItems.append(g);
    connect(this, SIGNAL(tickTime(double,int)), g, SLOT(tickTime(double,int)));
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
    // Extract address and port
    unsigned int port = 0;
    QStringList hostport = host.split(":");
    if(hostport.length()==2) port = hostport.value(1).toUInt(NULL);
    if(port==0) port = 51000;
    QHostAddress addr;
    addr.setAddress(hostport.value(0));
    qDebug() << Q_FUNC_INFO << hostport.value(0) << port;

    // Disconnect and reconnect
    connection->disconnectFromHost();
    connection->connectTo(QHostAddress(hostport.value(0)), port);
}



void PanelWindow::tick() {
    double dt = time.elapsed() / 1000.0f;
    time.start();
    if(dt > 0.2) {
        qDebug() << "Skipping frame, dt: " << dt;
        dt = 0;
    }
    emit tickTime(dt, totalTime.elapsed());
}

void PanelWindow::disableBlanking() {
#ifdef MAEMO
    QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH,MCE_REQUEST_IF, MCE_PREVENT_BLANK_REQ));
    blankingTimer.start(30000);
#endif
}







void PanelWindow::setEditMode(bool em) {
    qDebug() << Q_FUNC_INFO << em;
    editMode = em;
    foreach(PanelItem *it, panelItems)
        it->setEditMode(em);
}

QList<PanelItem*> PanelWindow::selectedGauges() {
    QList<PanelItem*> selection;
    foreach(PanelItem* g, panelItems) {
        if(g->isSelected())
            selection.append(g);
    }
    return selection;
}

void PanelWindow::addItem() {
    bool ok;
    QString item = QInputDialog::getItem(this, "Add item", "Choose type:", itemFactory->itemNames(), 0, false, &ok);
    if(ok) {
        PanelItem *g=itemFactory->itemForName(item, this);
        if(g) addItem(g);
    }
}

void PanelWindow::savePanel() {
    //settingsDialog->saveSettings();
    int panelNumber = 0;
    QString panelName = "Panel";
    appSettings->beginGroup("panel-" + QString::number(panelNumber));
    appSettings->group().clear();
    appSettings->setValue("number", panelNumber);
    appSettings->setValue("name", panelName);
    appSettings->setValue("gaugecount", panelItems.size());
    int gn = 0;
    foreach(PanelItem *g, panelItems) {
        appSettings->beginGroup("gauge-" + QString::number(gn));
        g->storeSettings(*appSettings);
        appSettings->endGroup();
        gn++;
    }
    appSettings->endGroup();
    appSettings->sync();
}

void PanelWindow::loadPanel() {
    //settingsDialog->loadSettings();

    foreach(PanelItem *g, panelItems) {
        g->deleteLater();
    }
    int panelNumber = 0;
    while(panelNumber >= 0) {
        appSettings->beginGroup("panel-" + QString::number(panelNumber));
        if(appSettings->contains("name")) {
            int gc = appSettings->value("gaugecount", 0).toInt();
            QString name = appSettings->value("name").toString();
            for(int gn=0;gn<gc;gn++) {
                appSettings->beginGroup("gauge-" + QString::number(gn));
                PanelItem *g = itemFactory->itemForName(appSettings->value("type").toString(), this);
                if(g) {
                    //emit itemAdded(g);
                    addItem(g);
                    g->loadSettings(*appSettings);
                    g->applySettings();
                } else {
                    qDebug() << Q_FUNC_INFO << "Can't load item of type " << appSettings->value("type").toString();
                }
                appSettings->endGroup();
            }
            panelNumber++;
        } else {
            panelNumber = -1;
        }
        appSettings->endGroup();
    }

    //closeDialog();
}


void PanelWindow::showSettings() {
    settingsDialog->show();
}

// Displays EditItemDialog. Closes old one if open
void PanelWindow::editItem(PanelItem *item) { // Call with item 0 to destroy dialog
    if(editItemDialog) {
        disconnect(editItemDialog, 0, this, 0);
        editItemDialog->deleteLater();
    }
    editItemDialog = 0;
    if(!item)
        return;

    editItemDialog =  new EditItemDialog(this);
    editItemDialog->setModal(false);
    editItemDialog->setPanelItem(item);
    editItemDialog->show();
    connect(editItemDialog, SIGNAL(destroyed()), this, SLOT(editItem())); // Call this slot when dialog closed
}

void PanelWindow::quit() {
    // @TODO: ask for save if dirty
    QCoreApplication::quit();
}
