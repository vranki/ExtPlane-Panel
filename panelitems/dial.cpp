#include "dial.h"

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGraphicsSceneMouseEvent>
#include <qmath.h>

#include "../widgets/numberinputlineedit.h"
#include "extplaneclient.h"
#include "../util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(Dial,"switches/dial");

Dial::Dial(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeSwitch, PanelItemShapeRectangular),
        _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(valueChanged(QString,double)));
    _value = false;
    _label = "Dial";
    _ref = 0;
    _dialPositions = 3;
    _dialPosition = 1;
    setSize(100,30);
}

void Dial::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);

    static const QPoint plane[] = {
        QPoint(0, 70),
        QPoint(5, 60),
        QPoint(5, 30),
        QPoint(60,-10),
        QPoint(60,-20),
        QPoint(5,-10),
        QPoint(5,-30),
        QPoint(4,-40),
        QPoint(25,-55),
        QPoint(25,-65),
        QPoint(2,-60),
        QPoint(0,-65),
        QPoint(-2,-60),
        QPoint(-25,-65),
        QPoint(-25,-55),
        QPoint(-4,-40),
        QPoint(-5,-30),
        QPoint(-5,-10),
        QPoint(-60,-20),
        QPoint(-60,-10),
        QPoint(-5, 30),
        QPoint(-5, 60),
    };


    double side = qMin(this->width(),this->height());
    QFont font = defaultFont;
    font.setPixelSize(side*0.08);
    painter->setFont(font);

    painter->save(); {

        // Init
        painter->translate(this->width()/2.0,this->height()/2.0);
        double dialR = side*0.25;
        //double totalPositionDegrees = 120;
        double degreesPerPosition = 30.0;
        double firstPositionDegrees = -(_dialPositions-1)*degreesPerPosition/2.0;
        double lineWidth = dialR*0.05;

        // Dial positions
        painter->save(); {
            painter->rotate(firstPositionDegrees);
            painter->setBrush(Qt::white);
            painter->setPen(QPen(Qt::white,lineWidth));
            for(int i = 1; i <= _dialPositions; i++) {
                painter->drawLine(QPointF(0,-dialR*1.1),QPointF(0,-dialR*1.4));
                painter->rotate(degreesPerPosition);
            }
        } painter->restore();

        // Dial position labels
        painter->setBrush(Qt::NoBrush);
        painter->setPen(Qt::white);
        double labelPositionDegrees = firstPositionDegrees;
        for(int i = 1; i <= _dialPositions; i++) {
            double labelW = dialR*1.5;
            double labelH = dialR*0.6;
            double labelX = 0;
            double labelY = -dialR*2.0;
            double theta = (labelPositionDegrees) * M_PI / 180.0;
            double cs = qCos(theta);
            double sn = qSin(theta);
            double rpx = labelX * cs - labelY * sn;
            double rpy = labelX * sn + labelY * cs;
            painter->drawText(QRect(rpx-labelW/2.0,rpy,labelW,labelH), Qt::AlignCenter, "OFF");
            //painter->drawRect(QRect(rpx-labelW/2.0,rpy,labelW,labelH));
            labelPositionDegrees += degreesPerPosition;
        }

        // Dial label
        painter->drawText(QRect(-width()/2.0,dialR*0.9,width(),dialR*1.0), Qt::AlignCenter, _label);

        // Draw dial body
        painter->rotate(firstPositionDegrees + ((_dialPosition-1)*degreesPerPosition));
        painter->setBrush(Qt::gray);
        painter->setPen(Qt::gray);
        painter->drawEllipse(QPointF(0,0),dialR,dialR);

        // Dial pointer
        QPointF dialPointer[] = {
            QPointF(-0.2*dialR, -dialR*0.9),
            QPointF(+0.0, -dialR*1.2),
            QPointF(+0.2*dialR, -dialR*0.9)
        };
        painter->drawConvexPolygon(dialPointer, sizeof(dialPointer)/sizeof(dialPointer[0]));

        // Dial line
        painter->setBrush(Qt::white);
        painter->setPen(QPen(Qt::white,lineWidth));
        painter->drawLine(QPointF(0,0),QPointF(0,-dialR*0.8));


    } painter->restore();

    PanelItem::paint(painter, option, widget);
    return;
/*
    double circleSize = height()/3;
    painter->drawEllipse(0, height()/2-circleSize/2, circleSize, circleSize);

    painter->save();
    double switchWidth = height()/3;
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
    PanelItem::paint(painter, option, widget);*/
}

void Dial::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("label", _label);
    settings.setValue("dataref", _refname);
}

void Dial::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setLabel(settings.value("label", "Dial").toString());
    setRef(settings.value("dataref", "").toString());
}

void Dial::createSettings(QGridLayout *layout) {
    layout->addWidget(new QLabel("Label", layout->parentWidget()));
    QLineEdit *labelEdit = new QLineEdit(_label, layout->parentWidget());
    connect(labelEdit, SIGNAL(textChanged(QString)), this, SLOT(setLabel(QString)));
    layout->addWidget(labelEdit);

    layout->addWidget(new QLabel("Dataref", layout->parentWidget()));
    QLineEdit *refEdit = new QLineEdit(_refname, layout->parentWidget());
    connect(refEdit, SIGNAL(textChanged(QString)), this, SLOT(setRef(QString)));
    layout->addWidget(refEdit);
}

void Dial::applySettings() {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    if(!_refname.isEmpty())
        _ref = _client.subscribeDataRef(_refname, 0);
}

void Dial::setLabel(QString txt) {
    _label = txt;
    update();
}

void Dial::setRef(QString txt) {
    if(_ref) {
        _ref->unsubscribe();
        _ref = 0;
    }
    _refname = txt;
    update();
}

void Dial::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mousePressEvent(event);
    }
}

void Dial::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseReleaseEvent(event);
    } else {
        //_value = !_value;
        //if(_ref) _ref->setValue(_value ? 1 : 0);
        _dialPosition++;
        if(_dialPosition > _dialPositions) _dialPosition = 1;
        update();
    }
}

void Dial::valueChanged(QString ref, double newValue) {
    Q_ASSERT(ref==_refname);
    _value = newValue != 0;
}
