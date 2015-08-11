#include "numberinputlineedit.h"

NumberInputLineEdit::NumberInputLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setValidator(new QDoubleValidator(this));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(changed()));
}

void NumberInputLineEdit::changed() {
    bool ok = false;
    float val = QLocale().toFloat(text(),&ok);
    if(ok)
        emit valueChangedFloat(val);
}
