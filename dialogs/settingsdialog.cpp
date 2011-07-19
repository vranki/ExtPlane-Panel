/* This file manages the getting and setting of application settings:
   - panel rotation
   - panel fullscreen
   - IP address
*/

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, QSettings *settings) :
    QDialog(parent),
    ui(new Ui::SettingsDialog){

/*    _rotation=0;
    _fullscreen=false;
    _serverAddress=QString("127.0.0.1:51000");
*/
    _settings = settings;

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
    /* Load settings for panel rotation, fullscreen, and address from .ini file */
    
    _settings->beginGroup("settings");
    qDebug() << Q_FUNC_INFO << _settings->value("panelrotation").toInt();
    ui->panelRotationDial->setValue(_settings->value("panelrotation").toInt());
    ui->fullscreenCheckbox->setChecked(_settings->value("fullscreen", false).toBool());
    emit fullscreenChanged(ui->fullscreenCheckbox->isChecked());
    ui->serverAddressEdit->setText(_settings->value("serveraddress", "127.0.0.1:51000").toString());
    emit setServerAddress(ui->serverAddressEdit->text());
    _settings->endGroup();

}

void SettingsDialog::saveSettings() {
    /* Load settings for panel rotation, fullscreen, and address from .ini file */
    
    _settings->beginGroup("settings");
    _settings->setValue("panelrotation", ui->panelRotationDial->value());
    _settings->setValue("fullscreen", ui->fullscreenCheckbox->isChecked());
    _settings->setValue("serveraddress", ui->serverAddressEdit->text());
    _settings->endGroup();
    
}
