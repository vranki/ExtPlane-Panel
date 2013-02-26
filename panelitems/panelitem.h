#ifndef PANELITEM_H
#define PANELITEM_H

#include <QGraphicsItem>
#include <QSettings>

// Not directly used by PanelItem, but included here to reduce
// code amount in all subclasses
#include <QPainter>
#include <QGridLayout>
#include "units.h"
#include "../valueinterpolator.h"
#include "extplaneconnection.h"
#include "extplaneclient.h"
#include "panelitemfactory.h"


#ifdef MOBILE_DEVICE
#define SCALE_HANDLE_SIZE 50
#else
#define SCALE_HANDLE_SIZE 20
#endif
#define SNAP_GRID_SIZE 10

class PanelItem : public QObject, public QGraphicsItem {
    Q_OBJECT
#if QT_VERSION >= 0x040600 // Doesn't work on 4.5
    Q_INTERFACES(QGraphicsItem)
#endif 
public:
    explicit PanelItem(QObject *parent);
    ~PanelItem();
    virtual QRectF boundingRect() const;
    /**
     * The paint method which repaints the item. Try to optimize it if implementing
     * complex instruments. Call setupPainter() to set correct rendering hints.
     */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setSize(float w, float h);
    float width() const;
    float height() const;
    int itemRotation();
    float itemFontSize();
    /**
     * This is called whenever the panel item has changed size (including when first added to the scene).
     * Panel items which cache resources or pre-render complicated stuff should use this method for doing so.
     */
    virtual void itemSizeChanged(float w, float h) {};
    void setEditMode(bool em);
    bool isEditMode();
    /**
     * Automatically implemented by panel items when using the REGISTER_WITH_PANEL_ITEM_FACTORY macro.
     * This method defines the display name for the type of gauge and is used by the panel item factory.
     */
    virtual QString typeName() = 0;
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
    virtual void applySettings();
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    /**
     * Sets correct antialias and other rendering settings configured by user.
     *
     * Call before painting stuff in paint().
     */
    void setupPainter(QPainter *painter); //
    QFont defaultFont;

    // Use for mechanical instruments
    QColor darkGrayColor;
signals:
    void panelItemSelected(PanelItem *g, bool sel=true);
    void editPanelItem(PanelItem *item);
    /**
     * @TODO: dankrusi: For warning when closing a dirty file without saving, this needs to be called by
     * the panel items when settings are changed. I haven't found a nice way to do this automatically without
     * going in and touching all the panel items...
     */
    void panelItemChanged(PanelItem *item);
public slots:
    void setPanelRotation(int angle);
    void setItemRotation(int angle);
    void setZValue(int z);
    virtual void tickTime(double dt, int total);
    virtual void setInterpolationEnabled(bool ie);
    virtual void setAntialiasEnabled(bool ie);
    virtual void setDefaultFontSize(double dfs);
    virtual void setItemFontSize(double ifs);
private:
    float _width, _height;
    bool resizing, _editMode, _aaEnabled;
    int _panelRotation, _itemRotation;
    float _itemFontSize, _defaultFontSize;
};

#endif // PANELITEM_H
