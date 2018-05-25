#ifndef GABALANCEDNEEDLE_H
#define GABALANCEDNEEDLE_H

#include "needle.h"

#define NEEDLE_W 0.03
#define WHITE_ARM_START 0.20
#define BLACK_ARM_LENGTH 0.3
#define BALANCE_SIZE 0.07

/**
 * A general aviation mechanical needle with round
 * counterbalance weight, like this one:
 *
 * http://www.transair.co.uk/products/images/large/winter_ASI_mph.jpg
 */
class GABalancedNeedle : public Needle
{
public:
    GABalancedNeedle(QObject *parent);
    virtual void paint(QPainter *painter);
    void setNeedleLength(double newLength); // Length of the white part (0-1)
private:
    QPolygonF whiteArm, blackArm;
    double length;
};

#endif // GABALANCEDNEEDLE_H
