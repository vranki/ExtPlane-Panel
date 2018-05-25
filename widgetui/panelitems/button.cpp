#include "button.h"

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include "../widgets/numberinputlineedit.h"
#include "extplaneclient.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(Button,"buttons/generic");

Button::Button(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeSwitch, PanelItemShapeRectangular),
        _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _buttonId = 0;
    _label = "Button";
    _isKey = false;
    _isHeld = false;
    setSize(100,40);
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);

    if(_isHeld) {
        painter->setBrush(Qt::gray);
    } else {
        painter->setBrush(Qt::black);
    }
    painter->setPen(Qt::white);
    painter->drawRoundedRect(0,0,width(),height(), 5, 5);
    QFont font = defaultFont;
    font.setPixelSize(height()*0.75);
    painter->setFont(font);
    painter->drawText(QRect(0,0,width(), height()), Qt::AlignCenter, _label);
    PanelItem::paint(painter, option, widget);
}

void Button::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("label", _label);
    settings.setValue("id", _buttonId);
    settings.setValue("iskey", _isKey);
}

void Button::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    setButtonId(settings.value("id", 0).toInt());
    setLabel(settings.value("label", "Button").toString());
    setIsKey(settings.value("iskey", false).toBool());
}


void Button::createSettings(QGridLayout *layout) {
    layout->addWidget(new QLabel("Label", layout->parentWidget()));
    QLineEdit *labelEdit = new QLineEdit(_label, layout->parentWidget());
    connect(labelEdit, SIGNAL(textChanged(QString)), this, SLOT(setLabel(QString)));
    layout->addWidget(labelEdit);

    layout->addWidget(new QLabel("Key/Button id", layout->parentWidget()));
    NumberInputLineEdit *idEdit = new NumberInputLineEdit(layout->parentWidget());
    idEdit->setText(QString::number(_buttonId));
    layout->addWidget(idEdit);
    connect(idEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setButtonId(float)));

    layout->addWidget(new QLabel("", layout->parentWidget()));

    QCheckBox *isKeyCheckbox = new QCheckBox("Is a key (not button)", layout->parentWidget());
    isKeyCheckbox->setChecked(_isKey);
    layout->addWidget(isKeyCheckbox);
    connect(isKeyCheckbox, SIGNAL(clicked(bool)), this, SLOT(setIsKey(bool)));
}

void Button::setButtonId(float id) {
    _buttonId = id;
}

void Button::setLabel(QString txt) {
    _label = txt;
    update();
}

void Button::setIsKey(bool is) {
    _isKey = is;
}

void Button::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    if(isEditMode()) {
        PanelItem::mousePressEvent(event);
    } else {
        _isHeld = true;
        if(_isKey) {
            _client.keyPress(_buttonId);
        } else {
            _client.buttonPress(_buttonId);
        }
        update();
    }
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(isEditMode()) {
        PanelItem::mouseReleaseEvent(event);
    } else {
        _isHeld = false;
        if(!_isKey) {
            _client.buttonRelease(_buttonId);
        }
        update();
    }
}
