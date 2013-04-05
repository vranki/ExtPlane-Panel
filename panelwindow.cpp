#include "panelwindow.h"
#ifdef MAEMO
    #include <QDBusConnection>
    #include <QDBusMessage>
    #include "mce/mode-names.h"
    #include "mce/dbus-names.h"
#elif QTMOBILITY
    #include <QSystemScreenSaver>
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
#include <QDesktopWidget>
#include <QDesktopServices>

#include "extplaneconnection.h"
#include "simulatedextplaneconnection.h"
#include "menubutton.h"
#include "panelitemfactory.h"
#include "panelitems/panelitem.h"
#include "hardware/hardwaremanager.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/edititemdialog.h"
#include "dialogs/panelitemselectiondialog.h"
#include "dialogs/hardwaredialog.h"

PanelWindow::PanelWindow() : QGraphicsView(), scene(), statusMessage() {
    // Init
    appSettings = NULL;
    panelSettings = NULL;
    settingsDialog = NULL;
    editItemDialog = 0;
    panelRotation = 0;
    hardwareDialog = 0;
    interpolationEnabled = false;
    aaEnabled = false;
    defaultFontSize = 15;
    editMode = false;
    dirty = false;

    // Load settings
    appSettings = new Settings("org.vranki", "extplane-panel", this);

    // Create connection and item factory
    appSettings->beginGroup("settings");
    connection = appSettings->valueFromSettingsOrCommandLine("simulate", false).toBool() ? new SimulatedExtPlaneConnection() : new ExtPlaneConnection();
    appSettings->endGroup();
    hwManager = new HardwareManager(this, connection);

    // Setup window
    setScene(&scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(0, 0, width(), height());
    setBackgroundBrush(QBrush(Qt::black));
    setFrameShape(QFrame::NoFrame);

    // Settings dialog
    settingsDialog = new SettingsDialog(this, appSettings);
    connect(settingsDialog, SIGNAL(rotationChanged(int)), this, SLOT(panelRotationChanged(int)));
    connect(settingsDialog, SIGNAL(fullscreenChanged(bool)), this, SLOT(fullscreenChanged(bool)));
    connect(settingsDialog, SIGNAL(setServerAddress(QString)), this, SLOT(setServerAddress(QString)));
    connect(settingsDialog, SIGNAL(setUpdateInterval(double)), connection, SLOT(setUpdateInterval(double)));
    connect(settingsDialog, SIGNAL(setPanelUpdateInterval(double)), this, SLOT(setPanelUpdateInterval(double)));
    connect(settingsDialog, SIGNAL(setInterpolationEnabled(bool)), this, SLOT(setInterpolationEnabled(bool)));
    connect(settingsDialog, SIGNAL(setAntialiasEnabled(bool)), this, SLOT(setAntialiasEnabled(bool)));
    connect(settingsDialog, SIGNAL(setDefaultFontSize(double)), this, SLOT(setDefaultFontSize(double)));
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

    // Setup tick timer
    connect(&tickTimer, SIGNAL(timeout()), this, SLOT(tick()));
    tickTimer.setInterval(64);
    tickTimer.setSingleShot(false);
    connect(this, SIGNAL(tickTime(double,int)), connection, SLOT(tickTime(double,int)));

    // Load the last loaded panel. If there is no last loaded panel, we will create a new default one.
    // Furthermore, if the command line specifies a filename flag (--filename x.ini), we will load that one instead
    QString panelToLoad = appSettings->valueFromSettingsOrCommandLine("lastloadedprofile","").toString();
    if(panelToLoad.isEmpty()) {
        // This must the first launch - in this case we will create and load a default panel for saving to in the user's documents folder
        #if QT_VERSION >= 0x050000
            panelToLoad = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first() + QDir::separator() + "ExtPlane-Panel-Default.ini";
        #else
            panelToLoad = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + QDir::separator() + "ExtPlane-Panel-Default.ini";
        #endif
        panelSettings = new QSettings(panelToLoad,QSettings::IniFormat,this);
        saveProfile();
    }
    if(!appSettings->valueFromSettingsOrCommandLine("profile","").toString().isEmpty())
        panelToLoad = appSettings->valueFromSettingsOrCommandLine("profile").toString();
    this->loadProfile(panelToLoad);

    // Start connection to ExtPlane
    this->settingsDialog->loadSettings(); // This will trigger signals to start connection to ExtPlane

    // Disable screensaver on Maemo
    #ifdef MAEMO
        connect(&blankingTimer, SIGNAL(timeout()), this, SLOT(disableBlanking()));
        blankingTimer.start(30000);
    #elif QTMOBILITY
        // Disable screensaver via Qt Mobility
        QtMobility::QSystemScreenSaver *sss = new QtMobility::QSystemScreenSaver(this);
        sss->setScreenSaverInhibit();
    #endif

    // Force specific screen? (for multiple monitors)
    if(!appSettings->valueFromSettingsOrCommandLine("screen","").toString().isEmpty()) {
        // Set the window geometry to a specific screen number (starts logically at 0).
        QDesktopWidget desktop;
        this->setGeometry(desktop.screenGeometry(appSettings->valueFromSettingsOrCommandLine("screen").toInt()));
    } else {
        // Default window size
        resize(1024, 768);
    }

    // Start timers
    totalTime.start();
    time.start();
    tickTimer.start();
}

PanelWindow::~PanelWindow() {
    while(!panelItems.isEmpty())
        delete panelItems.first(); // Calls itemDeleted() and removes itself from list
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

void PanelWindow::addItem(PanelItem *newItem) {
    Q_ASSERT(newItem->parent() == this); // Item's parent should always be this
    connect(newItem, SIGNAL(destroyed(QObject*)), this, SLOT(itemDestroyed(QObject*)));
    connect(newItem, SIGNAL(editPanelItem(PanelItem*)), this, SLOT(editItem(PanelItem*)));
    connect(newItem, SIGNAL(panelItemChanged(PanelItem*)), this, SLOT(panelItemChanged(PanelItem*)));
    connect(settingsDialog, SIGNAL(setInterpolationEnabled(bool)), newItem, SLOT(setInterpolationEnabled(bool)));
    connect(settingsDialog, SIGNAL(setAntialiasEnabled(bool)), newItem, SLOT(setAntialiasEnabled(bool)));
    connect(settingsDialog, SIGNAL(setDefaultFontSize(double)), newItem, SLOT(setDefaultFontSize(double)));
    newItem->setPos(width()/2, height()/2);
    newItem->setPanelRotation(panelRotation);
    newItem->setEditMode(editMode);
    newItem->setInterpolationEnabled(interpolationEnabled);
    newItem->setAntialiasEnabled(aaEnabled);
    newItem->setDefaultFontSize(defaultFontSize);
    scene.addItem(newItem);
    panelItems.append(newItem);
    connect(this, SIGNAL(tickTime(double,int)), newItem, SLOT(tickTime(double,int)));
}

void PanelWindow::panelRotationChanged(int r) {
    panelRotation = r;
    foreach(PanelItem *i, panelItems)
        i->setPanelRotation(r);
}

void PanelWindow::fullscreenChanged(bool fs) {
    if(fs) {
        showFullScreen();
    } else {
        showNormal();
    }
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
    if(dt > 0.2f) {
        qDebug() << "Skipping frame, dt: " << dt;
        dt = 0;
    }
    emit tickTime(dt, totalTime.elapsed());
}

void PanelWindow::setInterpolationEnabled(bool ie) {
    interpolationEnabled = ie;
}

void PanelWindow::setAntialiasEnabled(bool ie) {
    aaEnabled = ie;
}

void PanelWindow::setPanelUpdateInterval(double newInterval) {
    if(newInterval > 0) {
        tickTimer.setInterval(newInterval * 1000.f);
        tickTimer.start();
    } else {
        tickTimer.stop();
    }
}

void PanelWindow::setDefaultFontSize(double newFs) {
    defaultFontSize = newFs;
}

void PanelWindow::disableBlanking() {
#ifdef MAEMO
    QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH,MCE_REQUEST_IF, MCE_PREVENT_BLANK_REQ));
    blankingTimer.start(30000);
#endif
}

void PanelWindow::setEditMode(bool em) {
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

void PanelWindow::showAddItemDialog() {
    PanelItemSelectionDialog *pisd = new PanelItemSelectionDialog(this, connection);
    connect(pisd, SIGNAL(addItem(PanelItem*)), this, SLOT(addItem(PanelItem*)));
    connect(this, SIGNAL(tickTime(double,int)), pisd, SLOT(tickTime(double,int)));
    pisd->exec();
}

void PanelWindow::saveProfile() {
    // New or overwrite?
    if(panelSettings == NULL) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save Profile"), "", tr("Ini Files (*.ini)"));
        if(!filename.isEmpty()) {
            // Create new file and save
            panelSettings = new QSettings(filename,QSettings::IniFormat,this);
            saveProfile(panelSettings->fileName());

            // Register this file as the last loaded...
            appSettings->setValue("lastloadedprofile", panelSettings->fileName());
        }
    } else {
        saveProfile(panelSettings->fileName());
    }
}

void PanelWindow::saveProfileAs() {

    QString filename = QFileDialog::getSaveFileName(this, tr("Save Profile"), panelSettings->fileName(), tr("Ini Files (*.ini)"));
    if(!filename.isEmpty()) {
        // Create new file and save
        panelSettings = new QSettings(filename,QSettings::IniFormat,this);
        saveProfile(panelSettings->fileName());

        // Register this file as the last loaded...
        appSettings->setValue("lastloadedprofile", panelSettings->fileName());
    }
}


void PanelWindow::saveProfile(QString filename) {
    qDebug() << Q_FUNC_INFO << "saving profile to " << filename;
    int panelNumber = 0;
    QString panelName = "Panel";
    panelSettings->beginGroup("panel-" + QString::number(panelNumber)); {
        panelSettings->group().clear();
        panelSettings->setValue("number", panelNumber);
        panelSettings->setValue("name", panelName);
        panelSettings->setValue("gaugecount", panelItems.size());
        int gn = 0;
        foreach(PanelItem *g, panelItems) {
            panelSettings->beginGroup("gauge-" + QString::number(gn)); {
                g->storeSettings(*panelSettings);
                panelSettings->endGroup(); }
            gn++;
        }
        panelSettings->endGroup(); }
    panelSettings->beginGroup("hardware");
    hwManager->saveSettings(panelSettings);
    panelSettings->endGroup();
    panelSettings->sync();
    dirty = false;
}

void PanelWindow::loadProfile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Profile"), "", tr("Ini Files (*.ini)"));
    if(!filename.isEmpty()) loadProfile(filename);
}

void PanelWindow::loadProfile(QString filename) {
    if(filename.isEmpty()) return;

    //TODO: dankrusi: confirm if the currently loaded panel is dirty...

    // Clear all panel items
    newProfile();

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
                PanelItem *g = itemFactory.itemForName(panelSettings->value("type").toString(), this, connection);
                if(g) {
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
    panelSettings->beginGroup("hardware");
    hwManager->loadSettings(panelSettings);
    panelSettings->endGroup();

    // Register this file as the last loaded...
    appSettings->setValue("lastloadedprofile",filename);
    dirty = false;
}

void PanelWindow::newProfile() {
    // Clear all panel items
    foreach(PanelItem *g, panelItems) {
        g->deleteLater();
    }

    // Load panel settings file
    if(panelSettings) delete panelSettings;
    panelSettings = NULL;

    dirty = true;
}

void PanelWindow::showHardware() {
    if(!hardwareDialog) {
        hardwareDialog = new HardwareDialog(this, hwManager);
    }
    hardwareDialog->show();
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
