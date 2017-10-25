#include "panelitem.h"

#include <QPainter>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QVariant>
#include <QGridLayout>
#include <QApplication>
#include <QPushButton>
#include <QColorDialog>
#include <QCheckBox>
#include <QComboBox>

#include "../panel.h"
#include "../widgets/colorselector.h"
#include "../widgets/numberinputlineedit.h"

PanelItem::PanelItem(ExtPlanePanel *panel, PanelItemType type, PanelItemShape shape) : QObject(panel), QGraphicsItem(), darkGrayColor(30,30,30) {
    // Init
    _panel = panel;
    _itemType = type;
    _itemShape = shape;
    _width = _height = 200;
    _panelRotation = _itemRotation = 0;
    _itemFontSize = 0;
    _defaultFontSize = 10;
    defaultFont = QApplication::font();
    setEditMode(false);
}

PanelItem::~PanelItem() {
}

void PanelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(isSelected()) {
        painter->setPen(Qt::yellow);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(0,0,width(), height());
        painter->drawLine(0,0,width(),height());
        painter->drawLine(0,height(),width(),0);
        painter->drawRect(width()-SCALE_HANDLE_SIZE, height()-SCALE_HANDLE_SIZE, SCALE_HANDLE_SIZE, SCALE_HANDLE_SIZE);
    } else if(isEditMode()) {
        painter->setOpacity(0.3);
        painter->setPen(Qt::yellow);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(0,0,width(), height());
        painter->drawLine(0,0,width(),height());
        painter->drawLine(0,height(),width(),0);
        painter->setOpacity(1.0);
    }
    if(!panel()->hasAvionicsPower && (this->itemType() == PanelItemTypeGauge || this->itemType() == PanelItemTypeSwitch)) {
        // Dim the entire panel
        painter->setOpacity(0.5);
        painter->fillRect(0,0,this->width(),this->height(),Qt::black);
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
        _resizing = true;
    }
}

void PanelItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) {
    if(_resizing) {
        float w = (event->scenePos() - pos()).x();
        float h = (event->scenePos() - pos()).y();
        setSize(w,h);
    } else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void PanelItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(_resizing) _resizing = false;
    int nposx = pos().x();
    int nposy = pos().y();
    int snapSize = SNAP_GRID_SIZE;

    int snapx = (nposx % snapSize);
    if(snapx <= snapSize/2) nposx -= snapx;
    else                    nposx += (snapSize-snapx);

    int snapy = (nposy % snapSize);
    if(snapy <= snapSize/2) nposy -= snapy;
    else                    nposy += (snapSize-snapy);
    setPos(nposx, nposy);

    QGraphicsItem::mouseReleaseEvent(event);
}

void PanelItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
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
    painter->setRenderHints(_antialiasingEnabled ? aaRenderhints : noaaRenderhints);
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
    if(_itemType == PanelItemTypeCover) {
        // Covers can't change z-value, they are awlways ontop (editmode=false) or onbottom (editmode=true)
        if(_editMode)   QGraphicsItem::setZValue(-PANEL_PANELITEM_COVER_ZVALUE);
        else            QGraphicsItem::setZValue(+PANEL_PANELITEM_COVER_ZVALUE);
        return;
    }
    QGraphicsItem::setZValue(z);
}

void PanelItem::createSettings(QGridLayout *layout) {
    Q_UNUSED(layout);
}

void PanelItem::setEditMode(bool em) {
    // Setup the graphics item for editing
    _editMode = em;
    setFlag(QGraphicsItem::ItemIsMovable, em);
    setFlag(QGraphicsItem::ItemIsSelectable, em);
    _resizing = false;

    // Update z value for covers (they are fixed depending on edit mode)
    if(_itemType == PanelItemTypeCover) setZValue(0);
}

void PanelItem::setPos(int x, int y) {
    QGraphicsItem::setPos(x,y);
}

void PanelItem::createSliderSetting(QGridLayout *layout, QString label, int minV, int maxV, int initialV, const char* slot) {
    layout->addWidget(new QLabel(label, layout->parentWidget()));
    QSlider *widget = new QSlider(Qt::Horizontal,layout->parentWidget());
    widget->setRange(minV,maxV);
    widget->setValue(initialV);
    layout->addWidget(widget);
    connect(widget, SIGNAL(valueChanged(int)), this, slot);
    connect(widget, SIGNAL(valueChanged(int)), this, SLOT(settingChanged()));
}

void PanelItem::createCheckboxSetting(QGridLayout *layout, QString label, bool initialValue, const char* slot) {
    layout->addWidget(new QLabel(label, layout->parentWidget()));
    QCheckBox *widget = new QCheckBox(layout->parentWidget());
    widget->setChecked(initialValue);
    layout->addWidget(widget);
    connect(widget, SIGNAL(clicked(bool)), this, slot);
    connect(widget, SIGNAL(clicked(bool)), this, SLOT(settingChanged()));
}

void PanelItem::createLineEditSetting(QGridLayout *layout, QString label, QString initialValue, const char* slot) {
    layout->addWidget(new QLabel(label, layout->parentWidget()));
    QLineEdit *widget = new QLineEdit(layout->parentWidget());
    widget->setText(initialValue);
    layout->addWidget(widget);
    connect(widget, SIGNAL(textChanged(QString)), this, slot);
    connect(widget, SIGNAL(textChanged(QString)), this, SLOT(settingChanged()));
}

void PanelItem::createNumberInputSetting(QGridLayout *layout, QString label, float initialValue, const char* slot) {
    layout->addWidget(new QLabel(label, layout->parentWidget()));
    NumberInputLineEdit *widget = new NumberInputLineEdit(layout->parentWidget());
    widget->setText(QString::number(initialValue));
    layout->addWidget(widget);
    connect(widget, SIGNAL(valueChangedFloat(float)), this, slot);
    connect(widget, SIGNAL(valueChangedFloat(float)), this, SLOT(settingChanged()));
}

void PanelItem::createColorSetting(QGridLayout *layout, QString label, QColor initialValue, const char* slot) {
    layout->addWidget(new QLabel(label, layout->parentWidget()));
    ColorSelector *widget = new ColorSelector(layout->parentWidget());
    widget->setColor(initialValue);
    layout->addWidget(widget);
    connect(widget, SIGNAL(colorChanged(QColor)), this, slot);
    connect(widget, SIGNAL(colorChanged(QColor)), this, SLOT(settingChanged()));
}

void PanelItem::createComboBoxSetting(QGridLayout *layout, QString label, int initialValue, QStringList items, const char* slot) {
    layout->addWidget(new QLabel(label, layout->parentWidget()));
    QComboBox *widget = new QComboBox(layout->parentWidget());
    widget->addItems(items);
    widget->setCurrentIndex(initialValue);
    layout->addWidget(widget);
    connect(widget, SIGNAL(currentIndexChanged(int)), this, slot);
    connect(widget, SIGNAL(currentIndexChanged(int)), this, SLOT(settingChanged()));
}


bool PanelItem::isEditMode() {
    return _editMode;
}

bool PanelItem::isResizing() {
    return _resizing;
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

void PanelItem::setItemType(PanelItemType type) {
    _itemType = type;
}

void PanelItem::setItemShape(PanelItemShape shape) {
    _itemShape = shape;
}

void PanelItem::tickTime(double dt, int total) {
    Q_UNUSED(dt);
    Q_UNUSED(total);
}

void PanelItem::setInterpolationEnabled(bool ie) {
    Q_UNUSED(ie);
}

void PanelItem::setAntialiasEnabled(bool ie) {
    _antialiasingEnabled = ie;
    itemSizeChanged(width(), height()); // Force repaint of pixmaps
}
