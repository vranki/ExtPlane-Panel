#include "hardwaredialog.h"
#include "ui_hardwaredialog.h"
#include "../hardware/hardwaremanager.h"
#include "../hardware/hardwarebinding.h"

HardwareDialog::HardwareDialog(QWidget *parent, HardwareManager *manager) :
    QDialog(parent),
    ui(new Ui::HardwareDialog), hwManager(manager), currentBinding(0)
{
    ui->setupUi(this);
    connect(ui->newBindingButton, SIGNAL(clicked()), this, SLOT(newBinding()));
    connect(ui->deleteBindingButton, SIGNAL(clicked()), this, SLOT(deleteBinding()));
    connect(ui->saveChangesButton, SIGNAL(clicked()), this, SLOT(saveChanges()));
    connect(ui->bindingListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));
    updateUi();
}

HardwareDialog::~HardwareDialog()
{
    delete ui;
}

void HardwareDialog::changeEvent(QEvent *e)
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

void HardwareDialog::newBinding() {
    currentBinding = new HardwareBinding(hwManager, hwManager->connection());
    int num = ui->bindingListWidget->count();
    currentBinding->setName(QString("Binding %0").arg(num));
    hwManager->addBinding(currentBinding);
    updateUi();
}

void HardwareDialog::deleteBinding()
{
    int num = ui->bindingListWidget->row(ui->bindingListWidget->selectedItems().first());
    if(num >= 0) {
        HardwareBinding *binding = bindingRows.value(num);
        if(currentBinding == binding)
            currentBinding = 0;
        hwManager->deleteBinding(binding);
    }
    updateUi();
}

void HardwareDialog::saveChanges() {
    if(!currentBinding) return;
    currentBinding->setName(ui->bindingNameLineEdit->text());
    currentBinding->setRefName(ui->bindingRefNameEdit->text());
    currentBinding->setAccuracy(ui->refAccuracySpinBox->value());
    currentBinding->setInputValues(ui->inputMinSpinbox->value(), ui->inputMaxSpinbox->value());
    currentBinding->setOutputValues(ui->outputMinSpinbox->value(), ui->outputMaxSpinbox->value());
    updateUi();
    currentBinding->activate();
}

void HardwareDialog::currentRowChanged(int row) {
    if(row == -1) return;
    currentBinding = bindingRows.value(row);
    updateUi();
}

void HardwareDialog::updateUi() {
    if(currentBinding) {
        ui->bindingNameLineEdit->setText(currentBinding->name());
        ui->bindingRefNameEdit->setText(currentBinding->refName());
        ui->refAccuracySpinBox->setValue(currentBinding->accuracy());
        ui->inputMinSpinbox->setValue(currentBinding->inputMin());
        ui->inputMaxSpinbox->setValue(currentBinding->inputMax());
        ui->outputMinSpinbox->setValue(currentBinding->outputMin());
        ui->outputMaxSpinbox->setValue(currentBinding->outputMax());
    } else {
        ui->bindingNameLineEdit->setText("");
    }
    ui->bindingPropertiesGroupBox->setEnabled(currentBinding);

    ui->bindingListWidget->clear();
    bindingRows.clear();

    foreach(HardwareBinding *binding, hwManager->bindings()) {
        bindingRows.insert(ui->bindingListWidget->count(), binding);
        ui->bindingListWidget->addItem(binding->name());
    }
}
