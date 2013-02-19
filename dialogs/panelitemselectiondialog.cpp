#include "panelitemselectiondialog.h"
#include "ui_panelitemselectiondialog.h"
#include "../panelitems/panelitem.h"

PanelItemSelectionDialog::PanelItemSelectionDialog(QWidget *parent, ExtPlaneConnection *rConnection) :
    QDialog(parent),
    ui(new Ui::PanelItemSelectionDialog), selectedPanelItem(0), realConnection(rConnection)
{
    ui->setupUi(this);
    ui->itemPreview->setScene(&scene);
    ui->itemPreview->setBackgroundBrush(Qt::black);
    ui->itemList->addItems(factory.itemNames());
    connect(ui->itemList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));
    connect(this, SIGNAL(accepted()), this, SLOT(itemAccepted()));
    connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));
    // Set sensible size to splitter
    QList<int> sizes;
    sizes << 50 << 50;
    ui->splitter->setSizes(sizes);
}

PanelItemSelectionDialog::~PanelItemSelectionDialog() {
    itemChanged(0); // Make sure item is deleted
    delete ui;
}

void PanelItemSelectionDialog::itemAccepted() {
    emit addItem(factory.itemForName(ui->itemList->currentItem()->text(), parent(), realConnection)); // Set parent of this dialog as parent
}

void PanelItemSelectionDialog::tickTime(double dt, int total) {
    simulatedConnection.tickTime(dt, total);
    if(selectedPanelItem)
        selectedPanelItem->tickTime(dt, total);
}

void PanelItemSelectionDialog::itemChanged(QListWidgetItem *newItem) {
    if(selectedPanelItem) {
        scene.removeItem(selectedPanelItem);
        selectedPanelItem->deleteLater();
        selectedPanelItem = 0;
    }
    if(!newItem) return;
    selectedPanelItem = factory.itemForName(newItem->text(), this, &simulatedConnection);
    Q_ASSERT(selectedPanelItem);
    Q_ASSERT(ui->itemPreview->scene());
    scene.addItem(selectedPanelItem);
    ui->itemPreview->fitInView(selectedPanelItem, Qt::KeepAspectRatio);
}
