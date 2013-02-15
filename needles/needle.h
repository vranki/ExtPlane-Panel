#ifndef NEEDLE_H
#define NEEDLE_H

#include <QObject>
#include <QPainter>
#include <QColor>

/**
 * Base class for needle instruments
 *
 * When painting, needle center is at (0,0). Needle tip should be at (0,-1)
 */
class Needle : public QObject {
public:
    explicit Needle(QObject *parent);
    virtual void paint(QPainter *painter)=0;
    void setColor(QColor newColor);
protected:
    QColor needleColor, borderColor;
};

#endif // NEEDLE_H
