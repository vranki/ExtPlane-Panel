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
#include "panelitem.h"
#include "panelitemfactory.h"

class MenuButton : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit MenuButton(QWidget *parent, QList<PanelItem*> &gaugelist, PanelItemFactory *gf);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
signals:
    void itemAdded(PanelItem *g);
public slots:
    void addItem();
    void deleteItems();
    void closeDialog();
    void savePanel();
    void loadPanel();
private:
    QList<PanelItem*> selectedGauges();
    int side;
    QWidget *parentWidget;
    QList<PanelItem*> &panelItems;
    QDialog *msg;
    PanelItemFactory *itemFactory;
    QSettings settings;
};

#endif // MENUBUTTON_H
