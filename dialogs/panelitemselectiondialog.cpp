#include "panelitemselectiondialog.h"
#include "ui_panelitemselectiondialog.h"
#include "../panelitems/panelitem.h"

PanelItemSelectionDialog::PanelItemSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PanelItemSelectionDialog), selectedPanelItem(0)
{
    // Init
    panel = new ExtPlanePanel(NULL,this);

    // UI init
    ui->setupUi(this);
    ui->itemPreview->setScene(&scene);
    ui->itemPreview->setBackgroundBrush(Qt::black);
    ui->itemList->addItems(factory.itemNames());

    // Connections
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
    emit addItem(ui->itemList->currentItem()->text());
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
    selectedPanelItem = factory.itemForName(newItem->text(), panel, &simulatedConnection);
    Q_ASSERT(selectedPanelItem);
    Q_ASSERT(ui->itemPreview->scene());
    scene.addItem(selectedPanelItem);
    ui->itemPreview->fitInView(selectedPanelItem, Qt::KeepAspectRatio);
}
