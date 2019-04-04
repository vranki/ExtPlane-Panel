#include "slider.h"
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsSceneMouseEvent>
#include "../util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(Slider,"adjustment/slider_vertical");

Slider::Slider(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
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
    _ref = 0;
    setSize(70,200);
}

void Slider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);
    float myHeight = height();
    float myWidth = width();
    painter->setClipRect(0, 0, myWidth, myHeight);

    // Slider
    painter->save();
    painter->setPen(QPen(Qt::darkGray, (width() * 0.4f)));
    painter->translate(width() / 2, 0);
    painter->drawLine(0, 0, 0, height());
    painter->restore();

    // Slider knob
    float knobWidth = width(); // myWidth * 0.30f;
    float knobHeight = myHeight * 0.20f;
    float travel = myHeight - knobHeight;
    int ypos = travel - (travel * ((_value - _minimum) / (_maximum - _minimum)));
    painter->save();
    painter->setPen(_knobcolor);
    painter->setBrush(_knobcolor);
    painter->translate((width() / 2)-(knobWidth/2), 0);
//    Uncomment this if you want a rectangular knob
//    painter->drawRect(0, ypos, knobWidth, knobHeight);
    painter->drawEllipse(0, ypos, knobWidth, knobHeight);
    painter->restore();

    PanelItem::paint(painter, option, widget);
}

void Slider::storeSettings(QSettings &settings) {
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
}

void Slider::loadSettings(QSettings &settings) {
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
    _valueStartPoint = _minimum;
}

void Slider::createSettings(QGridLayout *layout) {
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

}

void Slider::applySettings() {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    if(!_refname.isEmpty())
        _ref = _client.subscribeDataRef(_refname, 0);
}

void Slider::setChange(double ch) {
    _change = ch;
    update();
}

void Slider::setChangeString(QString ch) {
    bool ok = false;
    double chDouble = ch.toDouble(&ok);
    if(ok)
        setChange(chDouble);
}

void Slider::setMinimum(float ch) {
    _minimum = ch;
    if (_minimum > _maximum) {
        _minimum = _maximum;
    }
    update();
}

void Slider::setMaximum(float ch) {
    _maximum = ch;
    if (_maximum < _minimum) {
        _maximum = _minimum;
    }
    update();
}

void Slider::setKnobColor(QColor ch) {
    _knobcolor = ch;
}

void Slider::setIndex1Set( bool input ) {
    _index1set = input;
}

void Slider::setIndex2Set( bool input ) {
    _index2set = input;
}

void Slider::setIndex3Set( bool input ) {
    _index3set = input;
}

void Slider::setIndex4Set( bool input ) {
    _index4set = input;
}

void Slider::setIndex5Set( bool input ) {
    _index5set = input;
}

void Slider::setIndex6Set( bool input ) {
    _index6set = input;
}

void Slider::setRef(QString txt) {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    _refname = txt;
    update();
}

void Slider::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    if(isEditMode()) {
        PanelItem::mousePressEvent(event);
    } else {
        dragStartPoint = event->pos();
        _valueStartPoint = _value;
    }
}

void Slider::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseReleaseEvent(event);
    } else {
        update();
    }
}
void Slider::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseMoveEvent(event);
    } else {
        double changeAmount = (dragStartPoint - event->pos()).y();
        float travel = height() - (height() * 0.20f);
        changeAmount = changeAmount * ((_maximum - _minimum) / travel);
        _value = _valueStartPoint + changeAmount;
        if (_value < _minimum) {
            _value = _minimum;
        }
        if (_value > _maximum) {
            _value = _maximum;
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
        }
        update();
    }
}

void Slider::valueChanged(QString ref, double newValue) {
    Q_ASSERT(ref==_refname);
    _value = newValue;
}
