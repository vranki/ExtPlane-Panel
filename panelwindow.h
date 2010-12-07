#ifndef PANELWINDOW_H
#define PANELWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QObject>
#include <QMessageBox>
#include <extplaneconnection.h>
#include "panelitemfactory.h"
#include "menubutton.h"

class GaugeWindow : public QGraphicsView {
    Q_OBJECT

public:
    explicit GaugeWindow();
    ~GaugeWindow();
signals:

public slots:
    void connectionError(QString txt);
    void itemDestroyed(QObject *obj);
    void addItem(PanelItem *g);
private:
    MenuButton *menuButton;
    QGraphicsScene scene;
    ExtPlaneConnection connection;
    QGraphicsTextItem errorMessage;
    QList<PanelItem *> panelItems;
    PanelItemFactory itemFactory;
};

#endif // PANELWINDOW_H
