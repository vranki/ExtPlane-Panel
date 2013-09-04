#ifndef BINDINGCURVEDIALOG_H
#define BINDINGCURVEDIALOG_H

#include <QDialog>
#include "../hardware/hardwarebinding.h"
#include <QList>
#include <QSlider>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

namespace Ui {
class BindingCurveDialog;
}

class BindingCurveDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BindingCurveDialog(QWidget *parent = 0);
    ~BindingCurveDialog();
    void setBinding(HardwareBinding *b);
    void updateTable();
private slots:
    void saveChanges();
    void updateValues();
    void minValueChanged();
    void maxValueChanged();
    void sliderChanged();
    void resetCurve();
private:
    void updateSliderLimits();
    bool valuesInverted();

    Ui::BindingCurveDialog *ui;
    HardwareBinding *currentBinding;
    QList<QSlider*> sliders;
    QGraphicsScene scene;
    QGraphicsPixmapItem pixmapItem;
    QPixmap pixmap, overlay;
};

#endif // BINDINGCURVEDIALOG_H
