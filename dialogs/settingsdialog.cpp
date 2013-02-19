#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent, QSettings *appSettings) :
    QDialog(parent), ui(new Ui::SettingsDialog), appSettings(appSettings) {
    ui->setupUi(this);
    connect(ui->panelRotationDial, SIGNAL(valueChanged(int)), this, SIGNAL(rotationChanged(int)));
    connect(ui->fullscreenCheckbox, SIGNAL(clicked(bool)), this, SIGNAL(fullscreenChanged(bool)));
    connect(ui->simulateCheckbox, SIGNAL(clicked(bool)), this, SIGNAL(simulateChanged(bool)));
    connect(ui->serverAddressEdit, SIGNAL(textChanged(QString)), this, SIGNAL(setServerAddress(QString)));
    connect(ui->updateIntervalSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(setUpdateInterval(double)));
    connect(ui->interpolateCheckbox, SIGNAL(toggled(bool)), this, SIGNAL(setInterpolationEnabled(bool)));
    connect(ui->panelUpdateSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(setPanelUpdateInterval(double)));
    connect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(setDefaultFontSize(int)));
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
    appSettings->beginGroup("settings");
    ui->panelRotationDial->setValue(appSettings->value("panelrotation").toInt());
#ifdef MOBILE_DEVICE // Default to fullscreen on mobile devices
    ui->fullscreenCheckbox->setChecked(appSettings->value("fullscreen", true).toBool());
#else
    ui->fullscreenCheckbox->setChecked(appSettings->value("fullscreen", false).toBool());
#endif
    emit fullscreenChanged(ui->fullscreenCheckbox->isChecked());
    ui->simulateCheckbox->setChecked(appSettings->value("simulate", false).toBool());
    emit simulateChanged(ui->simulateCheckbox->isChecked());
    ui->serverAddressEdit->setText(appSettings->value("serveraddress", "127.0.0.1:51000").toString());
    emit setServerAddress(ui->serverAddressEdit->text());
    ui->updateIntervalSpinBox->setValue(appSettings->value("updateinterval", 0.033).toDouble());
    emit setUpdateInterval(ui->updateIntervalSpinBox->value());
    ui->interpolateCheckbox->setChecked(appSettings->value("interpolate", true).toBool());
    emit setInterpolationEnabled(ui->interpolateCheckbox->isChecked());
    ui->panelUpdateSpinBox->setValue(appSettings->value("panelupdateinterval", 0.016).toDouble());
    emit setPanelUpdateInterval(ui->panelUpdateSpinBox->value());
    ui->fontSizeSpinBox->setValue(appSettings->value("fontsize", 10).toInt());
    emit setDefaultFontSize(ui->fontSizeSpinBox->value());
    appSettings->endGroup();
}

void SettingsDialog::saveSettings() {
    appSettings->beginGroup("settings");
    appSettings->setValue("panelrotation", ui->panelRotationDial->value());
    appSettings->setValue("fullscreen", ui->fullscreenCheckbox->isChecked());
    appSettings->setValue("simulate", ui->simulateCheckbox->isChecked());
    appSettings->setValue("serveraddress", ui->serverAddressEdit->text());
    appSettings->setValue("updateinterval", ui->updateIntervalSpinBox->value());
    appSettings->setValue("interpolate", ui->interpolateCheckbox->isChecked());
    appSettings->setValue("panelupdateinterval", ui->panelUpdateSpinBox->value());
    appSettings->setValue("fontsize", ui->fontSizeSpinBox->value());
    appSettings->endGroup();
    appSettings->sync();
}
