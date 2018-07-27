#include "paintedpanelitem.h"
#include <QPainter>
#include <QBrush>
#include "../widgetui/panelitems/button.h"

PaintedPanelItem::PaintedPanelItem(QQuickItem *parent) : QQuickPaintedItem(parent) {
    m_panelItem = new Button(nullptr, new ExtPlaneClient(true));
    m_panelItem->setSize(width(), height());
}

void PaintedPanelItem::paint(QPainter *painter)
{
    if(m_panelItem) {
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
        m_panelItem->paint(painter, nullptr, nullptr);
    }
}

void PaintedPanelItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    m_panelItem->setSize(newGeometry.width(), newGeometry.height());
}
