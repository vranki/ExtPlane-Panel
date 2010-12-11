#ifndef NUMBERINPUTLINEEDIT_H
#define NUMBERINPUTLINEEDIT_H

#include <QLineEdit>
#include <QDoubleValidator>

class NumberInputLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit NumberInputLineEdit(QWidget *parent = 0);

signals:
    void valueChangedFloat(float val);
public slots:
    void changed();
};

#endif // NUMBERINPUTLINEEDIT_H
