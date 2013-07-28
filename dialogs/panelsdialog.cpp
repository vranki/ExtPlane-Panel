#include "panelsdialog.h"
#include "ui_panelsdialog.h"

PanelsDialog::PanelsDialog(QWidget *parent, Settings *appSettings, QSettings *profileSettings) :
    QDialog(parent),
    ui(new Ui::PanelsDialog)
{
    ui->setupUi(this);

    this->appSettings = appSettings;
    this->profileSettings = profileSettings;

    refreshPanelList();
}

void PanelsDialog::refreshPanelList() {
    // Load from settings files
    int panelNumber = 0;
    while(panelNumber >= 0) {
        profileSettings->beginGroup("panel-" + QString::number(panelNumber));
        if(profileSettings->contains("name")) {
            ui->listWidget->addItem(profileSettings->value("name").toString());
            panelNumber++;
        } else {
            panelNumber = -1;
        }
        profileSettings->endGroup();
    }
}

PanelsDialog::~PanelsDialog()
{
    delete ui;
}
