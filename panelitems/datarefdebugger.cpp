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
    _currentValue = "";
    _currentAccuracy = 0;
    setDataRefName("sim/cockpit/misc/compass_indicated");

    // Make connection
    conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(dataRefChanged(QString,QString)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(dataRefChanged(QString,double)));
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(dataRefChanged(QString,QStringList)));

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
    settings.setValue("datarefaccuracy", _currentAccuracy);
}

void DataRefDebugger::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setDataRefAccuracy(settings.value("datarefaccuracy",0).toDouble());
    setDataRefName(settings.value("datarefname","sim/cockpit/misc/compass_indicated").toString());
}

void DataRefDebugger::createSettings(QGridLayout *layout) {
    PanelItem::createSettings(layout);

    layout->addWidget(new QLabel("DataRef", layout->parentWidget()));
    QLineEdit *dataRefNameLineEdit = new QLineEdit(layout->parentWidget());
    dataRefNameLineEdit->setText(_currentName);
    layout->addWidget(dataRefNameLineEdit);
    connect(dataRefNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setDataRefName(QString)));

    createNumberInputSetting(layout,"Accuracy", _currentAccuracy, SLOT(setDataRefAccuracy(float)));
}

void DataRefDebugger::setDataRefName(QString name) {

    // Unsubscribe old
    if(_currentName != "") _client.unsubscribeDataRef(_currentName); //TODO: there seems to be something wrong with unsubscribing...
    _currentName = name;
    _currentValue = "";

    // Subscribe new
    if(name != "") _client.subscribeDataRef(name, _currentAccuracy);
    update();
}

void DataRefDebugger::setDataRefAccuracy(float val) {
    _currentAccuracy = val;
    setDataRefName(_currentName);
}

void DataRefDebugger::dataRefChanged(QString name, QString val) {
    _currentValue = val;
    update();
}

void DataRefDebugger::dataRefChanged(QString name, double val) {
    dataRefChanged(name,QString("%1").arg(val));
}

void DataRefDebugger::dataRefChanged(QString name, QStringList values) {
    QString out = "[";
    for(int i = 0; i < values.count(); i++) {
        if(i != 0) out += ",";
        out.append(values.at(i));
    }
    out.append("]");
    dataRefChanged(name,out);
}
