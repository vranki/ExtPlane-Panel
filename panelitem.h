#ifndef PANELITEM_H
#define PANELITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QVariant>
#include <QSettings>
#include "extplaneclient.h"
#include "extplaneconnection.h"

#define SCALE_HANDLE_SIZE 10
#define SNAP_GRID_SIZE 10

class PanelItem : public QObject, public QGraphicsItem{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit PanelItem(QObject *parent);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    float width() const;
    float height() const;
    void setSize(float w, float h);
    virtual QString typeName() = 0;
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
signals:
    void panelItemSelected(PanelItem *g, bool sel=true);
public slots:
    void setPanelRotation(float angle);
    void setItemRotation(float angle);
private:
    float _width, _height;
    bool resizing;
    float _panelRotation, _itemRotation;
};

#endif // GAUGE_H
