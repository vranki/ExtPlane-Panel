#include "menubutton.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>
#include <QCheckBox>
#include "panelitems/panelitem.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/edititemdialog.h"

MenuButton::MenuButton(QWidget *parent) : QObject(parent), side(20) {
    // Init
    panelWindow = parent;
    #ifdef MOBILE_DEVICE
        side = 50;
    #endif
    editMode = false;
    currentMenu = NULL;
}

void MenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::yellow);
    //painter->drawRect(0,0,side,side);
    painter->drawLine(0,side,side,0);
}

QRectF MenuButton::boundingRect() const {
    return QRectF(0, 0 , side, side);
}

void MenuButton::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    QGraphicsItem::mousePressEvent(event);
    if(event->button() != Qt::LeftButton) return;
    if(currentMenu) {
        currentMenu->setFocus(); //BUG: doesnt seem to work
        return;
    }

    QDialog *menu = new QDialog(panelWindow);
#if defined(Q_OS_MACX) // Don't do this on other platforms, it's annoying.
    menu->setWindowFlags(menu->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint); // For OSX
#endif
    menu->move(event->screenPos().x(), event->screenPos().y());
    QVBoxLayout *layout = new QVBoxLayout();
#if defined(Q_OS_ANDROID)
    layout->setSpacing(1); // Smaller spacing for small screens
#endif
    QCheckBox *editModeCheck = new QCheckBox("Edit Panel", menu);
    editModeCheck->setChecked(editMode);
    connect(editModeCheck, SIGNAL(clicked(bool)), panelWindow, SLOT(setEditMode(bool)));
    connect(editModeCheck, SIGNAL(clicked(bool)), this, SLOT(setEditMode(bool)));

    layout->addWidget(editModeCheck);
    QPushButton *addButton = new QPushButton("Add Item", menu);
    connect(addButton, SIGNAL(clicked()), panelWindow, SLOT(showAddItemDialog()));
    layout->addWidget(addButton);

    QPushButton *saveButton = new QPushButton("Save Profile", menu);
    connect(saveButton, SIGNAL(clicked()), panelWindow, SLOT(saveProfile()));
    layout->addWidget(saveButton);

    QPushButton *saveAsButton = new QPushButton("Save Profile As...", menu);
    connect(saveAsButton, SIGNAL(clicked()), panelWindow, SLOT(saveProfileAs()));
    layout->addWidget(saveAsButton);

    QPushButton *loadButton = new QPushButton("Load Profile", menu);
    connect(loadButton, SIGNAL(clicked()), panelWindow, SLOT(loadProfile()));
    layout->addWidget(loadButton);

    QPushButton *newButton = new QPushButton("New Profile", menu);
    connect(newButton, SIGNAL(clicked()), panelWindow, SLOT(newProfile()));
    layout->addWidget(newButton);

    QPushButton *mangePanelsButton = new QPushButton("Manage Panels", menu);
    connect(mangePanelsButton, SIGNAL(clicked()), panelWindow, SLOT(showPanels()));
    connect(mangePanelsButton, SIGNAL(clicked()), this, SLOT(closeCurrentMenu()));
    layout->addWidget(mangePanelsButton);

    QPushButton *hardwareButton = new QPushButton("Hardware", menu);
    connect(hardwareButton, SIGNAL(clicked()), panelWindow, SLOT(showHardware()));
    connect(hardwareButton, SIGNAL(clicked()), this, SLOT(closeCurrentMenu()));
    layout->addWidget(hardwareButton);

    QPushButton *settingsButton = new QPushButton("App Settings", menu);
    connect(settingsButton, SIGNAL(clicked()), panelWindow, SLOT(showSettings()));
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(closeCurrentMenu()));
    layout->addWidget(settingsButton);

    QPushButton *closeButton = new QPushButton("Close", menu);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeCurrentMenu()));
    layout->addWidget(closeButton);

    QPushButton *quitButton = new QPushButton("Quit", menu);
    connect(quitButton, SIGNAL(clicked()), panelWindow, SLOT(quit()));
    layout->addWidget(quitButton);

    currentMenu = menu;
    connect(currentMenu, SIGNAL(finished(int)), this, SLOT(closeCurrentMenu()));

    menu->setLayout(layout);
    menu->setModal(false);
    menu->show();
}

void MenuButton::setEditMode(bool em) {
    editMode = em; // @todo Set the checkbox in UI
}

void MenuButton::closeCurrentMenu() {
    if(currentMenu) {
        currentMenu->deleteLater();
        currentMenu = NULL;
    }
}

