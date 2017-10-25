#include "hardwaredialog.h"
#include "ui_hardwaredialog.h"
#include "../hardware/hardwaremanager.h"
#include "../hardware/hardwarebinding.h"
#include "../hardware/outputdevice.h"
#include "../hardware/servoblasteroutputdevice.h"
#include "../hardware/pololuoutputdevice.h"
#include "../hardware/chromaoutputdevice.h"
#include "bindingcurvedialog.h"

HardwareDialog::HardwareDialog(QWidget *parent, HardwareManager *manager) :
    QDialog(parent),
    ui(new Ui::HardwareDialog), hwManager(manager), currentBinding(0)
{
    ui->setupUi(this);
    connect(ui->newBindingButton, SIGNAL(clicked()), this, SLOT(newBinding()));
    connect(ui->deleteBindingButton, SIGNAL(clicked()), this, SLOT(deleteBinding()));
    connect(ui->saveChangesButton, SIGNAL(clicked()), this, SLOT(saveChanges()));
    connect(ui->bindingListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));
    connect(ui->enableSB, SIGNAL(clicked(bool)), this, SLOT(enableSB(bool)));
    connect(ui->enablePololu, SIGNAL(clicked(bool)), this, SLOT(enablePololu(bool)));
    connect(ui->enableChroma, SIGNAL(clicked(bool)), this, SLOT(enableChroma(bool)));
    connect(ui->outputCurvesButton, SIGNAL(clicked()), this, SLOT(showOutputCurvesDialog()));
    connect(hwManager->devices().value(POLOLU_ID), SIGNAL(deviceEnabled(bool)), ui->enablePololu, SLOT(setChecked(bool)));
    connect(hwManager->devices().value(SERVOBLASTER_ID), SIGNAL(deviceEnabled(bool)), ui->enableSB, SLOT(setChecked(bool)));
    connect(hwManager->devices().value(CHROMA_ID), SIGNAL(deviceEnabled(bool)), ui->enableChroma, SLOT(setChecked(bool)));
    connect(manager, SIGNAL(deviceAvailable(int,bool)), this, SLOT(deviceAvailable(int,bool)));
    connect(this, SIGNAL(deviceEnabled(int,bool)), manager, SLOT(deviceEnabled(int,bool)));
    updateUi();
}

HardwareDialog::~HardwareDialog()
{
    delete ui;
}

void HardwareDialog::deviceAvailable(int dev, bool avail)
{
    Q_UNUSED(dev);
    Q_UNUSED(avail);
    updateUi();
}

void HardwareDialog::showOutputCurvesDialog()
{
    if(!currentBinding) return;
    BindingCurveDialog *bcd = new BindingCurveDialog(this);
    bcd->setBinding(currentBinding);
    bcd->exec();
    bcd->deleteLater();
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
    currentBinding->setDevice(ui->outputDeviceComboBox->currentIndex());
    currentBinding->setOutput(ui->outputNumSpinbox->value());
    currentBinding->setInterpolationSpeed(ui->interpolationSpeedSpinbox->value());
    currentBinding->setSpeed(ui->outputSpeedSpinbox->value());
    updateUi();
    currentBinding->activate();
}

void HardwareDialog::currentRowChanged(int row) {
    if(row == -1) return;
    currentBinding = bindingRows.value(row);
    updateUi();
}

void HardwareDialog::enableSB(bool enable)
{
    emit deviceEnabled(SERVOBLASTER_ID, enable);
}

void HardwareDialog::enablePololu(bool enable)
{
    emit deviceEnabled(POLOLU_ID, enable);
}

void HardwareDialog::enableChroma(bool enable)
{
    emit deviceEnabled(CHROMA_ID, enable);
}

void HardwareDialog::updateUi() {
    if(currentBinding) {
        ui->bindingNameLineEdit->setText(currentBinding->name());
        ui->bindingRefNameEdit->setText(currentBinding->refName());
        ui->refAccuracySpinBox->setValue(currentBinding->accuracy());
        ui->inputMinSpinbox->setValue(currentBinding->inputMin());
        ui->inputMaxSpinbox->setValue(currentBinding->inputMax());
        ui->outputDeviceComboBox->setCurrentIndex(currentBinding->device());
        ui->outputNumSpinbox->setValue(currentBinding->output());
        ui->interpolationSpeedSpinbox->setValue(currentBinding->interpolationSpeed());
        ui->outputSpeedSpinbox->setValue(currentBinding->speed());
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
    foreach(OutputDevice *device, hwManager->devices().values()) {
        if(device->id()==SERVOBLASTER_ID) {
            ui->enableSB->setChecked(device->isEnabled());
            ui->enableSB->setEnabled(device->isAvailable());
            ui->sbWorkingLabel->setText(device->statusString());
        }
        if(device->id()==POLOLU_ID) {
            ui->enablePololu->setChecked(device->isEnabled());
            ui->enablePololu->setEnabled(device->isAvailable());
            ui->pololuWorkingLabel->setText(device->statusString());
        }
        if(device->id()==CHROMA_ID) {
            ui->enableChroma->setChecked(device->isEnabled());
            ui->enableChroma->setEnabled(device->isAvailable());
            ui->chromaWorkingLabel->setText(device->statusString());
        }
    }
}
