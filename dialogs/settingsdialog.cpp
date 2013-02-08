#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, QSettings *appSettings) :
    QDialog(parent), ui(new Ui::SettingsDialog), appSettings(appSettings) {
    ui->setupUi(this);
    connect(ui->panelRotationDial, SIGNAL(valueChanged(int)), this, SIGNAL(rotationChanged(int)));
    connect(ui->fullscreenCheckbox, SIGNAL(clicked(bool)), this, SIGNAL(fullscreenChanged(bool)));
    connect(ui->simulateCheckbox, SIGNAL(clicked(bool)), this, SIGNAL(simulateChanged(bool)));
    connect(ui->serverAddressEdit, SIGNAL(textChanged(QString)), this, SIGNAL(setServerAddress(QString)));
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
    qDebug() << Q_FUNC_INFO;
    appSettings->beginGroup("settings");
    ui->panelRotationDial->setValue(appSettings->value("panelrotation").toInt());
#ifdef MOBILE_DEVICE
    ui->fullscreenCheckbox->setChecked(appSettings->value("fullscreen", true).toBool());
#else
    ui->fullscreenCheckbox->setChecked(appSettings->value("fullscreen", false).toBool());
#endif
    emit fullscreenChanged(ui->fullscreenCheckbox->isChecked());
    ui->simulateCheckbox->setChecked(appSettings->value("simulate", false).toBool());
    emit simulateChanged(ui->simulateCheckbox->isChecked());
    ui->serverAddressEdit->setText(appSettings->value("serveraddress", "127.0.0.1:51000").toString());
    emit setServerAddress(ui->serverAddressEdit->text());
    appSettings->endGroup();
}

void SettingsDialog::saveSettings() {
    qDebug() << Q_FUNC_INFO;
    appSettings->beginGroup("settings");
    appSettings->setValue("panelrotation", ui->panelRotationDial->value());
    appSettings->setValue("fullscreen", ui->fullscreenCheckbox->isChecked());
    appSettings->setValue("simulate", ui->simulateCheckbox->isChecked());
    appSettings->setValue("serveraddress", ui->serverAddressEdit->text());
    appSettings->endGroup();
    appSettings->sync();
}
