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
    void enableSB(bool enable);
    void enablePololu(bool enable);
    void enableChroma(bool enable);
    void deviceAvailable(int dev, bool avail);
    void showOutputCurvesDialog();
signals:
    void deviceEnabled(int dev, bool enable);
private:
    void updateUi();

    Ui::HardwareDialog *ui;
    HardwareManager *hwManager;
    HardwareBinding *currentBinding;

    QMap<int, HardwareBinding*> bindingRows; // List row <-> binding map
};

#endif // HARDWAREDIALOG_H
