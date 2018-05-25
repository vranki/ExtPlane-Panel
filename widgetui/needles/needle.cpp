#include "needle.h"

Needle::Needle(QObject *parent) : QObject(parent), needleColor(Qt::white) {
}

void Needle::setColor(QColor newColor) {
    needleColor = newColor;
    borderColor = needleColor.darker(150);
}
