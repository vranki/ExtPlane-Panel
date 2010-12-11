#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog), settings("org.vranki", "extplane-gauges-settings", this) {
    ui->setupUi(this);
    connect(ui->panelRotationDial, SIGNAL(valueChanged(int)), this, SIGNAL(rotationChanged(int)));
    connect(ui->fullscreenCheckbox, SIGNAL(clicked(bool)), this, SIGNAL(fullscreenChanged(bool)));
    connect(ui->serverAddressEdit, SIGNAL(textChanged(QString)), this, SIGNAL(setServerAddress(QString)));
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
    ui->fullscreenCheckbox->setChecked(settings.value("fullscreen", false).toBool());
    emit fullscreenChanged(ui->fullscreenCheckbox->isChecked());
    ui->serverAddressEdit->setText(settings.value("serveraddress", "127.0.0.1:51000").toString());
    emit setServerAddress(ui->serverAddressEdit->text());
    settings.endGroup();
}

void SettingsDialog::saveSettings() {
    settings.beginGroup("settings");
    settings.setValue("panelrotation", ui->panelRotationDial->value());
    settings.setValue("fullscreen", ui->fullscreenCheckbox->isChecked());
    settings.setValue("serveraddress", ui->serverAddressEdit->text());
    settings.endGroup();
}
