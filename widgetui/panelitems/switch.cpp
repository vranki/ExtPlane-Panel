#include "switch.h"
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include "../widgets/numberinputlineedit.h"
#include "extplaneclient.h"
#include "../util/console.h"
#include <QGraphicsSceneMouseEvent>

REGISTER_WITH_PANEL_ITEM_FACTORY(Switch,"switches/generic");

Switch::Switch(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeSwitch, PanelItemShapeRectangular),
        _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(valueChanged(QString,double)));
    _value = false;
    _label = "Switch";
    _ref = 0;
    setSize(100,30);
}

void Switch::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);
    painter->setBrush(Qt::gray);
    painter->setPen(Qt::gray);

    double circleSize = height()/3;
    painter->drawEllipse(0, height()/2-circleSize/2, circleSize, circleSize);

    painter->save();
    switchWidth = height()/3;
    painter->translate(switchWidth/2, height()/2);

    QPolygon p;
    p << QPoint(-switchWidth/4, 0) << QPoint(switchWidth/4, 0)
      << QPoint(switchWidth/2, height()/2) << QPoint(-switchWidth/2, height()/2);
    if(_value)
        painter->scale(1,-1);
    if(_value) {
        painter->setBrush(Qt::darkGray);
    } else {
        painter->setBrush(Qt::lightGray);
    }
    painter->drawPolygon(p);

    painter->restore();
    painter->setPen(Qt::white);
    QFont font = defaultFont;
    font.setPixelSize(height()*0.75);
    painter->setFont(font);
    painter->drawText(QRect(switchWidth,0,width()-switchWidth, height()), Qt::AlignCenter, _label);
    PanelItem::paint(painter, option, widget);
}

void Switch::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("label", _label);
    settings.setValue("dataref", _refname);
}

void Switch::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    setLabel(settings.value("label", "Switch").toString());
    setRef(settings.value("dataref", "").toString());
}

void Switch::createSettings(QGridLayout *layout) {
    layout->addWidget(new QLabel("Label", layout->parentWidget()));
    QLineEdit *labelEdit = new QLineEdit(_label, layout->parentWidget());
    connect(labelEdit, SIGNAL(textChanged(QString)), this, SLOT(setLabel(QString)));
    layout->addWidget(labelEdit);

    layout->addWidget(new QLabel("Dataref", layout->parentWidget()));
    QLineEdit *refEdit = new QLineEdit(_refname, layout->parentWidget());
    connect(refEdit, SIGNAL(textChanged(QString)), this, SLOT(setRef(QString)));
    layout->addWidget(refEdit);
}

void Switch::applySettings() {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    if(!_refname.isEmpty())
        _ref = _client.subscribeDataRef(_refname, 0);
}

void Switch::setLabel(QString txt) {
    _label = txt;
    update();
}

void Switch::setRef(QString txt) {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    _refname = txt;
    update();
}

void Switch::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    if(isEditMode()) {
        PanelItem::mousePressEvent(event);
    }
}

void Switch::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseReleaseEvent(event);
    } else if(event->pos().x() < switchWidth) {
        _value = !_value;
        if(_ref) _ref->setValue(_value ? "1" : "0");
        update();
    }
}

void Switch::valueChanged(QString ref, double newValue) {
    DEBUG << ref << newValue;
    Q_ASSERT(ref==_refname);
    _value = newValue != 0;
}
