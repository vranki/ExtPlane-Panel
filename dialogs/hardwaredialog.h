#ifndef HARDWAREDIALOG_H
#define HARDWAREDIALOG_H

#include <QDialog>
#include <QMap>

class HardwareManager;
class HardwareBinding;
class ExtPlaneConnection;

namespace Ui {
class HardwareDialog;
}

class HardwareDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HardwareDialog(QWidget *parent, HardwareManager *manager);
    ~HardwareDialog();
protected:
    void changeEvent(QEvent *e);
private slots:
    void newBinding();
    void deleteBinding();
    void saveChanges();
    void currentRowChanged(int row);
private:
    void updateUi();

    Ui::HardwareDialog *ui;
    HardwareManager *hwManager;
    HardwareBinding *currentBinding;

    QMap<int, HardwareBinding*> bindingRows; // List row <-> binding map
};

#endif // HARDWAREDIALOG_H
