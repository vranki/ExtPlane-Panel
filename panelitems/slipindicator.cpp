#include "slipindicator.h"
#include "extplaneclient.h"
#include "QLinearGradient"

REGISTER_WITH_PANEL_ITEM_FACTORY(SlipIndicator,"indicator/slip/basic");

SlipIndicator::SlipIndicator(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
        _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit2/gauges/indicators/slip_deg", 0.1);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(slipChanged(QString,double)));
    _slip = 0;
    setSize(100,50);
}

void SlipIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);
    painter->setPen(Qt::NoPen);

    int tubeHeight = width()*0.25f;

    QRect tubeRect(0,0,width(),tubeHeight);

    QLinearGradient gradient(QPoint(0, 0), QPoint(0, tubeHeight));
    gradient.setColorAt(0, Qt::gray);
    gradient.setColorAt(0.3, Qt::white);
    gradient.setColorAt(1, Qt::gray);
    painter->fillRect(tubeRect, gradient);

    // Ball
    painter->setBrush(Qt::black);

    int ballWidth = tubeHeight/4;
    float slipAmount = -(_slip / SLIP_RANGE_DEG) * width()/2; // Offset in pixels

    slipAmount = qMin(slipAmount, width()/2);
    slipAmount = qMax(slipAmount, -width()/2);
    painter->drawEllipse(QPoint(width()/2 + slipAmount, tubeHeight/2), ballWidth, ballWidth*2);

    // Vertical lines
    painter->setPen(Qt::white);
    painter->drawLine(width()/2 - ballWidth, 0, width()/2 - ballWidth, tubeHeight);
    painter->drawLine(width()/2 + ballWidth, 0, width()/2 + ballWidth, tubeHeight);
    PanelItem::paint(painter, option, widget);
}

void SlipIndicator::slipChanged(QString name, double slip) {
    Q_UNUSED(name);
    _slip = slip;
    update();
}
