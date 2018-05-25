#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent, Settings *appSettings) :
    QDialog(parent), ui(new Ui::SettingsDialog), appSettings(appSettings) {
    ui->setupUi(this);
    connect(ui->panelRotationDial, SIGNAL(valueChanged(int)), this, SIGNAL(rotationChanged(int)));
    connect(ui->fullscreenCheckbox, SIGNAL(clicked(bool)), this, SIGNAL(fullscreenChanged(bool)));
    connect(ui->simulateCheckbox, SIGNAL(clicked(bool)), this, SIGNAL(simulateChanged(bool)));
    connect(ui->serverAddressEdit, SIGNAL(textChanged(QString)), this, SIGNAL(setServerAddress(QString)));
    connect(ui->updateIntervalComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIntervalChanged()));
    connect(ui->interpolateCheckbox, SIGNAL(toggled(bool)), this, SIGNAL(setInterpolationEnabled(bool)));
    connect(ui->panelUpdateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(panelUpdateIntervalChanged()));
    connect(ui->fontSizeSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(setDefaultFontSize(double)));
    connect(ui->antialiasCheckbox, SIGNAL(toggled(bool)), this, SIGNAL(setAntialiasEnabled(bool)));
    connect(ui->autoPanelsCheckbox, SIGNAL(toggled(bool)), this, SIGNAL(setAutoPanelsEnabled(bool)));
    connect(ui->adjustPowerCheckbox, SIGNAL(toggled(bool)), this, SIGNAL(setAdjustPowerEnabled(bool)));
    connect(this, SIGNAL(finished(int)), this, SLOT(saveSettings()));
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsDialog::loadSettings() {
    // Global rotation
    ui->panelRotationDial->setValue(appSettings->valueFromSettingsOrCommandLine("panelrotation").toInt());

    // Fullscreen checkbox
    #ifdef MOBILE_DEVICE // Default to fullscreen on mobile devices
        ui->fullscreenCheckbox->setChecked(appSettings->valueFromSettingsOrCommandLine("fullscreen", true).toBool());
    #else
        ui->fullscreenCheckbox->setChecked(appSettings->valueFromSettingsOrCommandLine("fullscreen", false).toBool());
    #endif
    emit fullscreenChanged(ui->fullscreenCheckbox->isChecked());

    // Simulate checkbox
    ui->simulateCheckbox->setChecked(appSettings->valueFromSettingsOrCommandLine("simulate", false).toBool());
    emit simulateChanged(ui->simulateCheckbox->isChecked());

    // Server line edit
    ui->serverAddressEdit->setText(appSettings->valueFromSettingsOrCommandLine("serveraddress", "127.0.0.1:51000").toString());
    emit setServerAddress(ui->serverAddressEdit->text());

    // Update interval combobox
    ui->updateIntervalComboBox->setCurrentIndex(appSettings->valueFromSettingsOrCommandLine("updateinterval", 0).toInt());
    updateIntervalChanged();

    // Additional checkboxes
    ui->interpolateCheckbox->setChecked(appSettings->valueFromSettingsOrCommandLine("interpolate", true).toBool());
    emit setInterpolationEnabled(ui->interpolateCheckbox->isChecked());
    ui->antialiasCheckbox->setChecked(appSettings->valueFromSettingsOrCommandLine("antialias", true).toBool());
    emit setAntialiasEnabled(ui->antialiasCheckbox->isChecked());
    ui->autoPanelsCheckbox->setChecked(appSettings->valueFromSettingsOrCommandLine("autopanels", false).toBool());
    emit setAutoPanelsEnabled(ui->autoPanelsCheckbox->isChecked());
    ui->adjustPowerCheckbox->setChecked(appSettings->valueFromSettingsOrCommandLine("adjustpower", false).toBool());
    emit setAdjustPowerEnabled(ui->adjustPowerCheckbox->isChecked());
    ui->rememberSizeAndPositionCheckBox->setChecked(appSettings->valueFromSettingsOrCommandLine("remembersizeandposition", false).toBool());

    // Panel update interval
    ui->panelUpdateComboBox->setCurrentIndex(appSettings->valueFromSettingsOrCommandLine("panelupdateinterval", 4).toInt());

    // Font size
    ui->fontSizeSpinBox->setValue(appSettings->valueFromSettingsOrCommandLine("fontsize", 10).toDouble());
    emit setDefaultFontSize(ui->fontSizeSpinBox->value());
}

void SettingsDialog::saveSettings() {
    appSettings->setValue("panelrotation", ui->panelRotationDial->value());
    appSettings->setValue("fullscreen", ui->fullscreenCheckbox->isChecked());
    appSettings->setValue("simulate", ui->simulateCheckbox->isChecked());
    appSettings->setValue("serveraddress", ui->serverAddressEdit->text());
    appSettings->setValue("updateinterval", ui->updateIntervalComboBox->currentIndex());
    appSettings->setValue("interpolate", ui->interpolateCheckbox->isChecked());
    appSettings->setValue("antialias", ui->antialiasCheckbox->isChecked());
    appSettings->setValue("autopanels", ui->autoPanelsCheckbox->isChecked());
    appSettings->setValue("adjustpower", ui->adjustPowerCheckbox->isChecked());
    appSettings->setValue("panelupdateinterval", ui->panelUpdateComboBox->currentIndex());
    appSettings->setValue("fontsize", ui->fontSizeSpinBox->value());
    appSettings->setValue("remembersizeandposition", ui->rememberSizeAndPositionCheckBox->isChecked());
    appSettings->sync();
}

void SettingsDialog::updateIntervalChanged() {
    bool ok;
    float newInterval = ui->updateIntervalComboBox->currentText().toFloat(&ok);
    if(!ok) {
        newInterval = 0;
    } else {
        newInterval = 1.f / newInterval; // Convert Hz to delay in s
    }

    emit setUpdateInterval(newInterval);
}

void SettingsDialog::panelUpdateIntervalChanged() {
    bool ok;
    float newInterval = ui->panelUpdateComboBox->currentText().toFloat(&ok);
    if(!ok) {
        newInterval = 0;
    } else {
        newInterval = 1.f / newInterval; // Convert Hz to delay in s
    }
    emit setPanelUpdateInterval(newInterval);
}
