#ifndef TRIANGLENEEDLE_H
#define TRIANGLENEEDLE_H

#include "needle.h"

/**
 * Simple triangle needle
 */
class TriangleNeedle : public Needle
{
public:
    TriangleNeedle(QObject *parent);
    virtual void paint(QPainter *painter);
private:
    QPolygonF polygon;
};

#endif // TRIANGLENEEDLE_H
