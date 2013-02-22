#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
    class SettingsDialog;
}

/**
 * Main settings for the whole application
 */
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent, QSettings *appSettings);
    ~SettingsDialog();
public slots:
    void loadSettings();
    void saveSettings();
    void updateIntervalChanged();
    void panelUpdateIntervalChanged();
protected:
    void changeEvent(QEvent *e);
signals:
    void rotationChanged(int r);
    void fullscreenChanged(bool fs);
    void simulateChanged(bool simulate);
    void setServerAddress(QString host);
    void setUpdateInterval(double newInterval);
    void setPanelUpdateInterval(double newInterval);
    void setInterpolationEnabled(bool ie);
    void setAntialiasEnabled(bool ie);
    void setDefaultFontSize(double newSize);
private:
    Ui::SettingsDialog *ui;
    QSettings *appSettings;
};

#endif // SETTINGSDIALOG_H
