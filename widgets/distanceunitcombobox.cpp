#include "distanceunitcombobox.h"

DistanceUnitComboBox::DistanceUnitComboBox(QWidget *parent, DistanceUnit un) : QComboBox(parent) {
    addItem(Units::unitName(DISTANCE_M));
    addItem(Units::unitName(DISTANCE_FT));
    if(un == DISTANCE_M) setCurrentIndex(0);
    if(un == DISTANCE_FT) setCurrentIndex(1);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionChanged(int)));
}

void DistanceUnitComboBox::selectionChanged(int idx) {
    if(idx == 0) emit unitSelected(DISTANCE_M);
    if(idx == 1) emit unitSelected(DISTANCE_FT);
}
