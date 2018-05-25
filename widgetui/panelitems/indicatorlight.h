#ifndef INDICATORLIGHT_H
#define INDICATORLIGHT_H

#include "panelitem.h"

#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>

class IndicatorLight : public PanelItem
{
    Q_OBJECT

public:
    Q_INVOKABLE IndicatorLight(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual ~IndicatorLight();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
    virtual void itemSizeChanged(float w, float h);
    virtual void settingChanged();
    virtual void setPos(int x, int y);
    void createLabel(int w, int h);

public slots:
    void dataRefChanged(QString name, double val);
    void dataRefChanged(QString name, QString val);
    void setDataRefName(QString name);
    void setThreshold(float val) { _threshold = val; }
    void setLabelColor(QColor val) { _labelColor = val; createLabel(width(),height()); }
    void setLabelOn(QString val) { _labelOn = val; }
    void setLabelOff(QString val) { _labelOff = val; }
    void setGlowStrength(int val) { _glowStrength = val; _glowEnabled=(_glowStrength!=0); }
    void setStrengthOn(int val) { _strengthOn = val; }
    void setStrengthOff(int val) { _strengthOff = val; }
    void loadPreset(int val);

private:
    // Internal variables
    ExtPlaneClient _client;
    double _datarefValue;
    bool _on;
    QGraphicsPixmapItem *_labelGlowItem;

    // Configuration variables
    QString _labelOn;
    QString _labelOff;
    bool _glowEnabled;
    QColor _labelColor;
    QString _datarefName;
    double _threshold;
    int _strengthOn;
    int _strengthOff;
    int _glowStrength;
};

#endif // INDICATORLIGHT_H
