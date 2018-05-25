#ifndef TANKLEVELER_H
#define TANKLEVELER_H

#include "panelitem.h"

class TankLeveler : public PanelItem
{
public:
    Q_INVOKABLE TankLeveler(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    ~TankLeveler();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
    virtual void itemSizeChanged(float w, float h);

signals:
    public slots:
    void quantityChanged(QString name, QStringList values);
    void setTankNumber(float val);
    void setMaxValue(float mv);
    void setShortDesignation(QString s);

private:
    Q_OBJECT
    void drawBottomPixmap();
    float value2Angle(const float &p);

    int _tankNumber;
    QString tankShortDesignation;
    float quantityValue;
    float valueMax;
    float scaleFactor;

    ExtPlaneClient _client;
    const QImage bottomImage;
    QPixmap* bottomPixmap;
    QPixmap needleImage;

};

#endif // TANKLEVELER_H
