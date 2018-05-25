#ifndef COLORSELECTOR_H
#define COLORSELECTOR_H

#include <QWidget>
#include <QColorDialog>
#include <QPushButton>
#include <QGridLayout>

class ColorSelector : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorSelector(QWidget *parent = 0);
    ~ColorSelector();
    
signals:
    void colorChanged(QColor);
    
public slots:
    void setColor(QString color);
    void setColor(QColor color);
    void selectColor();
    void colorSelected(QColor color);

private:
    QColorDialog *_colorDialog;
    QWidget *_colorWidget;
    QColor _currentColor;
};

#endif // COLORSELECTOR_H
