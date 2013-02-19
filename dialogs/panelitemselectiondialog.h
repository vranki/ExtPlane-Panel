#ifndef PANELITEMSELECTIONDIALOG_H
#define PANELITEMSELECTIONDIALOG_H

#include <QObject>
#include <QDialog>
#include <QListWidgetItem>
#include <QGraphicsScene>
#include "../panelitemfactory.h"
#include "../simulatedextplaneconnection.h"
#include "../extplaneconnection.h"

namespace Ui {
class PanelItemSelectionDialog;
}

class PanelItem;

class PanelItemSelectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PanelItemSelectionDialog(QWidget *parent, ExtPlaneConnection *rConnection);
    ~PanelItemSelectionDialog();
public slots:
    void tickTime(double dt, int total);
signals:
    void addItem(PanelItem * item);
private slots:
    void itemChanged(QListWidgetItem* newItem);
    void itemAccepted();
private:
    Ui::PanelItemSelectionDialog *ui;
    PanelItemFactory factory;
    PanelItem *selectedPanelItem;
    SimulatedExtPlaneConnection simulatedConnection;
    ExtPlaneConnection *realConnection;
    QGraphicsScene scene;
};

#endif // PANELITEMSELECTIONDIALOG_H
