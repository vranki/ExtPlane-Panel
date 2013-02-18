#include "panelwindow.h"
#ifdef MAEMO
#include <QDBusConnection>
#include <QDBusMessage>
#include "mce/mode-names.h"
#include "mce/dbus-names.h"
#endif

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QInputDialog>
#include <QFileDialog>
#include <QCoreApplication>
#include "extplaneconnection.h"
#include "simulatedextplaneconnection.h"
#include "menubutton.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/edititemdialog.h"
#include "panelitemfactory.h"
#include "panelitems/panelitem.h"

PanelWindow::PanelWindow() : QGraphicsView(), scene(), statusMessage() {
    // Init
    appSettings = NULL;
    panelSettings = NULL;
    settingsDialog = NULL;
    itemFactory = NULL;
    editItemDialog = 0;
    panelRotation = 0;
    editMode = false;
    dirty = false;

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
    connect(connection, SIGNAL(connectionMessage(QString)), this, SLOT(connectionMessage(QString)));
    statusMessage.setDefaultTextColor(Qt::red);
    statusMessage.setPos(0,20);
    scene.addItem(&statusMessage);

    // Load last-loaded panel
    this->loadPanel(appSettings->value("lastloadedpanel","").toString());

    // Start connection to ExtPlane
    this->settingsDialog->loadSettings(); // This will trigger signals to start connection to ExtPlane
    //this->loadPanel();

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

void PanelWindow::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    if(event->key()==Qt::Key_Space) {
        setEditMode(!editMode); // @todo set the ui checkbox also
    } else if(event->key()==Qt::Key_F) {
        fullscreenChanged(!isFullScreen());
    } else if(event->key()==Qt::Key_Delete) {
        qDeleteAll(selectedGauges());
    }
}

void PanelWindow::connectionMessage(QString txt) {
    qDebug() << Q_FUNC_INFO << txt;
    statusMessage.setPlainText(txt);
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
    connect(g, SIGNAL(panelItemChanged(PanelItem*)), this, SLOT(panelItemChanged(PanelItem*)));
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
    // New or overwrite?
    if(panelSettings == NULL) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save Panel File"), "", tr("Ini Files (*.ini)"));
        if(!filename.isEmpty()) {
            // Create new file and save
            panelSettings = new QSettings(filename,QSettings::IniFormat,this);
            savePanel(panelSettings->fileName());

            // Register this file as the last loaded...
            appSettings->setValue("lastloadedpanel",panelSettings->fileName());
        }
    } else {
        savePanel(panelSettings->fileName());
    }
}


void PanelWindow::savePanel(QString filename) {
    qDebug() << Q_FUNC_INFO << "saving panel to " << filename;
    int panelNumber = 0;
    QString panelName = "Panel";
    panelSettings->beginGroup("panel-" + QString::number(panelNumber));
    panelSettings->group().clear();
    panelSettings->setValue("number", panelNumber);
    panelSettings->setValue("name", panelName);
    panelSettings->setValue("gaugecount", panelItems.size());
    int gn = 0;
    foreach(PanelItem *g, panelItems) {
        panelSettings->beginGroup("gauge-" + QString::number(gn));
        g->storeSettings(*panelSettings);
        panelSettings->endGroup();
        gn++;
    }
    panelSettings->endGroup();
    panelSettings->sync();
    dirty = false;
}

void PanelWindow::loadPanel() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Panel File"), "", tr("Ini Files (*.ini)"));
    if(!filename.isEmpty()) loadPanel(filename);
}

void PanelWindow::loadPanel(QString filename) {
    if(filename.isEmpty()) return;

    //TODO: dankrusi: confirm if the currently loaded panel is dirty...

    // Clear all panel items
    newPanel();

    // Load panel settings file
    qDebug() << Q_FUNC_INFO << "loading panel from " << filename;
    if(!QFile::exists(filename)) {
        connectionMessage(QString("Panel file %1 does not exist.").arg(filename));
        return;
    }
    panelSettings = new QSettings(filename,QSettings::IniFormat,this);

    // Load from settings files
    int panelNumber = 0;
    while(panelNumber >= 0) {
        panelSettings->beginGroup("panel-" + QString::number(panelNumber));
        if(panelSettings->contains("name")) {
            int gc = panelSettings->value("gaugecount", 0).toInt();
            QString name = panelSettings->value("name").toString();
            for(int gn=0;gn<gc;gn++) {
                panelSettings->beginGroup("gauge-" + QString::number(gn));
                PanelItem *g = itemFactory->itemForName(panelSettings->value("type").toString(), this);
                if(g) {
                    //emit itemAdded(g);
                    addItem(g);
                    g->loadSettings(*panelSettings);
                    g->applySettings();
                } else {
                    qDebug() << Q_FUNC_INFO << "Can't load item of type " << panelSettings->value("type").toString();
                }
                panelSettings->endGroup();
            }
            panelNumber++;
        } else {
            panelNumber = -1;
        }
        panelSettings->endGroup();
    }

    // Register this file as the last loaded...
    appSettings->setValue("lastloadedpanel",filename);
    dirty = false;
}

void PanelWindow::newPanel() {
    qDebug() << Q_FUNC_INFO << "new panel";

    // Clear all panel items
    foreach(PanelItem *g, panelItems) {
	g->deleteLater();
    }

    // Load panel settings file
    if(panelSettings) delete panelSettings;
    panelSettings = NULL;

    dirty = true;
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
    if(!editMode) return; // Don't open dialog if not in edit mode
    editItemDialog =  new EditItemDialog(this);
    editItemDialog->setModal(false);
    editItemDialog->setPanelItem(item);
    editItemDialog->show();
    connect(editItemDialog, SIGNAL(destroyed()), this, SLOT(editItem())); // Call this slot when dialog closed
}

void PanelWindow::panelItemChanged(PanelItem *item) {
    if(!dirty) {
        qDebug() << Q_FUNC_INFO;
        dirty = true;
    }
}

void PanelWindow::quit() {
    // @TODO: ask for save if dirty
    QCoreApplication::quit();
}
