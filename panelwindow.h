#ifndef PANELWINDOW_H
#define PANELWINDOW_H

#ifdef MAEMO
#include <QDBusConnection>
#include <QDBusMessage>
#include "mce/mode-names.h"
#include "mce/dbus-names.h"
#endif

#include <QTimer>
#include <QTime>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsItemGroup>
#include <QObject>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QInputDialog>
#include <QFileDialog>
#include "extplaneconnection.h"
#include "simulatedextplaneconnection.h"
#include "menubutton.h"
#include "dialogs/settingsdialog.h"


class PanelWindow : public QGraphicsView {
    Q_OBJECT

public:
    explicit PanelWindow();
    ~PanelWindow();
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
    QGraphicsTextItem errorMessage;

    int panelRotation;
    bool editMode;
    bool dirty; // True when any panel changes have occured
    ExtPlaneConnection *connection;
    QSettings *appSettings; // Loaded on app start, contains general settings
    QSettings *panelSettings; // Contains all PanelItem settings
    QList<PanelItem *> panelItems;
    PanelItemFactory *itemFactory;
    EditItemDialog *editItemDialog; // Only one open at a time
    QTimer blankingTimer, tickTimer;
    QTime time, totalTime;
};

#endif // PANELWINDOW_H
