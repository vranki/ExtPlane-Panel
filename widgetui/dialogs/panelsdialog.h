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
    explicit PanelsDialog(QWidget *parent, Settings *appSettings);
    ~PanelsDialog();

public slots:
    void refreshPanelList();
    void newPanelClicked();
    void copyPanelClicked();
    void removePanelClicked();
    void selectPanelClicked();

signals:
    void newPanel();
    void renamePanel(QString name, QString newName);
    void copyPanel(QString name);
    void removePanel(QString name);
    void loadPanel(QString name);
    
private:
    Ui::PanelsDialog *ui;
    Settings *appSettings;

public:
    QSettings *profileSettings;
};

#endif // PANELSDIALOG_H
