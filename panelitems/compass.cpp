#include "compass.h"

Compass::Compass(QObject *parent, ExtPlaneConnection *conn) :
        PanelItem(parent), _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit/misc/compass_indicated", 1);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(headingChanged(QString,double)));
    _heading = 0;
}

void Compass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::white);
    painter->drawRect(0,0,width(),height());
    QFont font = painter->font();
    font.setPixelSize(height()*0.75);
    painter->setFont(font);
    QString hdgString = QString::number(_heading);
    while(hdgString.length() < 3)
        hdgString = "0" + hdgString;
    painter->drawText(QRect(0,0,width(), height()), Qt::AlignCenter, hdgString);
    PanelItem::paint(painter, option, widget);
}

void Compass::headingChanged(QString name, double hdg) {
    _heading = hdg;
    update();
}

QString Compass::typeName() {
    return typeNameStatic();
}

QString Compass::typeNameStatic() {
    return "indicator/compass/basic";
}
