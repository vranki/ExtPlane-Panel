#ifndef PANELWINDOW_H
#define PANELWINDOW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QSettings>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>

#include "panelitemfactory.h"
#include "settings.h"
#include "panel.h"

class PanelItem;
class MenuButton;
class SettingsDialog;
class ExtPlaneConnection;
class EditItemDialog;
class HardwareDialog;
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
    void saveProfile();
    void saveProfile(QString filename);
    void saveProfileAs();
    void loadProfile();
    void loadProfile(QString filename);
    void newProfile();
    void showHardware();
    void showSettings();
    void editItem(PanelItem *item=0);
    void panelItemChanged(PanelItem *item=0); // Should be emitted by panel item when they become dirty
    void quit();
private slots:
    void tick();
    void setInterpolationEnabled(bool ie);
    void setAntialiasEnabled(bool ie);
    void setPanelUpdateInterval(double newInterval);
    void setDefaultFontSize(double newFs);
protected:
    virtual void resizeEvent(QResizeEvent * event);
private:
    QList<PanelItem*> selectedGauges();
private:
    ExtPlanePanel *panel; // Contains the panel items
    HardwareManager *hwManager;
    MenuButton *menuButton;
    SettingsDialog *settingsDialog;
    HardwareDialog *hardwareDialog;
    QGraphicsScene scene;
    QGraphicsTextItem statusMessage; // Displayed in panel
    bool editMode; // True if in edit mode
    bool dirty; // True when any panel changes have occured
    ExtPlaneConnection *connection;
    PanelItemFactory itemFactory;
    Settings *appSettings; // Loaded on app start, contains general settings, passed on to settings dialog. Use valueFromSettingsOrCommandLine to extract settings from both the file and command line.
    EditItemDialog *editItemDialog; // Only one open at a time
    QTimer tickTimer; // Timer to update items synchronously
    QTime time, totalTime;
    bool interpolationEnabled, aaEnabled;
    double defaultFontSize;
    #ifdef MAEMO
        QTimer blankingTimer; // Disables blanking on Maemo
    #endif


        //QSettings *panelSettings; // Contains all PanelItem settings
        //PanelItemFactory itemFactory;
        //QList<PanelItem *> panelItems;
        //int panelRotation; // Master rotation of the panel
};

#endif // PANELWINDOW_H
