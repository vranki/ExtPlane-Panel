/*
 *  Created by nicolou on 2015/06/18.
 */

#ifndef ENGINEFUELP_H
#define ENGINEFUELP_H

#include "panelitem.h"

class EngineFuelP: public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE EngineFuelP(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    ~EngineFuelP();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
    virtual void itemSizeChanged(float w, float h);
signals:
    
    public slots:
    void pressureChanged(QString name, QStringList values);
    void setEngineNumber(float val);
    void setMaxValue(float mv);
    void setGreenBeginValue(float mv);
    void setGreenEndValue(float mv);

private:
    void drawBottomPixmap();
    float value2Angle(const float &p);

    int _engineNumber;
    float pressureValue;
    float pressureValueMin;
    float pressureValueMax;
    float scaleFactor;
    float pressureGreenBegin;
    float pressureGreenEnd;

    bool allowUpdateBottomPixmap;
    ExtPlaneClient _client;
    const QImage bottomImage;
    QPixmap* bottomPixmap;
    QPixmap needleImage;
};

#endif // ENGINEFUELP_H

