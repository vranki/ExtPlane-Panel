#include "panelsdialog.h"
#include "ui_panelsdialog.h"

PanelsDialog::PanelsDialog(QWidget *parent, Settings *appSettings) :
    QDialog(parent),
    ui(new Ui::PanelsDialog)
{
    // Init
    ui->setupUi(this);
    this->appSettings = appSettings;
    this->profileSettings = NULL;

    // Net yet supported
    ui->buttonMoveUp->setVisible(false);
    ui->buttonMoveDown->setVisible(false);

    // Connections
    connect(ui->buttonAddNew,SIGNAL(clicked()),this,SLOT(newPanelClicked()));
    connect(ui->buttonCopy,SIGNAL(clicked()),this,SLOT(copyPanelClicked()));
    connect(ui->buttonRemove,SIGNAL(clicked()),this,SLOT(removePanelClicked()));
    connect(ui->buttonSelect,SIGNAL(clicked()),this,SLOT(selectPanelClicked()));
}

void PanelsDialog::refreshPanelList() {
    // Load from settings files
    ui->listWidget->clear();
    if(profileSettings != NULL) {
        foreach(QString key, profileSettings->allKeys()) {
            if(key.split('/').count() == 2 && key.startsWith("panel-") && key.endsWith("/name")) {
                QString groupName = key.replace("/name","");
                profileSettings->beginGroup(groupName);
                ui->listWidget->addItem(profileSettings->value("name").toString());
                profileSettings->endGroup();
            }
        }
    }
    ui->buttonRemove->setEnabled(ui->listWidget->count() > 1);
}

void PanelsDialog::newPanelClicked() {
    emit(newPanel());
}

void PanelsDialog::copyPanelClicked() {
    if(ui->listWidget->currentIndex().row() >= 0) {
        emit copyPanel(ui->listWidget->currentItem()->text());
    }
}

void PanelsDialog::removePanelClicked() {
    if(ui->listWidget->currentIndex().row() >= 0 && ui->listWidget->count() > 1) {
        emit removePanel(ui->listWidget->currentItem()->text());
    }
}

void PanelsDialog::selectPanelClicked() {
    if(ui->listWidget->currentIndex().row() >= 0) {
        emit loadPanel(ui->listWidget->currentItem()->text());
    }
}

PanelsDialog::~PanelsDialog()
{
    delete ui;
}
