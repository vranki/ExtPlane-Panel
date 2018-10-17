#include "paintedpanelitem.h"
#include <QPainter>
#include <QBrush>
#include "extplaneconnection.h"
#include "extplaneclient.h"
#include "../widgetui/panelitemfactory.h"
#include "../widgetui/panelitems/attitudeindicator.h"

PaintedPanelItem::PaintedPanelItem(QQuickItem *parent) : QQuickPaintedItem(parent)
  , m_panelItem(nullptr)
  , m_client(nullptr)
{
    m_client = &ExtPlaneClient::instance();
}

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
    Q_UNUSED(oldGeometry);
    if(m_panelItem) {
        m_panelItem->setSize(newGeometry.width(), newGeometry.height());
    }
}

QString PaintedPanelItem::itemName() const
{
    return m_itemName;
}

ExtPlaneClient *PaintedPanelItem::client() const
{
    return m_client;
}

void PaintedPanelItem::setItemName(QString itemName)
{
    if (m_itemName == itemName)
        return;

    m_itemName = itemName;
    createItemIfPossible();
    emit itemNameChanged(m_itemName);
}

void PaintedPanelItem::setClient(ExtPlaneClient *client)
{
    Q_ASSERT(!m_client); // Can't change client on the run yet..
    if (m_client == client)
        return;

    m_client = client;
    emit clientChanged(m_client);
    createItemIfPossible();
}

void PaintedPanelItem::updateRequest() {
    update();
}

void PaintedPanelItem::createItemIfPossible()
{
    if(!m_client || m_itemName.isEmpty()) return;
    if(m_panelItem) delete m_panelItem;
    PanelItemFactory pif;
    m_panelItem = pif.itemForName(m_itemName, nullptr, m_client);
    m_panelItem->setSize(width(), height());
    connect(m_panelItem, &PanelItem::updateRequest, this, &PaintedPanelItem::updateRequest);
}
