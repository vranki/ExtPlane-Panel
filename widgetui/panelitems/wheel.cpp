#include "wheel.h"
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsSceneMouseEvent>
#include "../util/console.h"

#if defined(Q_OS_ANDROID)
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroid>
#endif

REGISTER_WITH_PANEL_ITEM_FACTORY(Wheel,"adjustment/wheel");

Wheel::Wheel(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeSwitch, PanelItemShapeCircular),
        _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(valueChanged(QString,double)));
    _value = 1;
    _change = 1;
    _minimum = 1;
    _maximum = 100;
    _valueStartPoint = _minimum;
    _knobcolor = Qt::red;
    _index1set = true;
    _index2set = false;
    _index3set = false;
    _index4set = false;
    _index5set = false;
    _index6set = false;
    _index7set = false;
    _index8set = false;
    _draggedPixels =0;
    _turns = 2;
    _ref = 0;
    setSize(70,200);
}

void Wheel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);
    float myHeight = height();
    float myWidth = width();
    painter->setClipRect(0, 0, myWidth, myHeight);

    // Slider
    painter->save();
    painter->setPen(QPen(Qt::darkGray));
    painter->setBrush(Qt::darkGray);
    painter->drawRect((width() / 2) - (width() * 0.2f), 0, width() * 0.4f, height());
    painter->restore();

    // Wheel strips
    _stripDistance = height()/8;

//    INFO << "draggedPixels: " << _draggedPixels;
    for (int i=-1;i<9;i=i+1) {
        painter->save();
        // Constrain thick line within boundaries of gray backdrop
        painter->setClipRect((width() / 2) - (width() * 0.2f), 0, (width() * 0.4f), height());
        painter->translate((width() / 2) - (width() * 0.2f), _draggedPixels);

        // Calculate this strip's distance from center in pixels
        double distFromCenter;
        distFromCenter = abs(((height() / 2)-(_stripDistance*i)) - _draggedPixels);
        // INFO << "DistFromCenter: " << distFromCenter;

        // Calculate pen width for fake 3D effect
        double penwidth = (((height() / 2) - distFromCenter) / (height() / 2) * (height() * 0.05f)) + (height() * 0.01f);

        // Draw strip, from left to right
        painter->setPen(QPen(Qt::white, penwidth));
        painter->drawLine(0, _stripDistance*i, (width() * 0.4f), _stripDistance*i);

        painter->restore();
    }

    // Indicator triangle
    float indicatorHeight = 20;
    float travel = myHeight - indicatorHeight;
    int ypos = travel - (travel * ((_value - _minimum) / (_maximum - _minimum)));
    painter->save();
    painter->translate((width() / 2)-(width() * 0.2f)-20, ypos);
    painter->setPen(QPen(Qt::red, 0));
    painter->setBrush(Qt::red);
    static const QPointF indicator[3] = {
        QPointF(0.0, 0.0),
        QPointF(20.0, 10.0),
        QPointF(0.0, 20.0)
    };
    painter->drawPolygon(indicator, 3);
    painter->restore();

    PanelItem::paint(painter, option, widget);
}

void Wheel::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
    settings.setValue("dataref", _refname);
    settings.setValue("change", _change);
    settings.setValue("minimum", _minimum);
    settings.setValue("maximum", _maximum);
    settings.setValue("knobcolor", _knobcolor.rgba());
    settings.setValue("index1set", _index1set);
    settings.setValue("index2set", _index2set);
    settings.setValue("index3set", _index3set);
    settings.setValue("index4set", _index4set);
    settings.setValue("index5set", _index5set);
    settings.setValue("index6set", _index6set);
    settings.setValue("index7set", _index7set);
    settings.setValue("index8set", _index8set);
}

void Wheel::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    setRef(settings.value("dataref", "").toString());
    setChange(settings.value("change", 1).toDouble());
    setMinimum(settings.value("minimum", 1).toDouble());
    setMaximum(settings.value("maximum", 100).toDouble());

    QColor knobcolor;
    knobcolor.setRgba(settings.value("knobcolor").toUInt());
    setKnobColor(knobcolor);

    setIndex1Set(settings.value("index1set", true).toBool());
    setIndex2Set(settings.value("index2set", true).toBool());
    setIndex3Set(settings.value("index3set", true).toBool());
    setIndex4Set(settings.value("index4set", true).toBool());
    setIndex5Set(settings.value("index5set", true).toBool());
    setIndex6Set(settings.value("index6set", true).toBool());
    setIndex7Set(settings.value("index7set", true).toBool());
    setIndex8Set(settings.value("index8set", true).toBool());
    _valueStartPoint = _minimum;
}

void Wheel::createSettings(QGridLayout *layout) {
    layout->addWidget(new QLabel("Dataref", layout->parentWidget()));
    QLineEdit *refEdit = new QLineEdit(_refname, layout->parentWidget());
    connect(refEdit, SIGNAL(textChanged(QString)), this, SLOT(setRef(QString)));
    layout->addWidget(refEdit);

    layout->addWidget(new QLabel("Change amount", layout->parentWidget()));
    QLineEdit *changeEdit = new QLineEdit(QString::number(_change), layout->parentWidget());
    connect(changeEdit, SIGNAL(textChanged(QString)), this, SLOT(setChangeString(QString)));
    layout->addWidget(changeEdit);

    createNumberInputSetting(layout,"Minimum", _minimum, SLOT(setMinimum(float)));

    createNumberInputSetting(layout,"Maximum", _maximum, SLOT(setMaximum(float)));

    createColorSetting(layout, "Knob Color", _knobcolor, SLOT(setKnobColor(QColor)) );

    createCheckboxSetting( layout, "Write Array index 1", _index1set, SLOT(setIndex1Set(bool)));
    createCheckboxSetting( layout, "Write Array index 2", _index2set, SLOT(setIndex2Set(bool)));
    createCheckboxSetting( layout, "Write Array index 3", _index3set, SLOT(setIndex3Set(bool)));
    createCheckboxSetting( layout, "Write Array index 4", _index4set, SLOT(setIndex4Set(bool)));
    createCheckboxSetting( layout, "Write Array index 5", _index5set, SLOT(setIndex5Set(bool)));
    createCheckboxSetting( layout, "Write Array index 6", _index6set, SLOT(setIndex6Set(bool)));
    createCheckboxSetting( layout, "Write Array index 7", _index7set, SLOT(setIndex7Set(bool)));
    createCheckboxSetting( layout, "Write Array index 8", _index8set, SLOT(setIndex8Set(bool)));

}

void Wheel::applySettings() {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    if(!_refname.isEmpty())
        _ref = _client.subscribeDataRef(_refname, 0);
}

void Wheel::setChange(double ch) {
    _change = ch;
    update();
}

void Wheel::setChangeString(QString ch) {
    bool ok = false;
    double chDouble = ch.toDouble(&ok);
    if(ok)
        setChange(chDouble);
}

void Wheel::setMinimum(float ch) {
    _minimum = ch;
    if (_minimum > _maximum) {
        _minimum = _maximum;
    }
    update();
}

void Wheel::setMaximum(float ch) {
    _maximum = ch;
    if (_maximum < _minimum) {
        _maximum = _minimum;
    }
    update();
}

void Wheel::setKnobColor(QColor ch) {
    _knobcolor = ch;
}

void Wheel::setIndex1Set( bool input ) {
    _index1set = input;
}

void Wheel::setIndex2Set( bool input ) {
    _index2set = input;
}

void Wheel::setIndex3Set( bool input ) {
    _index3set = input;
}

void Wheel::setIndex4Set( bool input ) {
    _index4set = input;
}

void Wheel::setIndex5Set( bool input ) {
    _index5set = input;
}

void Wheel::setIndex6Set( bool input ) {
    _index6set = input;
}

void Wheel::setIndex7Set( bool input ) {
    _index7set = input;
}

void Wheel::setIndex8Set( bool input ) {
    _index8set = input;
}

void Wheel::setRef(QString txt) {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    _refname = txt;
    update();
}

void Wheel::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    INFO << "Wheel mousePressEvent.";
    if(isEditMode()) {
        PanelItem::mousePressEvent(event);
    } else {
        _dragStartPoint = event->pos();
        _dragStartPoint.setY(_dragStartPoint.y()-_draggedPixels);
        _valueStartPoint = _value;
        _dragValueStartPoint = event->pos();
    }
}

void Wheel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseReleaseEvent(event);
    } else {
        update();
    }
}
void Wheel::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseMoveEvent(event);
    } else {
        float travel = height() - 20;
        _value = _valueStartPoint + (((event->pos()-_dragValueStartPoint).y() * ((_maximum - _minimum) / travel)) / _turns);

        if (_value < _minimum) {
            _value = _minimum;
        } else if (_value > _maximum) {
            _value = _maximum;
        } else {
            double newoffset = (event->pos()-_dragStartPoint).y() / _stripDistance;
            _draggedPixels = (newoffset - floor(newoffset)) * _stripDistance;
        }
        if(_ref) {
            if (_index1set) {
                _ref->setValue(QString::number(_value), 0);
            }
            if (_index2set) {
                _ref->setValue(QString::number(_value), 1);
            }
            if (_index3set) {
                _ref->setValue(QString::number(_value), 2);
            }
            if (_index4set) {
                _ref->setValue(QString::number(_value), 3);
            }
            if (_index5set) {
                _ref->setValue(QString::number(_value), 4);
            }
            if (_index6set) {
                _ref->setValue(QString::number(_value), 5);
            }
            if (_index7set) {
                _ref->setValue(QString::number(_value), 7);
            }
            if (_index8set) {
                _ref->setValue(QString::number(_value), 8);
            }
        }
        update();
    }
}

void Wheel::valueChanged(QString ref, double newValue) {
    Q_ASSERT(ref==_refname);
    _value = newValue;
}
