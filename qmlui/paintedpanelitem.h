#ifndef PAINTEDPANELITEM_H
#define PAINTEDPANELITEM_H

#include <QtQuick>
#include "../widgetui/panelitems/panelitem.h"

class PaintedPanelItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString itemName READ itemName WRITE setItemName NOTIFY itemNameChanged)

public:
    PaintedPanelItem(QQuickItem *parent = nullptr);
    ~PaintedPanelItem();
    void paint(QPainter *painter);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    PanelItem *m_panelItem;
    QString itemName() const;

public slots:
    void setItemName(QString itemName);

signals:
    void itemNameChanged(QString itemName);

private:
    QString m_itemName;
};

#endif // PAINTEDPANELITEM_H
