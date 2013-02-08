#ifndef EDITITEMDIALOG_H
#define EDITITEMDIALOG_H

#include <QDialog>
#include "panelitems/panelitem.h"

namespace Ui {
    class EditItemDialog;
}

class EditItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditItemDialog(QWidget *parent = 0);
    ~EditItemDialog();
public slots:
    void setPanelItem(PanelItem *it=0);
protected:
    void changeEvent(QEvent *e);
private slots:
    void applySettings();
    void deleteItemClicked();
private:
    Ui::EditItemDialog *ui;
    PanelItem *currentItem;
};

#endif // EDITITEMDIALOG_H
