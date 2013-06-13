#ifndef ENGINEDISPLAY_H
#define ENGINEDISPLAY_H

#include "displayinstrument.h"

/**
 * Displays the engine thrust and rpm...
 */
class EngineDisplay : public DisplayInstrument
{
    Q_OBJECT
public:
    Q_INVOKABLE EngineDisplay(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void render(QPainter *painter, int width, int height);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
    
signals:
    
public slots:
    void refChanged(QString name, QString valueString);
    void setBarLabels(int val) {_barLabels=val;}

protected:
    int _engineCount;
    int _barLabels;
    QStringList _throttleValues;
    QStringList _rpmValues;
    
};

#endif // ENGINEDISPLAY_H
