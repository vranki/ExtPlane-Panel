#ifndef ENGINEDISPLAY_H
#define ENGINEDISPLAY_H

#include "displayinstrument.h"

/**
 * Displays the engine N1, EPR and EGT for any number of engines for Glass Cockpit.
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

private:
    void drawVerticalBarGauge(QPainter *painter, double strokeWidth, QColor color, double x, double y, double width, double height, double hPadding, double value, double minValue, double maxValue, double rangeStart, double rangeEnd, bool decimalPrecision, int labelCount);

signals:
    
public slots:
    void refChanged(QString name, double value);
    void refChanged(QString name, QStringList values);

    void setStyle(int val) {_style=val;}
    void setBarLabels(int val) {_barLabels=val;}
    void setAutoMin(bool val) {_autoMin=val;}
    void setAutoMax(bool val) {_autoMax=val;}

    void setN1Enabled(bool val) {_n1Enabled=val;}
    void setN1DatarefMinimum(float val) {_n1RangeMinimum=val;}
    void setN1DatarefMaximum(float val) {_n1RangeMaximum=val;}
    void setN1RangeMinimum(float val) {_n1RangeMinimum=val;}
    void setN1RangeMaximum(float val) {_n1RangeMaximum=val;}
    void setN1Color(QColor val) {_n1Color=val;}

    void setN2Enabled(bool val) {_n2Enabled=val;}
    void setN2DatarefMinimum(float val) {_n2RangeMinimum=val;}
    void setN2DatarefMaximum(float val) {_n2RangeMaximum=val;}
    void setN2RangeMinimum(float val) {_n2RangeMinimum=val;}
    void setN2RangeMaximum(float val) {_n2RangeMaximum=val;}
    void setN2Color(QColor val) {_n2Color=val;}

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

    void setFFEnabled(bool val) {_ffEnabled=val;}
    void setFFDatarefMinimum(float val) {_ffRangeMinimum=val;}
    void setFFDatarefMaximum(float val) {_ffRangeMaximum=val;}
    void setFFRangeMinimum(float val) {_ffRangeMinimum=val;}
    void setFFRangeMaximum(float val) {_ffRangeMaximum=val;}
    void setFFColor(QColor val) {_ffColor=val;}


protected:
    int _engineCount;
    int _barLabels;
    int _style; // 0=Generic 1=Boeing

    bool        _autoMin;
    bool        _autoMax;

    bool        _n1Enabled;
    QStringList _n1Values;
    double      _n1DatarefMinimum;
    double      _n1DatarefMaximum;
    double      _n1RangeMinimum;
    double      _n1RangeMaximum;
    QColor      _n1Color;

    bool        _n2Enabled;
    QStringList _n2Values;
    double      _n2DatarefMinimum;
    double      _n2DatarefMaximum;
    double      _n2RangeMinimum;
    double      _n2RangeMaximum;
    QColor      _n2Color;

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

    bool        _ffEnabled;
    QStringList _ffValues;
    double      _ffDatarefMinimum;
    double      _ffDatarefMaximum;
    double      _ffRangeMinimum;
    double      _ffRangeMaximum;
    QColor      _ffColor;
    
};

#endif // ENGINEDISPLAY_H
