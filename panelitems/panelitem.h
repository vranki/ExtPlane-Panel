#ifndef PANELITEM_H
#define PANELITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QVariant>
#include <QSettings>
#include <QGridLayout>
#include "extplaneclient.h"
#include "extplaneconnection.h"

#define SCALE_HANDLE_SIZE 20
#define SNAP_GRID_SIZE 10

class PanelItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit PanelItem(QObject *parent);
    ~PanelItem();
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    float width() const;
    float height() const;
    void setSize(float w, float h);
    void setEditMode(bool em);
    bool isEditMode();
    virtual QString typeName() = 0;
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    int itemRotation();
    virtual void createSettings(QGridLayout *layout);
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
signals:
    void panelItemSelected(PanelItem *g, bool sel=true);
public slots:
    void setPanelRotation(int angle);
    void setItemRotation(int angle);
    void setZValue(int z);
private:
    float _width, _height;
    bool resizing, _editMode;
    int _panelRotation, _itemRotation;

};

#endif // GAUGE_H
