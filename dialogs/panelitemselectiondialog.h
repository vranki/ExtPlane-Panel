#ifndef PANELITEMSELECTIONDIALOG_H
#define PANELITEMSELECTIONDIALOG_H

#include <QObject>
#include <QDialog>
#include <QListWidgetItem>
#include <QGraphicsScene>
#include "../panelitemfactory.h"
#include "simulatedextplaneconnection.h"
#include "extplaneconnection.h"
#include "../panel.h"

namespace Ui {
class PanelItemSelectionDialog;
}

class Panel;
class PanelItem;

class PanelItemSelectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PanelItemSelectionDialog(QWidget *parent);
    ~PanelItemSelectionDialog();
public slots:
    void tickTime(double dt, int total);
signals:
    void addItem(QString itemName);
private slots:
    void itemChanged(QListWidgetItem* newItem);
    void itemAccepted();
private:
    Ui::PanelItemSelectionDialog *ui;
    PanelItemFactory factory;
    PanelItem *selectedPanelItem;
    SimulatedExtPlaneConnection simulatedConnection; // Preview connection (simulated)
    QGraphicsScene scene; // Preview scence
    ExtPlanePanel *panel; // Preview panel
};

#endif // PANELITEMSELECTIONDIALOG_H
