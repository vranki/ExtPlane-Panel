#include "dial.h"

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
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
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(valueChanged(QString,QString)));
    _value = "";
    _label = "AUTOPILOT";
    _positionLabel1 = "OFF";
    _positionValue1 = "0";
    _positionLabel2 = "FD";
    _positionValue2 = "1";
    _positionLabel3 = "ON";
    _positionValue3 = "2";
    _positionLabel4 = "";
    _positionValue4 = "";
    _positionLabel5 = "";
    _positionValue5 = "";
    _dialPositions = 3;
    _dialPosition = 1;
    _dataref = NULL;
    setSize(200,200);
    updatePositions();
}

void Dial::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);

    // @todo Unused variable - remove?
    /*
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
    */

    double side = qMin(this->width(), this->height());
    QFont font = defaultFont;
    font.setPixelSize(side*0.08);
    painter->setFont(font);

    painter->save(); {

        // Init
        painter->translate(this->width()/2.0, this->height()/2.0);
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
            QString label = "";
            if(i == 1) label = _positionLabel1;
            if(i == 2) label = _positionLabel2;
            if(i == 3) label = _positionLabel3;
            if(i == 4) label = _positionLabel4;
            if(i == 5) label = _positionLabel5;
            painter->drawText(QRect(rpx-labelW/2.0,rpy,labelW,labelH), Qt::AlignCenter, label);
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
}

void Dial::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("label", _label);
    settings.setValue("dataref", _datarefName);

    settings.setValue("positionLabel1", _positionLabel1);
    settings.setValue("positionValue1", _positionValue1);
    settings.setValue("positionLabel2", _positionLabel2);
    settings.setValue("positionValue2", _positionValue2);
    settings.setValue("positionLabel3", _positionLabel3);
    settings.setValue("positionValue3", _positionValue3);
    settings.setValue("positionLabel4", _positionLabel4);
    settings.setValue("positionValue4", _positionValue4);
    settings.setValue("positionLabel5", _positionLabel5);
    settings.setValue("positionValue5", _positionValue5);
}

void Dial::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setLabel(settings.value("label", "AUTOPILOT").toString());
    setDataRef(settings.value("dataref", "sim/cockpit/autopilot/autopilot_mode").toString());

    setPositionLabel1(settings.value("positionLabel1", "OFF").toString());
    setPositionValue1(settings.value("positionValue1", "0").toString());
    setPositionLabel2(settings.value("positionLabel2", "FD").toString());
    setPositionValue2(settings.value("positionValue2", "1").toString());
    setPositionLabel3(settings.value("positionLabel3", "ON").toString());
    setPositionValue3(settings.value("positionValue3", "2").toString());
    setPositionLabel4(settings.value("positionLabel4", "").toString());
    setPositionValue4(settings.value("positionValue4", "").toString());
    setPositionLabel5(settings.value("positionLabel5", "").toString());
    setPositionValue5(settings.value("positionValue5", "").toString());
}

void Dial::createSettings(QGridLayout *layout) {
    PanelItem::createSettings(layout);

    // Presets
    layout->addWidget(new QLabel("Load Preset", layout->parentWidget()));
    QComboBox *combobox = new QComboBox(layout->parentWidget());
    combobox->addItem("");
    combobox->addItem("Autopilot");
    //TODO: @noel add presets here, see indicatorlight.cpp for reference
    layout->addWidget(combobox);
    connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPreset(int)));
    connect(combobox, SIGNAL(currentIndexChanged(int)), layout->parentWidget()->window(), SLOT(close())); // This is kindof a hack, but we need to close the window to reflect the changes to the gui


    createLineEditSetting(layout, "Dial Label", _label, SLOT(setLabel(QString)));
    createLineEditSetting(layout, "Data Ref", _datarefName, SLOT(setDataRef(QString)));

    createLineEditSetting(layout, "Label #1", _positionLabel1, SLOT(setPositionLabel1(QString)));
    createLineEditSetting(layout, "Value #1", _positionValue1, SLOT(setPositionValue1(QString)));
    createLineEditSetting(layout, "Label #2", _positionLabel2, SLOT(setPositionLabel2(QString)));
    createLineEditSetting(layout, "Value #2", _positionValue2, SLOT(setPositionValue2(QString)));
    createLineEditSetting(layout, "Label #3", _positionLabel3, SLOT(setPositionLabel3(QString)));
    createLineEditSetting(layout, "Value #3", _positionValue3, SLOT(setPositionValue3(QString)));
    createLineEditSetting(layout, "Label #4", _positionLabel4, SLOT(setPositionLabel4(QString)));
    createLineEditSetting(layout, "Value #4", _positionValue4, SLOT(setPositionValue4(QString)));
    createLineEditSetting(layout, "Label #5", _positionLabel5, SLOT(setPositionLabel5(QString)));
    createLineEditSetting(layout, "Value #5", _positionValue5, SLOT(setPositionValue5(QString)));
}

void Dial::applySettings() {

}

void Dial::loadPreset(int val) {

    // Reset
    _label = "";
    _positionLabel1 = "";
    _positionValue1 = "";
    _positionLabel2 = "";
    _positionValue2 = "";
    _positionLabel3 = "";
    _positionValue3 = "";
    _positionLabel4 = "";
    _positionValue4 = "";
    _positionLabel5 = "";
    _positionValue5 = "";

    // Apply preset
    if(val == 1) {
        _label = "AUTOPILOT";
        _positionLabel1 = "OFF";
        _positionValue1 = "0";
        _positionLabel2 = "FD";
        _positionValue2 = "1";
        _positionLabel3 = "ON";
        _positionValue3 = "2";
        setDataRef("sim/cockpit/autopilot/autopilot_mode");
    }
    //TODO: @noe add presets here, see indicatorlight.cpp for referencel

    // Apply changes
    updatePositions();
}

void Dial::setLabel(QString val) {
    _label = val;
    update();
}

void Dial::setDataRef(QString val) {
    if(val.isEmpty()) return;
    if(_client.isDataRefSubscribed(_datarefName)) _client.unsubscribeDataRef(_datarefName);
    _datarefName = val;
    _dataref = _client.subscribeDataRef(_datarefName, 0);
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
        _dialPosition++;
        if(_dialPosition > _dialPositions) _dialPosition = 1;
        if(_dataref) {
            //TODO: if there is a problem with the connection this will crash
            if(_dialPosition == 1) _dataref->setValue(_positionValue1);
            if(_dialPosition == 2) _dataref->setValue(_positionValue2);
            if(_dialPosition == 3) _dataref->setValue(_positionValue3);
            if(_dialPosition == 4) _dataref->setValue(_positionValue4);
            if(_dialPosition == 5) _dataref->setValue(_positionValue5);
        }
        update();
    }
}

void Dial::valueChanged(QString ref, double newValue) {
    valueChanged(ref,QString("%1").arg(newValue));
}

void Dial::valueChanged(QString ref, QString newValue) {
    Q_UNUSED(ref);
    _value = newValue;
    DEBUG << "value is changing to" << newValue << "with dialpos" << _dialPosition;
    if(_value == _positionValue1) _dialPosition = 1;
    if(_value == _positionValue2) _dialPosition = 2;
    if(_value == _positionValue3) _dialPosition = 3;
    if(_value == _positionValue4) _dialPosition = 4;
    if(_value == _positionValue5) _dialPosition = 5;
    DEBUG << "value has changed to" << newValue << "with new dialpos" << _dialPosition;
    update();
}

void Dial::updatePositions() {
    int numPositions = 0;
    for(int i = 1; i <=5; i++) {
        if(i == 1 && _positionValue1.isEmpty()) break;
        if(i == 2 && _positionValue2.isEmpty()) break;
        if(i == 3 && _positionValue3.isEmpty()) break;
        if(i == 4 && _positionValue4.isEmpty()) break;
        if(i == 5 && _positionValue5.isEmpty()) break;
        numPositions++;
    }
    DEBUG << "new dial positions:" << numPositions;
    _dialPositions = numPositions;
    update();
}
