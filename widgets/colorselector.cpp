#include "colorselector.h"

ColorSelector::ColorSelector(QWidget *parent) :
    QPushButton(parent)
{
    // Init
    _colorDialog = NULL;
    this->setFlat(true);
    //QGridLayout *containerLayout = new QGridLayout(this);
    //this->setLayout(containerLayout);


    //_colorWidget = new QWidget(this);
    //_colorWidget->setGeometry(0,0,100,100);
    //_colorWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    //containerLayout->addWidget(_colorWidget,0,0);

    //QPushButton *buttonWidget = new QPushButton("+",this);
    //containerLayout->addWidget(buttonWidget,0,1);
    connect(this, SIGNAL(clicked()), this, SLOT(selectColor()));




}

ColorSelector::~ColorSelector() {
    if(_colorDialog) {
        _colorDialog->close();
        delete _colorDialog;
    }
}

void ColorSelector::setColor(QString color) {
    setColor(QColor(color));
}

void ColorSelector::setColor(QColor color) {
    _currentColor = color;
    QPalette newPalette(palette());
    newPalette.setColor(QPalette::Foreground, color);
    newPalette.setColor(QPalette::Button, color);
    this->setAutoFillBackground(true);
    this->setPalette(newPalette);
}

void ColorSelector::selectColor() {
    if(_colorDialog) {
        _colorDialog->close();
        delete _colorDialog;
    }
    _colorDialog = new QColorDialog(_currentColor, this);
    _colorDialog->show();
    connect(_colorDialog,SIGNAL(colorSelected(QColor)),this,SLOT(colorSelected(QColor)));
}

void ColorSelector::colorSelected(QColor color) {
    setColor(color);
    emit colorChanged(color);
}
