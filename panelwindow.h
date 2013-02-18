#ifndef PANELWINDOW_H
#define PANELWINDOW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QSettings>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>

class PanelItem;
class MenuButton;
class SettingsDialog;
class ExtPlaneConnection;
class PanelItemFactory;
class EditItemDialog;

/**
 * @brief The PanelWindow class
 *
 * The QGraphicsView widget which acts as the main window containing
 * panel items and menu button in a  QGraphicsScene. Also contains
 * some main logic of the application (could be moved to separate class
 * if needed).
 *
 * @see PanelItem
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
    void addItem(PanelItem *g);
    void panelRotationChanged(int r);
    void fullscreenChanged(bool fs);
    void setServerAddress(QString host);
    void disableBlanking();
public slots:
    void setEditMode(bool em);
    void addItem();
    void savePanel();
    void savePanel(QString filename);
    void loadPanel();
    void loadPanel(QString filename);
    void newPanel();
    void showSettings();
    void editItem(PanelItem *item=0);
    void panelItemChanged(PanelItem *item=0); // Should be emitted by panel item when they become dirty
    void quit();
private slots:
    void tick();
protected:
    virtual void resizeEvent(QResizeEvent * event);
private:
    QList<PanelItem*> selectedGauges();
private:
    MenuButton *menuButton;
    SettingsDialog *settingsDialog;
    QGraphicsScene scene;
    QGraphicsTextItem statusMessage; // Displayed in panel
    int panelRotation; // Master rotation of the panel
    bool editMode; // True if in edit mode
    bool dirty; // True when any panel changes have occured
    ExtPlaneConnection *connection;
    QSettings *appSettings; // Loaded on app start, contains general settings
    QSettings *panelSettings; // Contains all PanelItem settings
    QList<PanelItem *> panelItems;
    PanelItemFactory *itemFactory;
    EditItemDialog *editItemDialog; // Only one open at a time
    QTimer tickTimer; // Timer to update items synchronously
    QTime time, totalTime;

    // Disables blanking on Maemo
#ifdef MAEMO
    QTimer blankingTimer;
#endif
};

#endif // PANELWINDOW_H
