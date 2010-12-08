#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog), settings("org.vranki", "extplane-gauges-settings", this) {
    ui->setupUi(this);
    connect(ui->panelRotationDial, SIGNAL(valueChanged(int)), this, SIGNAL(rotationChanged(int)));
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
    settings.beginGroup("settings");
    qDebug() << Q_FUNC_INFO << settings.value("panelrotation").toInt();
    ui->panelRotationDial->setValue(settings.value("panelrotation").toInt());
    settings.endGroup();
}

void SettingsDialog::saveSettings() {
    settings.beginGroup("settings");
    settings.setValue("panelrotation", ui->panelRotationDial->value());
    settings.endGroup();
}
