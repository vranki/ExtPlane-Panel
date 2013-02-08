#include "panelitem.h"

PanelItem::PanelItem(QObject *parent) : QObject(parent), QGraphicsItem() {
    _width = _height = 200;
    setEditMode(false);
    _panelRotation = _itemRotation = 0;
}

void PanelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if(isSelected()) {
        painter->setPen(Qt::yellow);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(0,0,width(), height());
        painter->drawRect(width()-SCALE_HANDLE_SIZE, height()-SCALE_HANDLE_SIZE,
                          SCALE_HANDLE_SIZE, SCALE_HANDLE_SIZE);
    } else if(isEditMode()) {
	painter->setOpacity(0.1);
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
    if(w < SCALE_HANDLE_SIZE) w = SCALE_HANDLE_SIZE;
    if(h < SCALE_HANDLE_SIZE) h = SCALE_HANDLE_SIZE;
    int nw = w;
    int nh = h;
    nw /= SNAP_GRID_SIZE;
    nw *= SNAP_GRID_SIZE;
    nh /= SNAP_GRID_SIZE;
    nh *= SNAP_GRID_SIZE;
    _width = nw;
    _height = nh;
    setTransformOriginPoint(width()/2, height()/2);
    update();
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

void PanelItem::storeSettings(QSettings &settings) {
    settings.setValue("type", typeName());
    settings.setValue("pos_x", pos().x());
    settings.setValue("pos_y", pos().y());
    settings.setValue("width", (int) width());
    settings.setValue("height", (int) height());
    settings.setValue("rotation", _itemRotation);
    settings.setValue("zvalue", zValue());
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
}

void PanelItem::setPanelRotation(int angle) {
    _panelRotation = angle;
    setRotation(_panelRotation + _itemRotation);
}

void PanelItem::setItemRotation(int angle) {
    _itemRotation = angle;
    setRotation(_panelRotation + _itemRotation);
}

int PanelItem::itemRotation() {
    return _itemRotation;
}

void PanelItem::setZValue(int z) {
    QGraphicsItem::setZValue(z);
}

void PanelItem::createSettings(QGridLayout *layout) {

}

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

void PanelItem::tickTime(double dt, int total) {}
