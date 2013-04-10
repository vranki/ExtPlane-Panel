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
    void setLabelColor(QColor color) { _labelColor = color; }

private:
    // Internal variables
    ExtPlaneClient _client;
    double _datarefValue;
    bool _on;
    QGraphicsPixmapItem _labelGlowItem;
    QGraphicsBlurEffect _labelGlowEffect;

    // Configuration variables
    QString _labelOn;
    QString _labelOff;
    bool _labelGlow;
    QColor _labelColor;
    QString _datarefName;
    double _threshold;
    int _strengthOn;
    int _strengthOff;
};

#endif // INDICATORLIGHT_H
