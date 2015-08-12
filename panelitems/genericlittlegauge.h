#ifndef GENERICLITTLEGAUGE_H
#define GENERICLITTLEGAUGE_H

#include "panelitem.h"

class GenericLittleGauge : public PanelItem
{
public:
    Q_INVOKABLE GenericLittleGauge(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    ~GenericLittleGauge();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
    virtual void itemSizeChanged(float w, float h);

signals:
    public slots:
    void valueChanged(QString name, QStringList values);
    void valueChanged(QString name, double valueD);
    //dataref index values to manage if dataRefs is table
    void setIndexNumber(float val);
    //min, max, green arc, ...
    void setMaxValue(float mv);
    void setMinValue(float mv);
    void setGreenBeginValue(float mv);
    void setGreenEndValue(float mv);
    void setOrangeBeginValue(float mv);
    void setOrangeEndValue(float mv);
    void setRedBeginValue(float mv);
    void setRedEndValue(float mv);
    //strings to display on the gauge
    void setUpDesignation(QString s);
    void setDownDesignation(QString s);
    //manage datarefs
    void setIsTableValues(bool yes);
    void setDataRefString(QString s);
    void setAccuracy(float f);
    //manage graduations
    void setNbOfBigGraduations(float n);
    void setNbOfThinGraduations(float n);
    void setStringDisplayValue(QString value, unsigned int index);
    void setLogDisplayStyle(bool log);


private:
    Q_OBJECT
    void drawBottomPixmap();
    float value2Angle(const float &p);

    unsigned int _indexNumber;
    float value;
    float valueMin;
    float valueMax;
    float scaleFactor;
    float greenBeginValue;
    float greenEndValue;
    float orangeBeginValue;
    float orangeEndValue;
    float redBeginValue;
    float redEndValue;
    QString upDesignation;
    QString downDesignation;
    bool isTableValues;
    QString dataRefsString;
    float accuracy;
    int nbBigGraduations;
    int nbThinGraducations;
    QString stringDisplayValues[5];
    bool isLogDisplayStyle;

    ExtPlaneClient _client;
    const QImage bottomImage;
    QPixmap* bottomPixmap;
    QPixmap needleImage;


};

#endif // GENERICLITTLEGAUGE_H
