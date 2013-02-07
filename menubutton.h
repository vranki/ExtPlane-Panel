#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>
#include <QCheckBox>
#include "panelitems/panelitem.h"
#include "panelitemfactory.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/edititemdialog.h"

class MenuButton : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit MenuButton(QWidget *parent);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
signals:
    void itemAdded(PanelItem *g);
    void panelRotationChanged(int r);
    void fullscreenChanged(bool fs);
    void setServerAddress(QString host);
public slots:
    void setEditMode(bool em);
    void closeCurrentMenu();
private:
    int side;
    bool editMode;
    QWidget *panelWindow;
    QDialog *currentMenu;
    SettingsDialog *settingsDialog;
};

#endif // MENUBUTTON_H
