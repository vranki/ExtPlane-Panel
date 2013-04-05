#include "panelitem.h"
#include <QPainter>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QVariant>
#include <QGridLayout>
#include <QApplication>

PanelItem::PanelItem(QObject *parent) : QObject(parent), QGraphicsItem(), darkGrayColor(30,30,30) {
    _width = _height = 200;
    setEditMode(false);
    _panelRotation = _itemRotation = 0;
    _itemFontSize = 0;
    _defaultFontSize = 10;
    defaultFont = QApplication::font();
}

PanelItem::~PanelItem() {
}

void PanelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(isSelected()) {
        painter->setPen(Qt::yellow);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(0,0,width(), height());
        painter->drawRect(width()-SCALE_HANDLE_SIZE, height()-SCALE_HANDLE_SIZE,
                          SCALE_HANDLE_SIZE, SCALE_HANDLE_SIZE);
    } else if(isEditMode()) {
        painter->setOpacity(0.3);
	painter->setPen(Qt::yellow);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(0,0,width(), height());
	painter->setOpacity(1.0);
    }
}

float PanelItem::width() const {
    return _width;
}

float PanelItem::height() const {
    return _height;
}

QRectF PanelItem::boundingRect() const {
    return QRectF(0, 0 , width(), height());
}

void PanelItem::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    QGraphicsItem::mousePressEvent(event);
    if(event->pos().x() > width() - SCALE_HANDLE_SIZE && event->pos().y() > height() - SCALE_HANDLE_SIZE) {
        resizing = true;
    }
}

void PanelItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) {
    if(resizing) {
        float w = (event->scenePos() - pos()).x();
        float h = (event->scenePos() - pos()).y();
        setSize(w,h);
    } else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void PanelItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(resizing) {
        resizing = false;
    }
    int nposx = pos().x();
    int nposy = pos().y();
    nposx = nposx/SNAP_GRID_SIZE;
    nposx = nposx * SNAP_GRID_SIZE;
    nposy = nposy/SNAP_GRID_SIZE;
    nposy = nposy * SNAP_GRID_SIZE;
    setPos(nposx, nposy);

    QGraphicsItem::mouseReleaseEvent(event);
}

void PanelItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    emit editPanelItem(this);
}

void PanelItem::setSize(float w, float h) {
    w = qMax(w, (float) SCALE_HANDLE_SIZE);
    h = qMax(h, (float) SCALE_HANDLE_SIZE);
    int nw = w;
    int nh = h;
    nw /= SNAP_GRID_SIZE;
    nw *= SNAP_GRID_SIZE;
    nh /= SNAP_GRID_SIZE;
    nh *= SNAP_GRID_SIZE;
    _width = nw;
    _height = nh;
    itemSizeChanged(nw,nh); // Allow panel item to update any resources it needs for the new size...
// Supported from >=4.6
#if QT_VERSION >= 0x040600
    setTransformOriginPoint(width()/2, height()/2);
#endif
    update();
    if(scene())
        scene()->update();
}

QVariant PanelItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemSelectedChange && scene()) {
        bool newSelected = value.toBool();
        if(newSelected) {
            emit panelItemSelected(this);
        } else {
            emit panelItemSelected(0);
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void PanelItem::setupPainter(QPainter *painter) {
    static QPainter::RenderHints aaRenderhints = QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform;
    static QPainter::RenderHints noaaRenderhints = 0;
    painter->setRenderHints(_aaEnabled ? aaRenderhints : noaaRenderhints);
}

void PanelItem::storeSettings(QSettings &settings) {
    settings.setValue("type", typeName());
    settings.setValue("pos_x", pos().x());
    settings.setValue("pos_y", pos().y());
    settings.setValue("width", (int) width());
    settings.setValue("height", (int) height());
    settings.setValue("rotation", _itemRotation);
    settings.setValue("zvalue", zValue());
    settings.setValue("item_font_size", itemFontSize());
}

void PanelItem::loadSettings(QSettings &settings) {
    int posx = settings.value("pos_x").toInt();
    int posy = settings.value("pos_y").toInt();
    setPos(posx, posy);
    int w = settings.value("width").toInt();
    int h = settings.value("height").toInt();
    setSize(w, h);
    setItemRotation(settings.value("rotation", 0).toInt());
    setZValue(settings.value("zvalue", 0).toInt());
    setItemFontSize(settings.value("item_font_size", 0).toDouble());
}

void PanelItem::setPanelRotation(int angle) {
    _panelRotation = angle;
// Supported from >=4.6
#if QT_VERSION >= 0x040600
    setRotation(_panelRotation + _itemRotation);
#endif
}

void PanelItem::setItemRotation(int angle) {
    _itemRotation = angle;
#if QT_VERSION >= 0x040600
    setRotation(_panelRotation + _itemRotation);
#endif
}

int PanelItem::itemRotation() {
    return _itemRotation;
}

float PanelItem::itemFontSize() {
    return _itemFontSize;
}

void PanelItem::setZValue(int z) {
    QGraphicsItem::setZValue(z);
}

void PanelItem::createSettings(QGridLayout *layout) {}

void PanelItem::setEditMode(bool em) {
    _editMode = em;
    setFlag(QGraphicsItem::ItemIsMovable, em);
    setFlag(QGraphicsItem::ItemIsSelectable, em);
    resizing = false;
}

bool PanelItem::isEditMode() {
    return _editMode;
}

void PanelItem::applySettings() {
}

void PanelItem::setDefaultFontSize(double dfs) {
    _defaultFontSize = dfs;
    defaultFont.setPointSizeF(qMax(_defaultFontSize + _itemFontSize, 5.f)); // Limit font size to 5
    itemSizeChanged(width(), height()); // Force repaint of pixmaps
}

void PanelItem::setItemFontSize(double ifs) {
    _itemFontSize = ifs;
    setDefaultFontSize(_defaultFontSize);
}

void PanelItem::tickTime(double dt, int total) {}

void PanelItem::setInterpolationEnabled(bool ie) {}

void PanelItem::setAntialiasEnabled(bool ie) {
    _aaEnabled = ie;
    itemSizeChanged(width(), height()); // Force repaint of pixmaps
}
