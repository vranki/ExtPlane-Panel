#ifndef PANELSDIALOG_H
#define PANELSDIALOG_H

#include <QDialog>
#include <QSettings>

#include "../settings.h"

namespace Ui {
class PanelsDialog;
}

class PanelsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PanelsDialog(QWidget *parent, Settings *appSettings, QSettings *profileSettings);
    ~PanelsDialog();

public slots:
    void refreshPanelList();
    
private:
    Ui::PanelsDialog *ui;
    Settings *appSettings;
    QSettings *profileSettings;
};

#endif // PANELSDIALOG_H
