#ifndef PANELWINDOW_H
#define PANELWINDOW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QSettings>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QList>

#include "panelitemfactory.h"
#include "../settings.h"
#include "panel.h"

class PanelItem;
class MenuButton;
class SettingsDialog;
class ExtPlaneConnection;
class ExtPlaneClient;
class EditItemDialog;
class HardwareDialog;
class PanelsDialog;
class HardwareManager;

/**
 * @brief The PanelWindow class
 *
 * The QGraphicsView widget which acts as the main window containing
 * panel items and menu button in a QGraphicsScene. Also contains
 * some main logic of the application (could be moved to separate class
 * if needed, ExtPlanePanel).
 *
 * @see PanelItem, ExtPlanePanel
 */
class PanelWindow : public QGraphicsView {
    Q_OBJECT

public:
    explicit PanelWindow();
    ~PanelWindow();
    virtual void keyPressEvent( QKeyEvent * event );
signals:
    void tickTime(double dt, int total);
    void panelsChanged();
public slots:
    void connectionMessage(QString txt);
    void itemDestroyed(QObject *obj);
    void addItem(PanelItem *item);
    void addItem(QString itemName);
    void panelRotationChanged(int r);
    void fullscreenChanged(bool fs);
    void setServerAddress(QString host);
    void disableBlanking();
public slots:
    void setEditMode(bool em);
    void showAddItemDialog();
    void setProfileSettings(QSettings *settings);
    void saveProfile();
    void saveProfile(QString filename);
    void saveProfileAs();
    void loadProfile();
    void loadProfile(QString filename);
    void newProfile();
    void showHardware();
    void showSettings();
    void showPanels();
    void editItem(PanelItem *item=0);
    void panelItemChanged(PanelItem *item=0); // Should be emitted by panel item when they become dirty
    void quit();
    void clientDataRefChanged(QString name, QString val);
    void clientDataRefChanged(QString name, double val);
private slots:
    QStringList getPanelGroupNames();
    QStringList getPanelNames();
    void saveCurrentPanel();
    void loadPanel(QString name); // A null string will load the first panel found
    void copyPanel(QString name);
    void removePanel(QString name);
    QString newPanel();
    QString newPanelWithName(QString newName);
    void clearPanel();
    void prevPanel();
    void nextPanel();
    bool existsPanel(QString name);
    void tick();
    void setInterpolationEnabled(bool enabled);
    void setAntialiasEnabled(bool enabled);
    void setAutoPanelsEnabled(bool enabled);
    void setAdjustPowerEnabled(bool enabled);
    void setPanelUpdateInterval(double newInterval);
    void setDefaultFontSize(double newFs);
protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);
private:
    QList<PanelItem*> selectedGauges();
private:
    ExtPlaneConnection *connection; // The master connection which all panel item clients will use
    ExtPlanePanel *currentPanel; // Contains the panel items, exposed to panel items so they can know about each other (for example the covers)
    ExtPlaneClient *client; // Client for performing universal action such as automatically chaning the panel when aircraft changes, dimming lights, et cetera
    HardwareManager *hwManager;
    MenuButton *menuButton;
    SettingsDialog *settingsDialog;
    HardwareDialog *hardwareDialog;
    PanelsDialog *panelsDialog;
    QGraphicsScene scene;
    QGraphicsTextItem statusMessage; // Displayed in panel
    bool editMode; // True if in edit mode
    bool dirty; // True when any panel changes have occured
    PanelItemFactory itemFactory;
    Settings *appSettings; // Loaded on app start, contains general settings, passed on to settings dialog. Use valueFromSettingsOrCommandLine to extract settings from both the file and command line.
    QSettings *profileSettings; // Loaded when a profile is loaded, contains settings related to a single profile (which may contain multiple panels).
    EditItemDialog *editItemDialog; // Only one open at a time
    QTimer tickTimer; // Timer to update items synchronously
    QTime time, totalTime;
    bool interpolationEnabled, aaEnabled;
    double defaultFontSize;
    #ifdef MAEMO
        QTimer blankingTimer; // Disables blanking on Maemo
    #endif
};

#endif // PANELWINDOW_H
