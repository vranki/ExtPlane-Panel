#ifndef EDITITEMDIALOG_H
#define EDITITEMDIALOG_H

#include <QDialog>

namespace Ui {
    class EditItemDialog;
}

class PanelItem;


/**
 * Dialog for editing PanelItems. Has some common settings and
 * requests items to present their custom settings to the UI.
 *
 * @see PanelItem
 */
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
