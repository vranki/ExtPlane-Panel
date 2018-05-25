#ifndef PANELITEM_H
#define PANELITEM_H

#include <QGraphicsItem>
#include <QSettings>
#include <QPainter>
#include <QGridLayout>
#include <QSlider>
#include <QLineEdit>
#include <QLabel>

// Not directly used by PanelItem, but included here to reduce
// code amount in all subclasses
#include "../util/units.h"
#include "extplaneconnection.h"
#include "extplaneclient.h"
#include "../panelitemfactory.h"
#include "../util/valueinterpolator.h"

// Definitions
#ifdef MOBILE_DEVICE
    #define SCALE_HANDLE_SIZE 50
#else
    #define SCALE_HANDLE_SIZE 20
#endif
#define SNAP_GRID_SIZE 10 // Must be even

// Typedefs
enum PanelItemType
{
    PanelItemTypeGauge,     // Panel Item which acts as a gauge or flight instrument
    PanelItemTypeSwitch,    // Panel Item which takes user input and has a state
    PanelItemTypeCover,     // Panel Item that acts as a cover for other items
    PanelItemTypeDisplay    // Panel Item which mainly acts as a display (such as a brake light)
};
enum PanelItemShape
{
    PanelItemShapeCircular,
    PanelItemShapeRectangular
};

// Forward declares
class ExtPlanePanel;

class PanelItem : public QObject, public QGraphicsItem {

    Q_OBJECT
    #if QT_VERSION >= 0x040600 // Doesn't work on 4.5
        Q_INTERFACES(QGraphicsItem)
    #endif

public:
    explicit PanelItem(ExtPlanePanel *panel, PanelItemType type, PanelItemShape shape);
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
    virtual void itemSizeChanged(float w, float h) { Q_UNUSED(w); Q_UNUSED(h) };
    bool isResizing();
    virtual void setEditMode(bool em);
    virtual void setPos(int x, int y);
    bool isEditMode();
    /**
     * Automatically implemented by panel items when using the REGISTER_WITH_PANEL_ITEM_FACTORY macro.
     * This method defines the display name for the type of gauge and is used by the panel item factory.
     */
    virtual QString typeName() = 0;
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);

    /**
     * Method for creating any additional settings which a panel item might need
     */
    virtual void createSettings(QGridLayout *layout);
    /**
     * Helper method for easily creating a slider setting (int)
     */
    void createSliderSetting(QGridLayout *layout, QString label, int minV, int maxV, int initialV, const char* slot);
    /**
     * Helper method for easily creating a checkbox setting (bool)
     */
    void createCheckboxSetting(QGridLayout *layout, QString label, bool initialValue, const char* slot);
    /**
     * Helper method for easily creating a line edit setting (QString)
     */
    void createLineEditSetting(QGridLayout *layout, QString label, QString initialValue, const char* slot);
    void createNumberInputSetting(QGridLayout *layout, QString label, float initialValue, const char* slot);
    /**
     * Helper method for easily creating a color setting (QString) with an option for a color picker
     */
    void createColorSetting(QGridLayout *layout, QString label, QColor initialValue, const char* slot);
    /**
     * Helper method for easily creating a combo box setting (int) with a list of QString items
     */
    void createComboBoxSetting(QGridLayout *layout, QString label, int initialValue, QStringList items, const char* slot);
    virtual void applySettings();
    inline PanelItemType itemType() { return _itemType; }
    inline PanelItemShape itemShape() { return _itemShape; }
    inline ExtPlanePanel* panel() { return _panel; }

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
    virtual void setPanelRotation(int angle);
    virtual void setItemRotation(int angle);
    virtual void setZValue(int z);
    virtual void tickTime(double dt, int total);
    virtual void setInterpolationEnabled(bool ie);
    virtual void setAntialiasEnabled(bool ie);
    virtual void setDefaultFontSize(double dfs);
    virtual void setItemFontSize(double ifs);
    /**
     * Called when a setting has changed via the UI for settings created with the helper methods.
     */
    virtual void settingChanged() {};

protected:
    void setItemType(PanelItemType type);
    void setItemShape(PanelItemShape shape);

private:
    PanelItemType _itemType;
    PanelItemShape _itemShape;
    ExtPlanePanel *_panel;
    float _width, _height;
    bool _resizing, _editMode, _antialiasingEnabled;
    int _panelRotation, _itemRotation;
    float _itemFontSize, _defaultFontSize;
};

#endif // PANELITEM_H
