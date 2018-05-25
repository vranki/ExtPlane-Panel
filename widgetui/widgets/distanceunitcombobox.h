#ifndef DISTANCEUNITCOMBOBOX_H
#define DISTANCEUNITCOMBOBOX_H

#include <QComboBox>
#include "../util/units.h"

/**
 * Allows user to pick one distance unit
 */
class DistanceUnitComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DistanceUnitComboBox(QWidget *parent, DistanceUnit un);
signals:
    void unitSelected(DistanceUnit unit);
public slots:
    void selectionChanged(int idx);
};

#endif // DISTANCEUNITCOMBOBOX_H
