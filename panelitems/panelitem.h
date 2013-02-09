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

#ifdef MOBILE_DEVICE
#define SCALE_HANDLE_SIZE 50
#else
#define SCALE_HANDLE_SIZE 20
#endif
#define SNAP_GRID_SIZE 10

class PanelItem : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit PanelItem(QObject *parent);
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
    virtual void applySettings();
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
signals:
    void panelItemSelected(PanelItem *g, bool sel=true);
    void editPanelItem(PanelItem *item);
    void panelItemChanged(PanelItem *item); //TODO: dankrusi: For warning when closing a dirty file without saving, this needs to be called by the panel items when settings are changed. I haven't found a nice way to do this automatically without going in and touching all the panel items...
public slots:
    void setPanelRotation(int angle);
    void setItemRotation(int angle);
    void setZValue(int z);
    virtual void tickTime(double dt, int total);
private:
    float _width, _height;
    bool resizing, _editMode;
    int _panelRotation, _itemRotation;
};





class PanelItemFactory
{
    typedef QMap<QString,const QMetaObject*> BaseFactoryMapType;

public:
    PanelItemFactory() { connection = NULL; };
    PanelItemFactory(ExtPlaneConnection *conn) { connection = conn; };

    PanelItem *itemForName(QString name, QObject *parentObject) {
	if(classMapping()->contains(name)) {
	    const QMetaObject *meta = classMapping()->find(name).value();
	    return (PanelItem*) (meta->newInstance(Q_ARG(QObject*,parentObject),Q_ARG(ExtPlaneConnection*,connection)));
	} else {
	    qWarning() << Q_FUNC_INFO << "the panel item " << name << "is not recognized";
	    return NULL;
	}
    };

    QStringList itemNames() {
	QStringList items;
	foreach(QString k,classMapping()->keys()) items << k;
	return items;
    };

private:
    ExtPlaneConnection *connection;

protected:
    static BaseFactoryMapType *classMapping() {
	static BaseFactoryMapType *_classMapping; // Statically allocated, lives for the entire period of the executable
	if(!_classMapping) { _classMapping = new BaseFactoryMapType; }
	return _classMapping;
    };
};


// Helper class which forces derived PanelItems which instante this to register with the PanelFactory
template<typename T>
struct RegisterWithPanelItemFactory : PanelItemFactory {
    RegisterWithPanelItemFactory(QString s) {
	classMapping()->insert(s, &T::staticMetaObject);
    }
};

// Helper macro used by derived classes of PanelItem to automatically register their name with the PanelFactory
#define REGISTER_WITH_PANEL_ITEM_FACTORY(CLASS,NAME) QString CLASS::typeName() {return NAME;} RegisterWithPanelItemFactory<CLASS> registerWithFactory_##CLASS(NAME);


#endif // PANELITEM_H
