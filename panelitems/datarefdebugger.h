#ifndef DATAREFDEBUGGER_H
#define DATAREFDEBUGGER_H

#include "panelitem.h"

class DataRefDebugger : public PanelItem
{
    Q_OBJECT

public:
    Q_INVOKABLE DataRefDebugger(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);

public slots:
    void dataRefChanged(QString name, double val);
    void dataRefChanged(QString name, QString val);
    void dataRefChanged(QString name, QStringList values);
    void setDataRefName(QString name);
    void setDataRefAccuracy(float accuracy);
    void setShowAllIndexes( bool showAll );
    void setChosenIndex( float index ); // is float to avoid error from UI call

private:
    ExtPlaneClient _client;
    QString _currentValue;
    QString _currentName;
    double _currentAccuracy;
    bool _showAllIndexes;
    int _chosenIndex; // UI index input from user
    int _validIndex; // chosenIndex clamped to dataref array length
};

#endif // DATAREFDEBUGGER_H
