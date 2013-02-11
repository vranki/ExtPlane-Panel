/*
 *  HSI_KS55.h
 *  extplane-panel
 *
 *  Created by bobgates on 2011/08/01.
 *  Copyright 2011 DaffeySoft. All rights reserved.
 *
 */

#ifndef HSI_H
#define HSI_H

#include <QLabel>
#include "../widgets/distanceunitcombobox.h"
#include "../extplaneclient.h"
#include "panelitem.h"
#include "../units.h"
#include "widgets/numberinputlineedit.h"
#include <QHash>


enum DataRefType {
    drFloat, drInt, drChar
};

struct DataRefStruct {
    QString         name;
    float           tolerance;
    DataRefType     dataType;
    void *          value;
};

class HSI : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE HSI(QObject *parent, ExtPlaneConnection *conn);
    virtual void createSettings(QGridLayout *layout);
    virtual void loadSettings(QSettings &settings);
    virtual void storeSettings(QSettings &settings);

    virtual QString typeName();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void createCard(void);
    
    void setLabel(QString text);
    
    
    
public slots:
    void dataRefsChanged(QString name, QString valueString);
    void refChanged(QString name, double alt);
        
private:
    ExtPlaneClient _client;
    bool    _pilotSide;
    float   value2Angle(float value);
    
    float   _heading;
    float   _bug;
    float   _course;
    float   _dme;
    float   _dmeTime;
    float   _hdots;
    float   _vdots;
    float    _hasVertical;      // All float because the import mechanism
    float    _hasHorizontal;    // expects floats.
    float    _hasDME;
    float    _glideslopeFlag;
    float    _toFrom;  // 0 = Flag, 1=To, 2=From
    
    QString _label;
    QString _dataRef;
    QPixmap _card;
    QPixmap _bezel;
    
    QList <DataRefStruct *> _dataRefs;
        QHash <QString, DataRefStruct> _dataRefLookup;
    //QHash <QString, void * > _dataRefLookup;

    float   _thickBars;
    float   _thinBars;
    float   _numbers;
};

#endif // HSI_H


/*
 DataRefs for HSI:

 hsi_bearing_deg_mag_pilot          float	930+	no	degrees_magnetic	Indicated relative bearing to the pilot's HSI-selected navaid
 hsi_relative_bearing_deg_pilot     float	930+	no	degrees             Indicated relative bearing to the pilot's HSI-selected navaid
 hsi_flag_from_to_pilot             int     930+	no	enum                Nav-To-From indication, nav1, pilot, 0 is flag, 1 is to, 2 is from.
 hsi_hdef_dots_pilot                float	930+	no	dots                CDI lateral deflection in dots, nav1, pilot
 hsi_vdef_dots_pilot                float	930+	no	dots                CDI vertical deflection in dots, nav1, pilot
 hsi_has_dme_pilot                  int     930+	no	boolean             Is there a DME signal from nav1's DME? 1 if yes, 0 if no
 hsi_dme_distance_nm_pilot          float	930+	no	nautical_miles      nav1 DME distance in nautical miles, pilot HSI
 hsi_dme_speed_kts_pilot            float	930+	no	knots               nav1 DME speed in knots. pilot HSI
 hsi_dme_time_min_pilot             float	930+	no	minutes             nav1 DME time in minutes. pilot HSI
 hsi_flag_glideslope_pilot          int     940+	no	boolean             Glide slope flag, pilot side HSI
 hsi_display_horizontal_pilot       int     940+	no	boolean             Is there some kind of horizontal signal on pilot side HSI
 hsi_display_vertical_pilot         int     940+	no	boolean             Is there some kind of vertical signal on pilot side HSI

 hsi_bearing_deg_mag_copilot        float	930+	no	degrees_magnetic	Indicated relative bearing to the copilot's HSI-selected navaid
 hsi_relative_bearing_deg_copilot	float	930+	no	degrees	Indicated relative bearing to the copilot's HSI-selected navaid
 hsi_flag_from_to_copilot           int     930+	no	enum	Nav-To-From indication, nav1, copilot, 0 is flag, 1 is to, 2 is from.
 hsi_hdef_dots_copilot              float	930+	no	dots	CDI lateral deflection in dots, nav1, copilot
 hsi_vdef_dots_copilot              float	930+	no	dots	CDI vertical deflection in dots, nav1, copilot
 hsi_has_dme_copilot                int     930+	no	boolean	Is there a DME signal from nav1's DME? 1 if yes, 0 if no
 hsi_dme_distance_nm_copilot        float	930+	no	nautical_miles	nav1 DME distance in nautical miles. copilot HSI
 hsi_dme_speed_kts_copilot          float	930+	no	knots	nav1 DME speed in knots. copilot HSI
 hsi_dme_time_min_copilot           float	930+	no	minutes	nav1 DME time in minutes. copilot HSI
 hsi_flag_glideslope_copilot        int     940+	no	boolean	Glide slope flag, copilot side HSI
 hsi_display_horizontal_copilot     int     940+	no	boolean	Is there some kind of horizontal signal on copilot side HSI
 hsi_display_vertical_copilot       int     940+	no	boolean	Is there some kind of vertical signal on copilot side HSI
*/
