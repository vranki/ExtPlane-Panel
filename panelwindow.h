#ifndef PANELWINDOW_H
#define PANELWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsItemGroup>
#include <QObject>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <extplaneconnection.h>
#include "panelitemfactory.h"
#include "menubutton.h"

class PanelWindow : public QGraphicsView {
    Q_OBJECT

public:
    explicit PanelWindow();
    ~PanelWindow();
signals:

public slots:
    void connectionError(QString txt);
    void itemDestroyed(QObject *obj);
    void addItem(PanelItem *g);
    void panelRotationChanged(int r);
    void fullscreenChanged(bool fs);
    void setServerAddress(QString host);
protected:
    virtual void resizeEvent(QResizeEvent * event);
private:
    MenuButton *menuButton;
    QGraphicsScene scene;
    ExtPlaneConnection connection;
    QGraphicsTextItem errorMessage;
    QList<PanelItem *> panelItems;
    PanelItemFactory itemFactory;
    float panelRotation;
};

#endif // PANELWINDOW_H
