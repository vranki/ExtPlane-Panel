#include "compass.h"
#include "extplaneclient.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(Compass,"indicator/compass/basic");

Compass::Compass(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeGauge, PanelItemShapeRectangular),
        _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit2/gauges/indicators/heading_electric_deg_mag_pilot", 1); // was sim/cockpit/misc/compass_indicated
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(headingChanged(QString,double)));
    _heading = 0;
    setSize(50,30);

}

void Compass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::white);
    painter->drawRect(0,0,width(),height());
    QFont font = defaultFont;
    font.setPixelSize(height()*0.75);
    painter->setFont(font);
    QString hdgString = QString::number(_heading);
    while(hdgString.length() < 3)
        hdgString = "0" + hdgString;
    painter->drawText(QRect(0,0,width(), height()), Qt::AlignCenter, hdgString);
 
    PanelItem::paint(painter, option, widget);
}

void Compass::headingChanged(QString name, double hdg) {
    Q_UNUSED(name);
    _heading = hdg;
    update();
}
