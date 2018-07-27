#ifndef PAINTEDPANELITEM_H
#define PAINTEDPANELITEM_H

#include <QtQuick>
#include "../widgetui/panelitems/panelitem.h"

class PaintedPanelItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    PaintedPanelItem(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    PanelItem *m_panelItem;
};

#endif // PAINTEDPANELITEM_H
