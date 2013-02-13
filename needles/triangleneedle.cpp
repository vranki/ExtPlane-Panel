#include "triangleneedle.h"
#include <QPolygon>
#include <QPen>

TriangleNeedle::TriangleNeedle(QObject *parent) : Needle(parent) {
    polygon << QPointF(0,-1) << QPointF(0.05,0)  << QPointF(0,0.05)  << QPointF(-0.05,0);
}

void TriangleNeedle::paint(QPainter *painter) {
    painter->save();
    QPen pen(borderColor, 0);
    painter->setPen(pen);
    painter->setBrush(needleColor);
    painter->drawPolygon(polygon);
    painter->restore();
}
