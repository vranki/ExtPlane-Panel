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
    void createGaugeSetSettings(QGridLayout *layout, QString name, bool enabled, double dataRefMin, double dataRefMax, double rangeMin, double rangeMax, QColor color);

signals:
    
public slots:
    void refChanged(QString name, double value);
    void refChanged(QString name, QStringList values);
    void setBarLabels(int val) {_barLabels=val;}

    void setN1Enabled(bool val) {_n1Enabled=val;}
    void setN1DatarefMinimum(float val) {_n1RangeMinimum=val;}
    void setN1DatarefMaximum(float val) {_n1RangeMaximum=val;}
    void setN1RangeMinimum(float val) {_n1RangeMinimum=val;}
    void setN1RangeMaximum(float val) {_n1RangeMaximum=val;}
    void setN1Color(QColor val) {_n1Color=val;}

    void setEPREnabled(bool val) {_eprEnabled=val;}
    void setEPRDatarefMinimum(float val) {_eprRangeMinimum=val;}
    void setEPRDatarefMaximum(float val) {_eprRangeMaximum=val;}
    void setEPRRangeMinimum(float val) {_eprRangeMinimum=val;}
    void setEPRRangeMaximum(float val) {_eprRangeMaximum=val;}
    void setEPRColor(QColor val) {_eprColor=val;}

    void setEGTEnabled(bool val) {_egtEnabled=val;}
    void setEGTDatarefMinimum(float val) {_egtRangeMinimum=val;}
    void setEGTDatarefMaximum(float val) {_egtRangeMaximum=val;}
    void setEGTRangeMinimum(float val) {_egtRangeMinimum=val;}
    void setEGTRangeMaximum(float val) {_egtRangeMaximum=val;}
    void setEGTColor(QColor val) {_egtColor=val;}


protected:
    int _engineCount;
    int _barLabels;

    bool        _n1Enabled;
    QStringList _n1Values;
    double      _n1DatarefMinimum;
    double      _n1DatarefMaximum;
    double      _n1RangeMinimum;
    double      _n1RangeMaximum;
    QColor      _n1Color;

    bool        _eprEnabled;
    QStringList _eprValues;
    double      _eprDatarefMinimum;
    double      _eprDatarefMaximum;
    double      _eprRangeMinimum;
    double      _eprRangeMaximum;
    QColor      _eprColor;

    bool        _egtEnabled;
    QStringList _egtValues;
    double      _egtDatarefMinimum;
    double      _egtDatarefMaximum;
    double      _egtRangeMinimum;
    double      _egtRangeMaximum;
    QColor      _egtColor;
    
};

#endif // ENGINEDISPLAY_H
