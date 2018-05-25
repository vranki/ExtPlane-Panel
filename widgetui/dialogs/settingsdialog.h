#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

#include "../settings.h"

namespace Ui {
    class SettingsDialog;
}

/**
 * Main settings for the whole application
 */
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent, Settings *appSettings);
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
    void setAutoPanelsEnabled(bool enabled);
    void setAdjustPowerEnabled(bool enabled);
private:
    Ui::SettingsDialog *ui;
    Settings *appSettings; // General settings, passed from PanelWindow
};

#endif // SETTINGSDIALOG_H
