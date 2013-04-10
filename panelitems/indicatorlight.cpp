#include "indicatorlight.h"

#include <QLineEdit>
#include <QLabel>
#include <QGraphicsScene>
#include <QImage>
#include <QBitmap>

#include "extplaneclient.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(IndicatorLight,"indicator/light/basic")

IndicatorLight::IndicatorLight(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeDisplay, PanelItemShapeRectangular),
        _client(this, typeName(), conn) {

    // Init
    setSize(400,60);
    _labelOn = "BRAKES";
    _labelOff = "BRAKES";
    _labelColor = Qt::red;
    _datarefName = "";
    _threshold = 0.1;
    _strengthOn = 100;
    _strengthOff = 50;
    _on = false;
    _labelGlow = true;

    // Add glow

    // Make connection
    conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(dataRefChanged(QString,QString)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(dataRefChanged(QString,double)));

}

void IndicatorLight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);

    // Draw the dataref name and value
    painter->setBrush(Qt::NoBrush);
    painter->setPen(_labelColor);
    QFont font = defaultFont;
    font.setBold(true);
    font.setPixelSize(height()*0.7);
    painter->setFont(font);
    if(_on) {
        painter->setOpacity(_strengthOn/100.0);
        painter->drawText(QRect(0,0,width(), height()), Qt::AlignCenter, _labelOn);
    } else {
        painter->setOpacity(_strengthOff/100.0);
        painter->drawText(QRect(0,0,width(), height()), Qt::AlignCenter, _labelOff);
    }

    if(_labelGlow) {
        _labelGlowItem.setVisible(_on);
    }

    PanelItem::paint(painter, option, widget);
}

void IndicatorLight::createLabel(int w, int h) {
    if(_labelGlow) {

        // Draw label glow
        QPixmap pixmap = QPixmap(w,h);
        pixmap.fill(Qt::transparent);
        QPainter painter;
        painter.begin(&pixmap); {
            setupPainter(&painter);
            painter.setBrush(_labelColor);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(0,0,w,h,20,20);
        } painter.end();

        // Setup the graphics item for glow
        _labelGlowItem.setPixmap(pixmap);
        _labelGlowItem.setX(this->x());
        _labelGlowItem.setY(this->y());
        _labelGlowItem.setZValue(PANEL_PANELITEM_GLOW_ZVALUE);
        _labelGlowEffect.setBlurRadius(40);
        _labelGlowItem.setGraphicsEffect(&_labelGlowEffect);
        if(_labelGlowItem.scene() == NULL) this->scene()->addItem(&_labelGlowItem);
    }
}

void IndicatorLight::itemSizeChanged(float w, float h) {
    createLabel(w,h);
}

void IndicatorLight::settingChanged() {
    createLabel(width(),height());
    update();
}

void IndicatorLight::setPos(int x, int y) {
    PanelItem::setPos(x,y);
    if(_labelGlow) {
        _labelGlowItem.setX(x);
        _labelGlowItem.setY(y);
    }
}

void IndicatorLight::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("datarefname", _datarefName);
}

void IndicatorLight::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setDataRefName(settings.value("datarefname","sim/cockpit/misc/compass_indicated").toString());
}

void IndicatorLight::createSettings(QGridLayout *layout) {
    PanelItem::createSettings(layout);

    createLineEditSetting(layout,"DataRef",_datarefName,SLOT(setDataRefName(QString)));
    createColorSetting(layout,"Label Color","red",SLOT(setLabelColor(QColor)));
}

void IndicatorLight::setDataRefName(QString name) {

    // Unsubscribe old
    if(_datarefName != "") _client.unsubscribeDataRef(_datarefName); //TODO: there seems to be something wrong with unsubscribing...
    _datarefName = name;
     _datarefValue = 0;

    // Subscribe new
    _client.subscribeDataRef(name, 0);
    update();
}

void IndicatorLight::dataRefChanged(QString name, QString val) {
    dataRefChanged(name,val.toDouble());
}

void IndicatorLight::dataRefChanged(QString name, double val) {
    // On or off?
    _datarefValue = val;
    bool newOn = (_datarefValue > _threshold);
    if(_on != newOn) {
        _on = newOn;
        update();
    }
}
