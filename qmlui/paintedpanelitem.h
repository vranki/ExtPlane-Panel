#ifndef PAINTEDPANELITEM_H
#define PAINTEDPANELITEM_H

#include <QtQuick>
#include "extplaneclient.h"
#include "../widgetui/panelitems/panelitem.h"

class PaintedPanelItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString itemName READ itemName WRITE setItemName NOTIFY itemNameChanged)
    Q_PROPERTY(ExtPlaneClient* client READ client WRITE setClient NOTIFY clientChanged)

public:
    PaintedPanelItem(QQuickItem *parent = nullptr);
    ~PaintedPanelItem();
    void paint(QPainter *painter);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    PanelItem *m_panelItem;
    QString itemName() const;
    ExtPlaneClient* client() const;

public slots:
    void setItemName(QString itemName);
    void setClient(ExtPlaneClient* client);

signals:
    void itemNameChanged(QString itemName);
    void clientChanged(ExtPlaneClient* client);

private slots:
    void updateRequest(); // Update this item

private:
    void createItemIfPossible();
    QString m_itemName;
    ExtPlaneClient* m_client;
};

#endif // PAINTEDPANELITEM_H
