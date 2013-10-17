#ifndef DIAL_H
#define DIAL_H

#include "panelitem.h"

class Dial : public PanelItem {
    Q_OBJECT

public:
    Q_INVOKABLE Dial(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
    virtual void applySettings();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:

public slots:
    void setLabel(QString val);
    void setDataRef(QString val);
    void setPositionLabel1(QString val) { _positionLabel1 = val; updatePositions(); }
    void setPositionValue1(QString val) { _positionValue1 = val; updatePositions(); }
    void setPositionLabel2(QString val) { _positionLabel2 = val; updatePositions(); }
    void setPositionValue2(QString val) { _positionValue2 = val; updatePositions(); }
    void setPositionLabel3(QString val) { _positionLabel3 = val; updatePositions(); }
    void setPositionValue3(QString val) { _positionValue3 = val; updatePositions(); }
    void setPositionLabel4(QString val) { _positionLabel4 = val; updatePositions(); }
    void setPositionValue4(QString val) { _positionValue4 = val; updatePositions(); }
    void setPositionLabel5(QString val) { _positionLabel5 = val; updatePositions(); }
    void setPositionValue5(QString val) { _positionValue5 = val; updatePositions(); }
    void loadPreset(int val);

private slots:
    void valueChanged(QString ref, double newValue);
    void valueChanged(QString ref, QString newValue);
    void updatePositions();

private:
    ExtPlaneClient _client;
    int _dialPosition;
    int _dialPositions;
    QString _label;
    QString _datarefName;
    ClientDataRef* _dataref;
    QString _value;
    QString _positionLabel1;
    QString _positionValue1;
    QString _positionLabel2;
    QString _positionValue2;
    QString _positionLabel3;
    QString _positionValue3;
    QString _positionLabel4;
    QString _positionValue4;
    QString _positionLabel5;
    QString _positionValue5;
};

#endif // DIAL_H
