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

#include "../util/console.h"
#include "extplaneconnection.h"
#include "extplaneclient.h"
#include "simulatedextplaneconnection.h"
#include "menubutton.h"
#include "panelitemfactory.h"
#include "panelitems/panelitem.h"
#include "../hardware/hardwaremanager.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/edititemdialog.h"
#include "dialogs/panelitemselectiondialog.h"
#include "dialogs/hardwaredialog.h"
#include "dialogs/panelsdialog.h"

#define AUTO_PANEL_DATAREF "sim/aircraft/view/acf_tailnum" // dankrusi: This seems to be the best trigger - correct me if I am wrong
#define ADJUST_POWER_DATAREF "sim/cockpit/electrical/avionics_on"

PanelWindow::PanelWindow() : QGraphicsView(), scene(), statusMessage() {
    // Init
    appSettings = NULL;
    profileSettings = NULL;
    settingsDialog = NULL;
    editItemDialog = NULL;
    hardwareDialog = NULL;
    panelsDialog = NULL;
    interpolationEnabled = false;
    aaEnabled = false;
    defaultFontSize = 15;
    editMode = false;
    dirty = false;

    // Set up viewport for touch events
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

    // Load settings
    appSettings = new Settings("org.vranki", "extplane-panel", this);

    // Create panel object
    currentPanel = new ExtPlanePanel(appSettings,this);

    // Create connection and item factory
    connection = appSettings->valueFromSettingsOrCommandLine("simulate", false).toBool() ? new SimulatedExtPlaneConnection() : new ExtPlaneConnection();
    hwManager = new HardwareManager(this, connection);
    connect(this, SIGNAL(tickTime(double,int)), hwManager, SLOT(tickTime(double,int)));

    // Create our panel window client
    client = new ExtPlaneClient(this,"PanelWindow",connection);
    connection->registerClient(client);
    connect(client, SIGNAL(refChanged(QString,QString)), this, SLOT(clientDataRefChanged(QString,QString)));
    connect(client, SIGNAL(refChanged(QString,double)), this, SLOT(clientDataRefChanged(QString,double)));

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
    connect(settingsDialog, SIGNAL(setAutoPanelsEnabled(bool)), this, SLOT(setAutoPanelsEnabled(bool)));
    connect(settingsDialog, SIGNAL(setAdjustPowerEnabled(bool)), this, SLOT(setAdjustPowerEnabled(bool)));
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
    menuButton->setZValue(PANEL_PANELINFO_ZVALUE);
    connect(menuButton, SIGNAL(itemAdded(PanelItem*)), this, SLOT(addItem(PanelItem*)));
    scene.addItem(menuButton);

    // Status message
    connect(connection, SIGNAL(connectionMessage(QString)), this, SLOT(connectionMessage(QString)));
    statusMessage.setZValue(PANEL_PANELINFO_ZVALUE);
    statusMessage.setDefaultTextColor(Qt::red);
    statusMessage.setPos(0,20);
    scene.addItem(&statusMessage);

    // Hidden GUI mode? This removes any GUI stuff (except edit mode of course), useful for a simpit
    if(appSettings->valueFromSettingsOrCommandLine("hidden-gui", false).toBool()) {
        menuButton->setOpacity(0.001); // Newer Qt version will set to hidden if 0
        statusMessage.setOpacity(0.0);
    }

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
        setProfileSettings(new QSettings(panelToLoad,QSettings::IniFormat,this));
        currentPanel->name = "Panel1";
        currentPanel->groupName = "panel-0";
        saveProfile(profileSettings->fileName());
        loadProfile(profileSettings->fileName());
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
        // Remember size and position?
        if(appSettings->valueFromSettingsOrCommandLine("remembersizeandposition",false).toBool() == true) {
            // Load from last saved setting
            this->setGeometry(appSettings->valueFromSettingsOrCommandLine("posx", 30).toInt(),
                              appSettings->valueFromSettingsOrCommandLine("posy", 30).toInt(),
                              appSettings->valueFromSettingsOrCommandLine("width", 1024).toInt(),
                              appSettings->valueFromSettingsOrCommandLine("height", 768).toInt());
        } else {
            // Default window size
            resize(1024,768);
        }
    }

    // Start timers
    totalTime.start();
    time.start();
    tickTimer.start();
}

PanelWindow::~PanelWindow() {
    while(!currentPanel->items()->isEmpty())
        delete currentPanel->items()->first(); // Calls itemDeleted() and removes itself from list
    if(connection) delete connection;
    if(appSettings) delete appSettings;
    if(currentPanel) delete currentPanel;
    if(client) delete client;
}

// -------------------------------------------------------------------------------------
// -- GUI Methods
// -------------------------------------------------------------------------------------

void PanelWindow::showHardware() {
    // Lazy load the UI
    if(!hardwareDialog) {
        hardwareDialog = new HardwareDialog(this, hwManager);
    }
    hardwareDialog->show();
}

void PanelWindow::showSettings() {
    // Settings UI is always loaded
    settingsDialog->show();
}

void PanelWindow::showPanels() {
    // Lazy load the UI
    if(!panelsDialog) {
        panelsDialog = new PanelsDialog(this, this->appSettings);
        panelsDialog->profileSettings = this->profileSettings;
        panelsDialog->refreshPanelList();
        connect(this,SIGNAL(panelsChanged()),panelsDialog,SLOT(refreshPanelList()));
        connect(panelsDialog,SIGNAL(newPanel()),this,SLOT(newPanel()));
        connect(panelsDialog,SIGNAL(copyPanel(QString)),this,SLOT(copyPanel(QString)));
        connect(panelsDialog,SIGNAL(removePanel(QString)),this,SLOT(removePanel(QString)));
        connect(panelsDialog,SIGNAL(loadPanel(QString)),this,SLOT(loadPanel(QString)));
    }
    panelsDialog->show();
}

void PanelWindow::showAddItemDialog() {
    PanelItemSelectionDialog *pisd = new PanelItemSelectionDialog(this);
    connect(pisd, SIGNAL(addItem(QString)), this, SLOT(addItem(QString)));
    connect(this, SIGNAL(tickTime(double,int)), pisd, SLOT(tickTime(double,int)));
    pisd->exec();
}

void PanelWindow::editItem(PanelItem *item) { // Call with item 0 to destroy dialog
    // Displays EditItemDialog. Closes old one if open
    if(editItemDialog) {
        disconnect(editItemDialog, 0, this, 0);
        editItemDialog->deleteLater();
    }
    editItemDialog = 0;
    if(!item) return;
    if(!editMode) return; // Don't open dialog if not in edit mode
    editItemDialog =  new EditItemDialog(this);
    editItemDialog->setModal(false);
    editItemDialog->setPanelItem(item);
    editItemDialog->show();
    connect(editItemDialog, SIGNAL(destroyed()), this, SLOT(editItem())); // Call this slot when dialog closed
}

void PanelWindow::panelItemChanged(PanelItem *item) {
    Q_UNUSED(item);
    dirty = true;
}

void PanelWindow::connectionMessage(QString txt) {
    INFO << txt;
    statusMessage.setPlainText(txt);
}

void PanelWindow::itemDestroyed(QObject *obj) {
    // Delete from panelItems
    for(int i = 0; i < currentPanel->items()->count(); i++) {
        PanelItem *item = currentPanel->items()->at(i);
        QObject *listItemQo = static_cast<QObject*>(item);
        if(listItemQo == obj) {
            currentPanel->items()->removeOne(item);
        }
    }
}

void PanelWindow::addItem(PanelItem *newItem) {
    //Q_ASSERT(newItem->parent() == this); // Item's parent should always be this
    connect(newItem, SIGNAL(destroyed(QObject*)), this, SLOT(itemDestroyed(QObject*)));
    connect(newItem, SIGNAL(editPanelItem(PanelItem*)), this, SLOT(editItem(PanelItem*)));
    connect(newItem, SIGNAL(panelItemChanged(PanelItem*)), this, SLOT(panelItemChanged(PanelItem*)));
    connect(settingsDialog, SIGNAL(setInterpolationEnabled(bool)), newItem, SLOT(setInterpolationEnabled(bool)));
    connect(settingsDialog, SIGNAL(setAntialiasEnabled(bool)), newItem, SLOT(setAntialiasEnabled(bool)));
    connect(settingsDialog, SIGNAL(setDefaultFontSize(double)), newItem, SLOT(setDefaultFontSize(double)));
    newItem->setPos(width()/2, height()/2);
    newItem->setPanelRotation(currentPanel->rotation);
    newItem->setEditMode(editMode);
    newItem->setInterpolationEnabled(interpolationEnabled);
    newItem->setAntialiasEnabled(aaEnabled);
    newItem->setDefaultFontSize(defaultFontSize);
    scene.addItem(newItem);
    currentPanel->items()->append(newItem);
    newItem->setSize(newItem->width(),newItem->height()); // PanelItems with cached Pixmaps redraw on the sizeChanged event, so we force it here to allow widgets to redraw with a scene
    connect(this, SIGNAL(tickTime(double,int)), newItem, SLOT(tickTime(double,int)));
}

void PanelWindow::addItem(QString itemName) {
    addItem(itemFactory.itemForName(itemName,currentPanel,connection));
}

void PanelWindow::tick() {
    double dt = time.elapsed() / 1000.0f;
    time.start();
    if(dt > 0.2f) {
        DEBUG << "Skipping frame, dt: " << dt;
        dt = 0;
    }
    emit tickTime(dt, totalTime.elapsed());
}

void PanelWindow::setEditMode(bool em) {
    editMode = em;
    for(int i = 0; i < currentPanel->items()->count(); i++) {
        PanelItem *item = currentPanel->items()->at(i);
        item->setEditMode(em);
    }
}

QList<PanelItem*> PanelWindow::selectedGauges() {
    QList<PanelItem*> selection;
    for(int i = 0; i < currentPanel->items()->count(); i++) {
        PanelItem *item = currentPanel->items()->at(i);
        if(item->isSelected())
            selection.append(item);
    }
    return selection;
}

// -------------------------------------------------------------------------------------
// -- Window Methods
// -------------------------------------------------------------------------------------

void PanelWindow::disableBlanking() {
#ifdef MAEMO
    QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH,MCE_REQUEST_IF, MCE_PREVENT_BLANK_REQ));
    blankingTimer.start(30000);
#endif
}

void PanelWindow::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    if(event->key()==Qt::Key_Space) {
        setEditMode(!editMode); // @todo set the ui checkbox also
    } else if(event->key()==Qt::Key_F) {
        fullscreenChanged(!isFullScreen());
    } else if(event->key()==Qt::Key_Delete) {
        qDeleteAll(selectedGauges());
    } else if(event->key()==Qt::Key_Left) {
        prevPanel();
    } else if(event->key()==Qt::Key_Right) {
        nextPanel();
    }
}

void PanelWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    setSceneRect(0,0,width(), height());
}

void PanelWindow::closeEvent(QCloseEvent *event) {

    // Save some gui related settings
    appSettings->setValue("width",geometry().width());
    appSettings->setValue("height",geometry().height());
    appSettings->setValue("posx",geometry().x());
    appSettings->setValue("posy",geometry().y());
    appSettings->sync();

    QWidget::closeEvent(event);
}

void PanelWindow::quit() {
    // @TODO: ask for save if dirty
    QCoreApplication::quit();
}

// -------------------------------------------------------------------------------------
// -- Settings Methods
// -------------------------------------------------------------------------------------

void PanelWindow::panelRotationChanged(int r) {
    currentPanel->rotation = r;
    for(int i = 0; i < currentPanel->items()->count(); i++) {
        PanelItem *item = currentPanel->items()->at(i);
        item->setPanelRotation(r);
    }
}

void PanelWindow::fullscreenChanged(bool fs) {
    if(fs) {
        showFullScreen();
    } else {
        showNormal();
    }
}

void PanelWindow::setServerAddress(QString host) {
    // Extract address and port
    unsigned int port = 0;
    QStringList hostport = host.split(":");
    if(hostport.length()==2) port = hostport.value(1).toUInt(NULL);
    if(port==0) port = 51000;
    INFO << "Connecting to" << hostport.value(0) << "on port" << port;

    // Disconnect and reconnect
    connection->disconnectFromHost();
    connection->setHostName(host);
    connection->setPort(port);
    connection->startConnection();
}

void PanelWindow::setInterpolationEnabled(bool enabled) {
    interpolationEnabled = enabled;
}

void PanelWindow::setAntialiasEnabled(bool enabled) {
    aaEnabled = enabled;
}

void PanelWindow::setAutoPanelsEnabled(bool enabled) {
    if(client && enabled && !client->isDataRefSubscribed(AUTO_PANEL_DATAREF)) client->subscribeDataRef(AUTO_PANEL_DATAREF);
    else if(client && !enabled && client->isDataRefSubscribed(AUTO_PANEL_DATAREF)) client->unsubscribeDataRef(AUTO_PANEL_DATAREF);
}

void PanelWindow::setAdjustPowerEnabled(bool enabled) {
    if(client && enabled && !client->isDataRefSubscribed(ADJUST_POWER_DATAREF)) client->subscribeDataRef(ADJUST_POWER_DATAREF);
    else if(client && !enabled && client->isDataRefSubscribed(ADJUST_POWER_DATAREF)) client->unsubscribeDataRef(ADJUST_POWER_DATAREF);
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

// -------------------------------------------------------------------------------------
// -- Profile Methods
// -------------------------------------------------------------------------------------

void PanelWindow::setProfileSettings(QSettings *settings) {
    if(profileSettings) delete profileSettings;
    profileSettings = settings;
    currentPanel->settings = settings;
    if(panelsDialog) panelsDialog->profileSettings = settings;

}

void PanelWindow::saveProfile() {
    // New or overwrite?
    if(profileSettings == NULL) {
        saveProfileAs();
    } else {
        saveProfile(profileSettings->fileName());
    }
}

void PanelWindow::saveProfileAs() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Profile"), profileSettings == NULL ? "" : profileSettings->fileName(), tr("Ini Files (*.ini)"));
    if(!filename.isEmpty()) {
        // Create new file and save
        setProfileSettings(new QSettings(filename,QSettings::IniFormat,this));
        saveProfile(profileSettings->fileName());

        // Register this file as the last loaded...
        appSettings->setValue("lastloadedprofile", profileSettings->fileName());
    }
}


void PanelWindow::saveProfile(QString filename) {
    INFO << "Saving profile to " << filename;
    saveCurrentPanel();
    profileSettings->beginGroup("hardware");
    hwManager->saveSettings(profileSettings);
    profileSettings->endGroup();
    profileSettings->sync();
    dirty = false;
}

void PanelWindow::loadProfile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Profile"), "", tr("Ini Files (*.ini)"));
    if(!filename.isEmpty()) loadProfile(filename);
}

void PanelWindow::loadProfile(QString filename) {
    if(filename.isEmpty()) return;

    //TODO: dankrusi: confirm if the currently loaded panel is dirty...

    // Load panel settings file
    INFO << "Loading profile from" << filename;
    if(!QFile::exists(filename)) {
        connectionMessage(QString("Profile %1 does not exist.").arg(filename));
        return;
    }

    // Clear all panel items
    clearPanel();

    // Load settings
    setProfileSettings(new QSettings(filename,QSettings::IniFormat,this));
    DEBUG << "Discovered panels:" << getPanelGroupNames();

    // Load first panel
    loadPanel(QString::null);

    // Load hardware settings
    profileSettings->beginGroup("hardware");
    hwManager->loadSettings(profileSettings);
    profileSettings->endGroup();

    // Register this file as the last loaded...
    appSettings->setValue("lastloadedprofile",filename);
    dirty = false;
}

void PanelWindow::newProfile() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Profile"), "", tr("Ini Files (*.ini)"));
    if(!filename.isEmpty()) {
        // Clear out panel
        clearPanel();

        // Create new file and save
        setProfileSettings(new QSettings(filename,QSettings::IniFormat,this));
        currentPanel->name = "Panel1";
        currentPanel->groupName = "panel-0";
        saveProfile(profileSettings->fileName());
        loadProfile(profileSettings->fileName());

        // Register this file as the last loaded...
        appSettings->setValue("lastloadedprofile", profileSettings->fileName());
    }
}

// -------------------------------------------------------------------------------------
// -- Panel Methods
// -------------------------------------------------------------------------------------

QStringList PanelWindow::getPanelGroupNames() {
    // The QSettings and its groups are not really optimal for having data which cannot be entirely flushed out on
    // every save. Thus we have a problem when using panel-# when copying and removing and renaming, as we cannot
    // do this easily using QSettings in a manor where we can still read by iterating 0-N until nothing shows up.
    // Thus we use this method for 'discovering' all panels that might be contained, which also alows a more sane
    // iteration. Con: we must loop all keys (the entire file). Probably no problem, if it is, we can cache this on
    // profile load.
    QStringList ret;
    if(profileSettings != NULL) {
        foreach(QString key, profileSettings->allKeys()) {
            if(key.split('/').count() == 2 && key.startsWith("panel-") && key.endsWith("/name")) {
                ret.append(key.replace("/name",""));
            }
        }
    }
    return ret;
}

QStringList PanelWindow::getPanelNames() {
    QStringList ret;
    if(profileSettings != NULL) {
        foreach(QString groupName, getPanelGroupNames()) {
            profileSettings->beginGroup(groupName);
            ret.append(profileSettings->value("name").toString());
            profileSettings->endGroup();
        }
    }
    return ret;
}

void PanelWindow::saveCurrentPanel() {
    profileSettings->beginGroup(currentPanel->groupName); {
        profileSettings->group().clear(); //TODO: this doesnt actually clear the group; group() is a string.
        profileSettings->setValue("name", currentPanel->name);
        profileSettings->setValue("gaugecount", currentPanel->items()->size());
        int gn = 0;
        for(int i = 0; i < currentPanel->items()->count(); i++) {
            PanelItem *item = currentPanel->items()->at(i);
            profileSettings->beginGroup("gauge-" + QString::number(gn)); {
                item->storeSettings(*profileSettings);
                profileSettings->endGroup(); }
            gn++;
        }
        profileSettings->endGroup();
    }
}

void PanelWindow::loadPanel(QString name) {

    // Clear all panel items
    clearPanel();

    // Find the panel by name
    foreach(QString panelGroupName, getPanelGroupNames()) {
        profileSettings->beginGroup(panelGroupName);
        // Check if name matches, if search name is null, take first panel
        if(name == QString::null || profileSettings->value("name").toString() == name) {
            INFO << "Loading panel" << profileSettings->value("name").toString();
            currentPanel->name = profileSettings->value("name").toString();
            currentPanel->groupName = panelGroupName;
            this->setWindowTitle(QString("ExtPlane-Panel - %1").arg(profileSettings->value("name").toString()));
            // Load all panel items
            int gc = profileSettings->value("gaugecount", 0).toInt();
            for(int gn=0;gn<gc;gn++) {
                profileSettings->beginGroup("gauge-" + QString::number(gn));
                PanelItem *g = itemFactory.itemForName(profileSettings->value("type").toString(), currentPanel, connection);
                if(g) {
                    addItem(g);
                    g->loadSettings(*profileSettings);
                    g->applySettings();
                } else {
                    DEBUG << "Error creating item of type" << profileSettings->value("type").toString();
                }
                profileSettings->endGroup();
            }
            // Force covers to update once all items have been added
            for(int i = 0; i < currentPanel->items()->count(); i++) {
                PanelItem *item = currentPanel->items()->at(i);
                if(item->itemType() == PanelItemTypeCover) item->setEditMode(editMode);
            }
            // Break out
            profileSettings->endGroup();
            emit panelsChanged();
            break;
        }
        profileSettings->endGroup();
    }
}

void PanelWindow::copyPanel(QString name) {
    if(existsPanel(name)) {
        //TODO

        emit panelsChanged();
    }
}

void PanelWindow::removePanel(QString name) {

    //TODO: is currently loaded?

    // Find correct settings group
    foreach(QString panelGroupName, getPanelGroupNames()) {
        profileSettings->beginGroup(panelGroupName);
        if(profileSettings->value("name") == name) {
            // Remove all keys for this group
            foreach(QString key, profileSettings->allKeys()) profileSettings->remove(key);
            profileSettings->endGroup();
            INFO << "Removed panel with name" << name;
            emit panelsChanged();
            return;
        }
        profileSettings->endGroup();
    }
}

QString PanelWindow::newPanel() {
    return newPanelWithName(QString::null);
}

QString PanelWindow::newPanelWithName(QString newName) {
    // Find a unique name
    if(newName == QString::null) {
        int panelNameNumber = 1;
        newName = QString("Panel%1").arg(panelNameNumber);
        while(existsPanel(newName)) {
            panelNameNumber++;
            newName = QString("Panel%1").arg(panelNameNumber);
        }
    }
    // Find the lowest panel number available
    int panelNumber = 0;
    while(panelNumber >= 0) {
        profileSettings->beginGroup("panel-" + QString::number(panelNumber));
        if(!profileSettings->contains("name")) {
            INFO << "New panel with name" << newName << "(#" << panelNumber << ")";
            profileSettings->setValue("name",newName);
            panelNumber = -1; // break out
        } else {
            panelNumber++;
        }
        profileSettings->endGroup();
    }
    loadPanel(newName);
    emit panelsChanged();
    return newName;
}

bool PanelWindow::existsPanel(QString name) {
    foreach(QString panelGroupName, getPanelGroupNames()) {
        profileSettings->beginGroup(panelGroupName);
        if(profileSettings->value("name") == name) {
            profileSettings->endGroup();
            return true;
        }
        profileSettings->endGroup();
    }
    return false;
}

void PanelWindow::clearPanel() {
    // Clear all panel items
    for(int i = 0; i < currentPanel->items()->count(); i++) {
        PanelItem *item = currentPanel->items()->at(i);
        item->deleteLater();
    }
    currentPanel->items()->clear();
}

void PanelWindow::prevPanel() {
    QStringList panelNames = getPanelNames();
    int newIndex = panelNames.indexOf(currentPanel->name)-1;
    if(newIndex >= 0 && newIndex < panelNames.count()) {
        loadPanel(panelNames.at(newIndex));
    }
}

void PanelWindow::nextPanel() {
    QStringList panelNames = getPanelNames();
    int newIndex = panelNames.indexOf(currentPanel->name)+1;
    if(newIndex >= 0 && newIndex < panelNames.count()) {
        loadPanel(panelNames.at(newIndex));
    }
}

// -------------------------------------------------------------------------------------
// -- Client Data Refs
// -------------------------------------------------------------------------------------

void PanelWindow::clientDataRefChanged(QString name, QString val) {
    if(name == AUTO_PANEL_DATAREF && appSettings->value("autopanels").toBool()) {
        INFO << "Aircraft changed:" << val;
        if(existsPanel(val)) {
            loadPanel(val);
        } else {
            newPanelWithName(val);
        }
    }
}

void PanelWindow::clientDataRefChanged(QString name, double val) {
    if(name == ADJUST_POWER_DATAREF && appSettings->value("adjustpower").toBool()) {
        INFO << "Power changed:" << val;
        currentPanel->hasAvionicsPower = val == 1;
    }
}
