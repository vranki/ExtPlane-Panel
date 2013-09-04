#include "bindingcurvedialog.h"
#include "ui_bindingcurvedialog.h"
#include "../util/interpolation.h"
#include <QSlider>
#include <QDebug>
#include <QPixmap>
#include <QPainter>

BindingCurveDialog::BindingCurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BindingCurveDialog)
{
    ui->setupUi(this);
    sliders.append(ui->verticalSlider);
    sliders.append(ui->verticalSlider_2);
    sliders.append(ui->verticalSlider_3);
    sliders.append(ui->verticalSlider_4);
    sliders.append(ui->verticalSlider_5);
    sliders.append(ui->verticalSlider_6);
    sliders.append(ui->verticalSlider_7);
    sliders.append(ui->verticalSlider_8);
    ui->outputGraphView->setScene(&scene);
    scene.addItem(&pixmapItem);
    pixmapItem.setVisible(true);
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetCurve()));
}

BindingCurveDialog::~BindingCurveDialog()
{
    delete ui;
}

void BindingCurveDialog::setBinding(HardwareBinding *b)
{
    currentBinding = b;
    ui->outputMinSpinBox->setValue(currentBinding->outputCurve().first());
    ui->outputMaxSpinBox->setValue(currentBinding->outputCurve().last());
    ui->invertCheckbox->setChecked(currentBinding->isInverted());
    updateSliderLimits();
    foreach(QSlider *slider, sliders) {
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged()));
    }
    connect(ui->outputMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(minValueChanged()));
    connect(ui->outputMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(maxValueChanged()));
    connect(ui->invertCheckbox, SIGNAL(clicked()), this, SLOT(updateValues()));

    updateTable();
    ui->outputGraphView->fitInView(&pixmapItem);
}

void BindingCurveDialog::saveChanges()
{
    currentBinding->outputCurve().replace(0, ui->outputMinSpinBox->value());
    currentBinding->outputCurve().replace(9, ui->outputMaxSpinBox->value());

    for(int i=1;i<=8;i++) {
        currentBinding->outputCurve().replace(i, sliders.at(i-1)->value());
    }

    currentBinding->setInverted(ui->invertCheckbox->isChecked());

    qDebug() << Q_FUNC_INFO << currentBinding->outputCurve();
}

void BindingCurveDialog::updateTable()
{
    double inputRange = currentBinding->inputMax() - currentBinding->inputMin();
    double inputStep = inputRange / 9;
    for(int i=0;i<=9;i++) {
        double in = currentBinding->inputMin() + i * inputStep;
        ui->tableWidget->setItem(0,i, new QTableWidgetItem(QString::number(in)));
        ui->tableWidget->setItem(1,i, new QTableWidgetItem(QString::number(currentBinding->calculateOutValue(in))));
    }
    pixmap = QPixmap(ui->outputGraphView->size());
    pixmap.fill(Qt::white);
    overlay = QPixmap(ui->outputGraphView->size());
    overlay.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPainter overlayPainter(&overlay);
    for(int x=0;x<pixmap.width();x++) {
        double input = Interpolation::linear(0, pixmap.width(), currentBinding->inputMin(), currentBinding->inputMax(), x);
        double output = currentBinding->calculateOutValue(input);
        int y = Interpolation::linear(currentBinding->outputMin(), currentBinding->outputMax(), 0, pixmap.height(), output);
        painter.setPen(Qt::black);
        painter.drawLine(x, pixmap.height() - y, x, pixmap.height());
        if(x % 50==0) {
            overlayPainter.setPen(Qt::green);
            overlayPainter.setOpacity(1);
            overlayPainter.drawText(x, 20, QString::number(input, 'g', 4));
            overlayPainter.drawText(x, 40, QString::number(output, 'g', 4));
            overlayPainter.setOpacity(0.3);
            overlayPainter.drawLine(x, 0, x, pixmap.height());
        }
    }
    painter.drawPixmap(0,0, overlay);
    pixmapItem.setPixmap(pixmap);
    ui->outputGraphView->fitInView(&pixmapItem);
}

void BindingCurveDialog::updateValues()
{
    saveChanges();
    updateTable();
}

void BindingCurveDialog::minValueChanged()
{
    saveChanges();
    currentBinding->outputValue(currentBinding->invertValueIfNeeded(ui->outputMinSpinBox->value()), currentBinding->output(), 0);
    updateSliderLimits();
    updateValues();
}

void BindingCurveDialog::maxValueChanged()
{
    saveChanges();
    currentBinding->outputValue(currentBinding->invertValueIfNeeded(ui->outputMaxSpinBox->value()), currentBinding->output(), 0);
    updateSliderLimits();
    updateValues();
}

void BindingCurveDialog::sliderChanged()
{
    QObject *sliderO = sender();
    QSlider *slider = qobject_cast<QSlider*> (sliderO);
    currentBinding->outputValue(currentBinding->invertValueIfNeeded(slider->value()), currentBinding->output(), 0);
    saveChanges();
    updateValues();
}

void BindingCurveDialog::resetCurve()
{
    updateSliderLimits();
    double v = currentBinding->outputMin();
    double d = currentBinding->outputRange() / 8;
    foreach(QSlider *slider, sliders) {
        slider->setValue(v);
        v += d;
    }
}

void BindingCurveDialog::updateSliderLimits()
{
    int i = 1;
    foreach(QSlider *slider, sliders) {
        slider->setRange(currentBinding->outputMin(), currentBinding->outputMax());
        slider->setSingleStep(currentBinding->outputRange() / 1000.0f);
        slider->setValue(currentBinding->outputCurve().at(i));
        i++;
    }
    qDebug() << "Set slider ranges to " << currentBinding->outputMin() << currentBinding->outputMax() << currentBinding->outputRange() / 1000.0f;
}

bool BindingCurveDialog::valuesInverted()
{
    return currentBinding->outputMin() > currentBinding->outputMax();
}
