#include "velocityunitcombobox.h"

VelocityUnitComboBox::VelocityUnitComboBox(QWidget *parent, VelocityUnit un=VELOCITY_KMH) :
    QComboBox(parent) {
    addItem(Units::unitName(VELOCITY_KMH));
    addItem(Units::unitName(VELOCITY_KTS));
    addItem(Units::unitName(VELOCITY_MS));
    addItem(Units::unitName(VELOCITY_FPM));
    addItem(Units::unitName(VELOCITY_KFPM));
    if(un == VELOCITY_KMH) setCurrentIndex(0);
    if(un == VELOCITY_KTS) setCurrentIndex(1);
    if(un == VELOCITY_MS) setCurrentIndex(2);
    if(un == VELOCITY_FPM) setCurrentIndex(3);
    if(un == VELOCITY_KFPM) setCurrentIndex(4);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionChanged(int)));
}

void VelocityUnitComboBox::selectionChanged(int idx) {
    if(idx == 0) emit unitSelected(VELOCITY_KMH);
    if(idx == 1) emit unitSelected(VELOCITY_KTS);
    if(idx == 2) emit unitSelected(VELOCITY_MS);
    if(idx == 3) emit unitSelected(VELOCITY_FPM);
    if(idx == 4) emit unitSelected(VELOCITY_KFPM);
}
