#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QDebug>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent);
    ~SettingsDialog();
    void loadSettings();
    void saveSettings();
protected:
    void changeEvent(QEvent *e);
signals:
    void rotationChanged(int r);
    void fullscreenChanged(bool fs);
    void setServerAddress(QString host);
private:
    Ui::SettingsDialog *ui;
    QSettings settings;
};

#endif // SETTINGSDIALOG_H
