#ifndef VELOCITYUNITCOMBOBOX_H
#define VELOCITYUNITCOMBOBOX_H

#include <QComboBox>
#include "../util/units.h"

/**
 * Allows user to pick one velocity unit
 */
class VelocityUnitComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit VelocityUnitComboBox(QWidget *parent, VelocityUnit un);
signals:
    void unitSelected(VelocityUnit unit);
public slots:
    void selectionChanged(int idx);
};

#endif // VELOCITYUNITCOMBOBOX_H
