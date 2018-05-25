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
    // Remove the previously showed one
    if(selectedPanelItem) {
        scene.removeItem(selectedPanelItem);
        scene.clear();
        selectedPanelItem->deleteLater();
        selectedPanelItem = 0;
    }
    if(!newItem) return;
    // Create new preview item
    selectedPanelItem = factory.itemForName(newItem->text(), panel, &simulatedConnection);
    Q_ASSERT(selectedPanelItem);
    Q_ASSERT(ui->itemPreview->scene());
    selectedPanelItem->setAntialiasEnabled(true); // Wouldn't want the preview to look ugly would we?
    scene.addItem(selectedPanelItem);
    // Set size - first we get the size we need from the scene's fitInView, then we update the size so that the item can cache any pixmaps it needs, then we refit so that it is centered
    //TODO: for some reason this is still upscaling any pixmaps within the item...?
    ui->itemPreview->fitInView(selectedPanelItem, Qt::KeepAspectRatio);
    selectedPanelItem->setSize(selectedPanelItem->width(),selectedPanelItem->height());
}
