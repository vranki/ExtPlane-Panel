#include "menubutton.h"

MenuButton::MenuButton(QWidget *parent, QList<PanelItem*> &gaugelist, PanelItemFactory *gf) :
        QObject(parent), panelItems(gaugelist), itemFactory(gf), settings("org.vranki", "extplane-gauges-panels", this)
         {
    parentWidget = parent;
    side = 20;
    msg = 0;
    editMode = false;
    settingsDialog = new SettingsDialog(parentWidget);
    connect(settingsDialog, SIGNAL(rotationChanged(int)), this, SIGNAL(panelRotationChanged(int)));
    connect(settingsDialog, SIGNAL(fullscreenChanged(bool)), this, SIGNAL(fullscreenChanged(bool)));
    connect(settingsDialog, SIGNAL(setServerAddress(QString)), this, SIGNAL(setServerAddress(QString)));
    settingsDialog->setModal(false);
    settingsDialog->hide();

    editItemDialog = 0;
}

void MenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::yellow);
    painter->drawRect(0,0,side,side);
    painter->drawLine(0,side,side,0);
}

QRectF MenuButton::boundingRect() const {
    return QRectF(0, 0 , side, side);
}

void MenuButton::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    QGraphicsItem::mousePressEvent(event);
    if(event->button()!=Qt::LeftButton)
        return;
    msg = new QDialog(parentWidget);
    msg->move(event->screenPos().x(), event->screenPos().y());
    QVBoxLayout *layout = new QVBoxLayout();
    QCheckBox *editModeCheck = new QCheckBox("Edit Panel", msg);
    editModeCheck->setChecked(editMode);
    connect(editModeCheck, SIGNAL(clicked(bool)), this, SIGNAL(editModeChanged(bool)));
    connect(editModeCheck, SIGNAL(clicked(bool)), this, SLOT(setEditMode(bool)));

    layout->addWidget(editModeCheck);
    QPushButton *addButton = new QPushButton("Add Item", msg);
    connect(addButton, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(msg, SIGNAL(rejected()), this, SLOT(closeDialog()));
    layout->addWidget(addButton);

    if(!selectedGauges().isEmpty()) {
        QPushButton *deleteButton = new QPushButton("Delete Item(s)", msg);
        connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteItems()));
        layout->addWidget(deleteButton);
        if(selectedGauges().size()==1) {
            QPushButton *editButton = new QPushButton("Item Properties", msg);
            connect(editButton, SIGNAL(clicked()), this, SLOT(itemProperties()));
            layout->addWidget(editButton);
        }
    }
    QPushButton *saveButton = new QPushButton("Save panel", msg);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(savePanel()));
    layout->addWidget(saveButton);
    QPushButton *loadButton = new QPushButton("Load panel", msg);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadPanel()));
    layout->addWidget(loadButton);
    QPushButton *settingsButton = new QPushButton("App Settings", msg);
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(showSettings()));
    layout->addWidget(settingsButton);
    QPushButton *closeButton = new QPushButton("Close", msg);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeDialog()));
    layout->addWidget(closeButton);
    QPushButton *quitButton = new QPushButton("Quit", msg);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    layout->addWidget(quitButton);
    msg->setLayout(layout);
    msg->exec();
}

QList<PanelItem*> MenuButton::selectedGauges() {
    QList<PanelItem*> selection;
    foreach(PanelItem* g, panelItems) {
        if(g->isSelected())
            selection.append(g);
    }
    return selection;
}

void MenuButton::addItem() {
    bool ok;

    QString item = QInputDialog::getItem(parentWidget, "Add item", "Choose type:", itemFactory->itemNames(), 0, false, &ok);
    if(ok) {
        PanelItem *g=itemFactory->itemForName(item, parentWidget);
        if(g)
            emit itemAdded(g);
    }

    closeDialog();
}

void MenuButton::deleteItems() {
    QList<PanelItem*> selection = selectedGauges();
    foreach(PanelItem* g, selection) {
//        delete g;
        Q_ASSERT(panelItems.removeOne(g));
        g->deleteLater();
    }

    closeDialog();
}

void MenuButton::savePanel() {
    settingsDialog->saveSettings();
    int panelNumber = 0;
    QString panelName = "Panel";
    settings.clear();
    settings.beginGroup("panel-" + QString::number(panelNumber));
    settings.setValue("number", panelNumber);
    settings.setValue("name", panelName);
    settings.setValue("gaugecount", panelItems.size());
    int gn = 0;
    foreach(PanelItem *g, panelItems) {
        settings.beginGroup("gauge-" + QString::number(gn));
        g->storeSettings(settings);
        settings.endGroup();
        gn++;
    }
    settings.endGroup();
    settings.sync();
    closeDialog();
}

void MenuButton::closeDialog() {
    msg->deleteLater();
}

void MenuButton::loadPanel() {
    settingsDialog->loadSettings();

    foreach(PanelItem *g, panelItems) {
        g->deleteLater();
    }
    int panelNumber = 0;
    while(panelNumber >= 0) {
        settings.beginGroup("panel-" + QString::number(panelNumber));
        if(settings.contains("name")) {
            int gc = settings.value("gaugecount", 0).toInt();
            QString name = settings.value("name").toString();
            for(int gn=0;gn<gc;gn++) {
                settings.beginGroup("gauge-" + QString::number(gn));
                PanelItem *g = itemFactory->itemForName(settings.value("type").toString(), parentWidget);
                if(g) {
                    emit itemAdded(g);
                    g->loadSettings(settings);
                    g->applySettings();
                } else {
                    qDebug() << Q_FUNC_INFO << "Can't load item of type " << settings.value("type").toString();
                }
                settings.endGroup();
            }
            panelNumber++;
        } else {
            panelNumber = -1;
        }
        settings.endGroup();
    }

    closeDialog();
}


void MenuButton::showSettings() {
    closeDialog();
    settingsDialog->show();
}
void MenuButton::quit() {
    QCoreApplication::quit();
}

void MenuButton::itemProperties() {
    closeDialog();
    if(editItemDialog)
        delete editItemDialog;
    editItemDialog =  new EditItemDialog(parentWidget);
    editItemDialog->setModal(false);
    editItemDialog->setPanelItem(selectedGauges().first());
    editItemDialog->show();
}

void MenuButton::setEditMode(bool em) {
    editMode = em;
}
