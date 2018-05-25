#include "gabalancedneedle.h"

#include <QPolygon>
#include <QPen>

GABalancedNeedle::GABalancedNeedle(QObject *parent) : Needle(parent) {
    setNeedleLength(1);
}

void GABalancedNeedle::paint(QPainter *painter) {
    painter->save();
    QPen pen(borderColor, 0);
    painter->setPen(pen);
    painter->setBrush(needleColor);
    painter->drawPolygon(whiteArm);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(30,30,30));
    painter->drawPolygon(blackArm);
    painter->drawEllipse(QPointF(0,0), BALANCE_SIZE, BALANCE_SIZE);
    painter->drawEllipse(QPointF(0,BLACK_ARM_LENGTH), BALANCE_SIZE, BALANCE_SIZE);
    painter->restore();
}

void GABalancedNeedle::setNeedleLength(double newLength) {
    Q_ASSERT(newLength > 0 && newLength <= 1);
    length = newLength;
    whiteArm.clear();
    blackArm.clear();
    whiteArm << QPointF(0,-1*length) << QPointF(NEEDLE_W,-0.70*length)  << QPointF(NEEDLE_W, -WHITE_ARM_START)  << QPointF(-NEEDLE_W, -WHITE_ARM_START)  << QPointF(-NEEDLE_W,-0.70*length);
    blackArm << QPointF(NEEDLE_W, -WHITE_ARM_START)  << QPointF(-NEEDLE_W,-WHITE_ARM_START) << QPointF(-NEEDLE_W, BLACK_ARM_LENGTH)  << QPointF(NEEDLE_W, BLACK_ARM_LENGTH);
}
