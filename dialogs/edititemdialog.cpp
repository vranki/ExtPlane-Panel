#include "edititemdialog.h"
#include "ui_edititemdialog.h"
#include "../panelitems/panelitem.h"

EditItemDialog::EditItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditItemDialog)
{
    ui->setupUi(this);
    currentItem = 0;
    connect(this, SIGNAL(finished(int)), this, SLOT(applySettings()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteItemClicked()));
}

EditItemDialog::~EditItemDialog()
{
    delete ui;
}

void EditItemDialog::applySettings() {
    if(currentItem)
        currentItem->applySettings();
    deleteLater();
}

void EditItemDialog::deleteItemClicked() {
    currentItem->deleteLater();
    currentItem = 0;
    // Delete the item, and this dialog should be deleted..
}

void EditItemDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void EditItemDialog::setPanelItem(PanelItem *it) {
    if(currentItem && currentItem != sender()) {
        applySettings();
        disconnect(currentItem, 0, this, 0);
    }
    currentItem = it;
    if(!currentItem) {
        hide();
        return;
    }
    this->setWindowTitle(it->typeName());
    connect(currentItem, SIGNAL(destroyed()), this, SLOT(setPanelItem()));
    ui->zSlider->setValue(it->zValue());
    ui->rotationDial->setValue(it->itemRotation());
    ui->itemFontsizeSpinBox->setValue(it->itemFontSize());
    connect(ui->zSlider, SIGNAL(valueChanged(int)), it, SLOT(setZValue(int)));
    connect(ui->rotationDial, SIGNAL(valueChanged(int)), it, SLOT(setItemRotation(int)));
    connect(ui->itemFontsizeSpinBox, SIGNAL(valueChanged(double)), it, SLOT(setItemFontSize(double)));
    it->createSettings(ui->itemPropertiesGrid);
}
