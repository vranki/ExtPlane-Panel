#include "datarefdebugger.h"

#include <QLineEdit>
#include <QLabel>

#include "extplaneclient.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(DataRefDebugger,"misc/debugging/dataref")

DataRefDebugger::DataRefDebugger(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeDisplay, PanelItemShapeRectangular),
        _client(this, typeName(), conn) {

    // Init
    setSize(400,60);
    setDataRefName("sim/cockpit/misc/compass_indicated");

    // Make connection
    conn->registerClient(&_client);
    //_client.subscribeDataRef("sim/flightmodel/position/hpath", 1);
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(dataRefChanged(QString,QString)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(dataRefChanged(QString,double)));

}

void DataRefDebugger::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);

    // Draw the dataref name and value
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::white);
    QFont font = defaultFont;
    font.setPixelSize(height()*0.3);
    painter->setFont(font);
    painter->drawText(QRect(0,0,width(), height()), Qt::AlignCenter, QString("%1\n%2").arg(_currentName).arg(_currentValue));

    PanelItem::paint(painter, option, widget);
}

void DataRefDebugger::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("datarefname", _currentName);
}

void DataRefDebugger::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setDataRefName(settings.value("datarefname","sim/cockpit/misc/compass_indicated").toString());
}

void DataRefDebugger::createSettings(QGridLayout *layout) {
    PanelItem::createSettings(layout);

    layout->addWidget(new QLabel("DataRef", layout->parentWidget()));
    QLineEdit *dataRefNameLineEdit = new QLineEdit(layout->parentWidget());
    dataRefNameLineEdit->setText(_currentName);
    layout->addWidget(dataRefNameLineEdit);
    connect(dataRefNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setDataRefName(QString)));
}

void DataRefDebugger::setDataRefName(QString name) {

    // Unsubscribe old
    if(_currentName != "") _client.unsubscribeDataRef(_currentName); //TODO: there seems to be something wrong with unsubscribing...
    _currentName = name;
     _currentValue = "";

    // Subscribe new
    _client.subscribeDataRef(name, 0);
    update();
}

void DataRefDebugger::dataRefChanged(QString name, QString val) {
    _currentValue = val;
    update();
}

void DataRefDebugger::dataRefChanged(QString name, double val) {
    dataRefChanged(name,QString("%1").arg(val));
}
