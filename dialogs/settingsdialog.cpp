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
    connect(ui->updateIntervalComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIntervalChanged()));
    connect(ui->interpolateCheckbox, SIGNAL(toggled(bool)), this, SIGNAL(setInterpolationEnabled(bool)));
    connect(ui->panelUpdateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(panelUpdateIntervalChanged()));
    connect(ui->fontSizeSpinBox, SIGNAL(valueChanged(double)), this, SIGNAL(setDefaultFontSize(double)));
    connect(ui->antialiasCheckbox, SIGNAL(toggled(bool)), this, SIGNAL(setAntialiasEnabled(bool)));
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

    ui->updateIntervalComboBox->setCurrentIndex(appSettings->value("updateinterval", 0).toInt());
    updateIntervalChanged();

    ui->interpolateCheckbox->setChecked(appSettings->value("interpolate", true).toBool());
    emit setInterpolationEnabled(ui->interpolateCheckbox->isChecked());
    ui->antialiasCheckbox->setChecked(appSettings->value("antialias", true).toBool());
    emit setAntialiasEnabled(ui->antialiasCheckbox->isChecked());

    ui->panelUpdateComboBox->setCurrentIndex(appSettings->value("panelupdateinterval", 4).toInt());

    ui->fontSizeSpinBox->setValue(appSettings->value("fontsize", 10).toDouble());
    emit setDefaultFontSize(ui->fontSizeSpinBox->value());
    appSettings->endGroup();
}

void SettingsDialog::saveSettings() {
    appSettings->beginGroup("settings");
    appSettings->setValue("panelrotation", ui->panelRotationDial->value());
    appSettings->setValue("fullscreen", ui->fullscreenCheckbox->isChecked());
    appSettings->setValue("simulate", ui->simulateCheckbox->isChecked());
    appSettings->setValue("serveraddress", ui->serverAddressEdit->text());
    appSettings->setValue("updateinterval", ui->updateIntervalComboBox->currentIndex());
    appSettings->setValue("interpolate", ui->interpolateCheckbox->isChecked());
    appSettings->setValue("antialias", ui->antialiasCheckbox->isChecked());
    appSettings->setValue("panelupdateinterval", ui->panelUpdateComboBox->currentIndex());
    appSettings->setValue("fontsize", ui->fontSizeSpinBox->value());
    appSettings->endGroup();
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
