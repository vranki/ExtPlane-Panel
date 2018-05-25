#ifndef ENGINEBATTERY_H
#define ENGINEBATTERY_H

#include "panelitem.h"

class EngineBattery : public PanelItem
{
public:
    Q_INVOKABLE EngineBattery(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    ~EngineBattery();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
    virtual void itemSizeChanged(float w, float h);

signals:
    public slots:
    void amperageChanged(QString name, QStringList values);
    void setBatteryNumber(float val);
    void setMaxValue(float mv);
    void setMinValue(float mv);

private:
    Q_OBJECT
    void drawBottomPixmap();
    float value2Angle(const float &p);

    int _batteryNumber;
    float amperageValue;
    float valueMin;
    float valueMax;
    float scaleFactor;

    ExtPlaneClient _client;
    const QImage bottomImage;
    QPixmap* bottomPixmap;

};

#endif // ENGINEBATTERY_H
