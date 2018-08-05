#include "paintedpanelitem.h"
#include <QPainter>
#include <QBrush>
#include "../widgetui/panelitemfactory.h"
#include "../widgetui/panelitems/attitudeindicator.h"

PaintedPanelItem::PaintedPanelItem(QQuickItem *parent) : QQuickPaintedItem(parent)
  , m_panelItem(nullptr)
{}

PaintedPanelItem::~PaintedPanelItem() {
    if(m_panelItem) delete m_panelItem;
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
    if(m_panelItem) {
        m_panelItem->setSize(newGeometry.width(), newGeometry.height());
    }
}

QString PaintedPanelItem::itemName() const
{
    return m_itemName;
}

void PaintedPanelItem::setItemName(QString itemName)
{
    if (m_itemName == itemName)
        return;

    m_itemName = itemName;
    if(m_panelItem) delete m_panelItem;

    PanelItemFactory pif;
    m_panelItem = pif.itemForName(itemName, nullptr, new ExtPlaneClient(true));
    m_panelItem->setSize(width(), height());

    emit itemNameChanged(m_itemName);
}
