#include "rotaryknob.h"
#include <QLabel>
#include <QLineEdit>

RotaryKnob::RotaryKnob(QObject *parent, ExtPlaneConnection *conn) :
        PanelItem(parent), _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(valueChanged(QString,double)));
    _value = 0;
    _change = 0;
    _valueStartPoint = 0;
    _ref = 0;
}

void RotaryKnob::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::gray);
    painter->setPen(Qt::gray);
    double circleSize = height();
    painter->drawEllipse(0, 0, circleSize, circleSize);
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->save();
    painter->translate(circleSize/2,circleSize/2);
    painter->rotate(_value);
    for(int i=0;i<8;i++) {
        painter->drawEllipse(0, circleSize/3, circleSize/10, circleSize/10);
        painter->rotate(45);
    }
    painter->restore();

    PanelItem::paint(painter, option, widget);
}

QString RotaryKnob::typeName() {
    return typeNameStatic();
}

QString RotaryKnob::typeNameStatic() {
    return "adjustment/rotary";
}

void RotaryKnob::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
    settings.setValue("dataref", _refname);
}

void RotaryKnob::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    setRef(settings.value("dataref", "").toString());
    setChange(settings.value("change", 1).toDouble());
}

void RotaryKnob::createSettings(QGridLayout *layout) {
    layout->addWidget(new QLabel("Dataref", layout->parentWidget()));
    QLineEdit *refEdit = new QLineEdit(_refname, layout->parentWidget());
    connect(refEdit, SIGNAL(textChanged(QString)), this, SLOT(setRef(QString)));
    layout->addWidget(refEdit);

    layout->addWidget(new QLabel("Change amount", layout->parentWidget()));
    QLineEdit *changeEdit = new QLineEdit(QString::number(_change), layout->parentWidget());
    connect(changeEdit, SIGNAL(textChanged(QString)), this, SLOT(setChangeString(QString)));
    layout->addWidget(changeEdit);
}

void RotaryKnob::applySettings() {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    if(!_refname.isEmpty())
        _ref = _client.subscribeDataRef(_refname, 0);
}

void RotaryKnob::setChange(double ch) {
    _change = ch;
    update();
}

void RotaryKnob::setChangeString(QString ch) {
    bool ok = false;
    double chDouble = ch.toDouble(&ok);
    if(ok)
        setChange(chDouble);
}

void RotaryKnob::setRef(QString txt) {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    _refname = txt;
    update();
}

void RotaryKnob::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    if(isEditMode()) {
        PanelItem::mousePressEvent(event);
    } else {
        dragStartPoint = event->pos();
        _valueStartPoint = _value;
    }
}

void RotaryKnob::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseReleaseEvent(event);
    } else {
        update();
    }
}
void RotaryKnob::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseMoveEvent(event);
    } else {
        double changeAmount = (event->pos() - dragStartPoint).x();
        // qDebug() << Q_FUNC_INFO << event->pos() - dragStartPoint;
        _value = _valueStartPoint + changeAmount * _change;
        if(_ref)
            _ref->setValue(_value);
        update();
    }
}

void RotaryKnob::valueChanged(QString ref, double newValue) {
    Q_ASSERT(ref==_refname);
    _value = newValue;
}
