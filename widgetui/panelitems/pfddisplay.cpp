#include "pfddisplay.h"

#include <QLabel>
#include <QCheckBox>

#include "../util/console.h"

#define _USE_MATH_DEFINES
#include <math.h>

REGISTER_WITH_PANEL_ITEM_FACTORY(PFDDisplay,"display/pfd")

#define DATAREF_PITCH "sim/cockpit2/gauges/indicators/pitch_vacuum_deg_pilot"
#define DATAREF_ROLL "sim/cockpit2/gauges/indicators/roll_electric_deg_pilot"
#define DATAREF_SLIP "sim/cockpit2/gauges/indicators/slip_deg"
#define DATAREF_AIRSPEED_KTS "sim/cockpit2/gauges/indicators/airspeed_kts_pilot"
#define DATAREF_ALTITUDE_FT "sim/cockpit2/gauges/indicators/altitude_ft_pilot"
#define DATAREF_AIRSPEED_ACC_KTS "sim/cockpit2/gauges/indicators/airspeed_acceleration_kts_sec_pilot"
#define DATAREF_HEADING_DEG "sim/cockpit2/gauges/indicators/heading_electric_deg_mag_pilot"
#define DATAREF_VERTICALSPEED_FPM "sim/cockpit2/gauges/indicators/vvi_fpm_pilot"

#define ENGINE_STYLE_GENERIC 0
#define ENGINE_STYLE_BOEING 1

#define SCALE_INDICATOR_TYPE_AIRSPEED 1
#define SCALE_INDICATOR_TYPE_ALTITUDE 2

PFDDisplay::PFDDisplay(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        DisplayInstrument(panel,conn) {
    // Init
    _style = ENGINE_STYLE_BOEING;
    _strokeSize = 2;
    _colorGround = QColor(164,147,108);
    _colorSky = QColor(0,128,255);
    _colorScale = QColor(128,128,128);
    _colorStroke = QColor(255,255,255);
    _colorWhisker = QColor(0,0,0);
    _colorValue = QColor(255,0,255);

    // Init attitude
    _attitude_pitchValue = 10;
    _attitude_rollValue = -20;

    // Connect
    _client.subscribeDataRef(DATAREF_PITCH,0.05);
    _client.subscribeDataRef(DATAREF_ROLL,0.05);
    _client.subscribeDataRef(DATAREF_SLIP,0.05);
    _client.subscribeDataRef(DATAREF_AIRSPEED_KTS,0.5);
    _client.subscribeDataRef(DATAREF_ALTITUDE_FT,0.5);
    _client.subscribeDataRef(DATAREF_AIRSPEED_ACC_KTS,0.5);
    _client.subscribeDataRef(DATAREF_HEADING_DEG,0.5);
    _client.subscribeDataRef(DATAREF_VERTICALSPEED_FPM,0.05);
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(refChanged(QString,QStringList)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));

}

void PFDDisplay::refChanged(QString name, double value) {
    if (name == DATAREF_ROLL) {
        _attitude_rollValue = value;
    } else if (name == DATAREF_PITCH) {
        _attitude_pitchValue = value;
    } else if (name == DATAREF_SLIP) {
        _attitude_slipValue = value;
    } else if (name == DATAREF_AIRSPEED_KTS) {
        _airspeed_value = value;
    } else if (name == DATAREF_ALTITUDE_FT) {
        _altitude_value = value;
    } else if (name == DATAREF_AIRSPEED_ACC_KTS) {
        _airspeedaccl_value = value;
    } else if (name == DATAREF_HEADING_DEG) {
        _compass_heading_value =  value;
    } else if (name == DATAREF_VERTICALSPEED_FPM) {
        _verticalspeed_value =  value;
    }
}

void PFDDisplay::refChanged(QString name, QStringList values) {
    Q_UNUSED(name);
    Q_UNUSED(values);
}

void PFDDisplay::itemSizeChanged(float w, float h) {
    DisplayInstrument::itemSizeChanged(w,h);
    // Sizes
    _attitude_size = qMin(w,h)*0.6;
    _scale_height = h*0.8;
    _scale_width = _attitude_size*0.19;
    _compass_size = _attitude_size*1.4;
    _verticalspeed_size = _scale_width*0.8;
    // Pens
    _defaultPen = QPen(_colorStroke,_strokeSize, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    _defaultPen2x = QPen(_colorStroke,_strokeSize*2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    // Fonts
    {
        _tickFont = this->defaultFont;
        _tickFont.setBold(true);
        _tickFont.setPixelSize(w*0.035);
    }
    {
        _valueFont = this->defaultFont;
        _valueFont.setBold(true);
        _valueFont.setPixelSize(w*0.06);
    }
    // Cached pixmaps
    createCompassBackplate(_compass_size,_compass_size);
}

void PFDDisplay::render(QPainter *painter, int width, int height) {

    painter->save(); {

        // Painter init
        painter->setFont(_tickFont);

        // Draw attitude
        int attitudeOffsetX = -_attitude_size*0.1;
        drawAttitudeIndicator(painter,width/2+attitudeOffsetX,height/2,_attitude_size,_attitude_size);

        // Draw vertical speed
        float verticalSpeedHeight = _scale_height*0.6;
        drawVerticalSpeed(painter,width,height,width-_verticalspeed_size,height/2-verticalSpeedHeight/2,_verticalspeed_size,verticalSpeedHeight);

        // Draw airspeed scale
        int scaleHeightInt = (int)_scale_height;
        if (scaleHeightInt % 2 != 0 ) scaleHeightInt++;
        drawScaleIndicator(painter,width,height,0,height/2-scaleHeightInt/2,_scale_width,scaleHeightInt,_airspeed_value,SCALE_INDICATOR_TYPE_AIRSPEED);

        // Draw altitude scale
        drawScaleIndicator(painter,width,height,width-_scale_width+attitudeOffsetX*2,height/2-scaleHeightInt/2,_scale_width,scaleHeightInt,_altitude_value,SCALE_INDICATOR_TYPE_ALTITUDE);

        // Draw compass
        drawCompass(painter,width,height,width/2+attitudeOffsetX,height/2+_attitude_size*1.3,_compass_size,_compass_size);

    } painter->restore();

}

void PFDDisplay::drawAttitudeIndicator(QPainter *painter, int attitudeX, int attitudeY, int attitudeWidth, int attitudeHeight) {
    painter->save(); {
        // Init
        double pitch = _attitude_pitchValue;
        double pixelsPerPitchDeg = (_attitude_size*0.02f);
        //pitch = 10;
        //if (pitch > maxPitch) pitch = maxPitch;
        //if (pitch < (0-maxPitch)) pitch = (0-maxPitch);

        // Move to center
        painter->translate(attitudeX,attitudeY);

        // Make sure to clip the widget because of moving card
        QPainterPath clipPath;
        clipPath.setFillRule(Qt::WindingFill);
        clipPath.addRoundedRect(QRect(-attitudeWidth/2,-attitudeHeight/2,attitudeWidth,attitudeHeight), _attitude_size*0.1, _attitude_size*0.1 );
        painter->setClipPath(clipPath);

        // Draw background (sky)
        painter->fillRect(-attitudeWidth/2,-attitudeHeight/2,attitudeWidth,attitudeHeight,_colorSky);

        // Rotate to roll and draw moving card
        //TODO: cache this as a pixmap?
        painter->save(); {
            double pitchPixelsY = pitch*pixelsPerPitchDeg;
            //DEBUG << pitch;
            painter->rotate(-_attitude_rollValue);
            painter->translate(0,pitchPixelsY);

            // Draw ground
            int groundWidth = attitudeWidth*2;
            int groundHeight = attitudeHeight*2;
            painter->fillRect(-groundWidth/2,0,groundWidth,groundHeight,_colorGround);

            // Draw ground line
            painter->fillRect(-groundWidth/2,-1,groundWidth,2,_colorStroke);

            // Draw pitch markers
            int degreesPerMarker = 10;
            for(int p = -90; p < 90; p += degreesPerMarker) {
                // Init
                int markerWidth = attitudeWidth*0.3;
                int middleMarkerWidth = markerWidth*0.4;
                int smallMarkerWidth = middleMarkerWidth*0.4;
                int markerHeight = _strokeSize;
                int pOffset = -p*pixelsPerPitchDeg;
                int markerY = -markerHeight/2+pOffset;
                // Main Marker
                painter->fillRect(-markerWidth/2,
                                  markerY,
                                  markerWidth,
                                  markerHeight,
                                  _colorStroke);
                // Middle Marker
                painter->fillRect(-middleMarkerWidth/2,
                                  markerY + (degreesPerMarker/2.0)*pixelsPerPitchDeg,
                                  middleMarkerWidth,
                                  markerHeight,
                                  _colorStroke);
                // Small Marker
                painter->fillRect(-smallMarkerWidth/2,
                                  markerY + (degreesPerMarker/4.0)*pixelsPerPitchDeg,
                                  smallMarkerWidth,
                                  markerHeight,
                                  _colorStroke);
                painter->fillRect(-smallMarkerWidth/2,
                                  markerY + (degreesPerMarker*0.75)*pixelsPerPitchDeg,
                                  smallMarkerWidth,
                                  markerHeight,
                                  _colorStroke);
                // Text
                int textWidth = markerWidth*0.3;
                int textHeight = textWidth/2;
                if(p % 10 == 0 && p != 0) {
                    painter->setPen(_defaultPen);
                    //QRect r1(markerWidth/2,-textHeight/2+pOffset,markerWidth+textWidth,textHeight);
                    QRect r(-markerWidth/2-textWidth,-textHeight/2+pOffset,markerWidth+textWidth+textWidth,textHeight);
                    painter->drawText(r,Qt::AlignVCenter|Qt::AlignLeft,QString("%1").arg(abs(p)),NULL);
                    painter->drawText(r,Qt::AlignVCenter|Qt::AlignRight,QString("%1").arg(abs(p)),NULL);
                }
            }

        } painter->restore();

        // Draw static attitude markers for roll
        int tickRadius = attitudeHeight/2;
        int tickSize = attitudeHeight*0.08;
        int triangleSize = tickSize/2;
        {
            // Line markings
            painter->setPen(_defaultPen);
            double r = tickRadius;
            double s = tickSize;
            double l1 = r-s;
            double l2l = r;
            double l2s = r-(s/2);
            // Short inner
            for(int i=70; i<=110; i+=10){
                if(i == 90) continue;
                painter->drawLine(l1*cos(float(i)/180.*M_PI),-l1*sin(float(i)/180.*M_PI),
                                  l2s*cos(float(i)/180.*M_PI),-l2s*sin(float(i)/180.*M_PI));
            }
            // Short outer at 45deg
            {
                int i=45;
                painter->drawLine(l1*cos(float(i)/180.*M_PI),-l1*sin(float(i)/180.*M_PI),
                                  l2s*cos(float(i)/180.*M_PI),-l2s*sin(float(i)/180.*M_PI));
            }
            {
                int i=90+45;
                painter->drawLine(l1*cos(float(i)/180.*M_PI),-l1*sin(float(i)/180.*M_PI),
                                  l2s*cos(float(i)/180.*M_PI),-l2s*sin(float(i)/180.*M_PI));
            }
            // Long
            for(int i=30; i<=180-30; i+=30){
                if(i == 90) continue;
                painter->drawLine(l1*cos(float(i)/180.*M_PI),-l1*sin(float(i)/180.*M_PI),
                                  l2l*cos(float(i)/180.*M_PI),-l2l*sin(float(i)/180.*M_PI));
            }
            // Fixed triangle at 0deg
            QPainterPath path;
            path.moveTo(-triangleSize,-(r-triangleSize));
            path.lineTo(+triangleSize,-(r-triangleSize));
            path.lineTo(0,-(r-triangleSize*2));
            painter->fillPath(path,_colorStroke);
        }

        // Draw roll marker
        {
            // Rotate
            painter->rotate(-_attitude_rollValue);
            // Fixed triangle at 0deg
            QPainterPath path;
            int r = tickRadius-triangleSize;
            int rollMarkerSize = triangleSize*1.3;
            path.moveTo(-rollMarkerSize,-(r-rollMarkerSize*2));
            path.lineTo(+rollMarkerSize,-(r-rollMarkerSize*2));
            path.lineTo(0,-(r-rollMarkerSize));
            painter->fillPath(path,_colorStroke);
            // Un-Rotate
            painter->rotate(+_attitude_rollValue);
            // Rotate
            painter->rotate(-_attitude_slipValue);
            painter->fillRect(QRect(-rollMarkerSize,-r+rollMarkerSize*2+_strokeSize,rollMarkerSize*2,_strokeSize),_colorStroke);
            painter->rotate(+_attitude_slipValue);
        }

        // Draw wiskers
        painter->save(); {
            // Init
            painter->setBrush(_colorWhisker);
            painter->setPen(QPen(_colorStroke,_strokeSize/2));
            int w = attitudeWidth*0.3;
            int h = w*0.1;
            int pad = w*0.18;
            QPainterPath path;
            path.moveTo(0,0);
            path.lineTo(w,0);
            path.lineTo(w,h*2);
            path.lineTo(w-h,h*2);
            path.lineTo(w-h,h);
            path.lineTo(0,h);
            path.lineTo(0,0);
            // Left
            painter->translate(-(w+pad),-h/2);
            painter->drawPath(path);
            // Right
            painter->scale(-1,1);
            painter->translate(-(w+pad)*2,0);
            painter->drawPath(path);
        } painter->restore();


    } painter->restore();
}

int roundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = abs(numToRound) % multiple;
    if (remainder == 0)
        return numToRound;

    if (numToRound < 0)
        return -(abs(numToRound) - remainder);
    else
        return numToRound + multiple - remainder;
}

void PFDDisplay::drawScaleIndicator(QPainter *painter, int screenWidth, int screenHeight, int x, int y, int w, int h, double value, int type) {
    Q_UNUSED(screenWidth);
    Q_UNUSED(screenHeight);
    painter->save(); {
        // Init
        int intValue = (int)value;
        QRect mainRect = QRect(0,0,w,h);

        // Setup for type (default = airspeed)
        double ticks = 13;
        double valuePerTick = 10;
        int textEveryXValue = 20;
        if(type == SCALE_INDICATOR_TYPE_ALTITUDE) {
            ticks = 9;
            valuePerTick = 100;
            textEveryXValue = 200;
        }
        double pixelsPerTick = h/ticks;
        double pixelsPerValue = h/(ticks*valuePerTick);

        // Move to position
        painter->translate(x,y);

        // Draw background
        painter->fillRect(mainRect,Qt::gray);

        // Move to center
        painter->translate(0,h/2);

        // Get tick offset
        // We need both a int version and a back-calculated float version
        // The int is used for drawing labels, the float version for drawing
        int valueOffset = intValue % (int)valuePerTick;
        double valueOffsetFloat = valueOffset + value - intValue;
        double pixelOffsetFloat = valueOffsetFloat * pixelsPerValue;

        // Draw ticks
        painter->setClipRect(QRect(0,-h/2,w,h));
        painter->setPen(_defaultPen);
        int tickWidth = w*0.16;
        for(int t = -ticks/2-1; t < ticks/2+1; t++) {
            // Get value and pixel
            // Here we need
            int tv = (value-valueOffset) + t*valuePerTick*-1;
            tv = roundUp(tv+1,valuePerTick)-valuePerTick;
            int tpy = t*pixelsPerTick+pixelOffsetFloat;
            // Draw tick
            if(type == SCALE_INDICATOR_TYPE_ALTITUDE) {
                painter->fillRect(0,tpy,tickWidth,_strokeSize,_colorStroke);
            } else {
                painter->fillRect(w-tickWidth,tpy,tickWidth,_strokeSize,_colorStroke);
            }
            // Draw text
            if(tv % textEveryXValue==0) {
                if(type == SCALE_INDICATOR_TYPE_ALTITUDE) {
                    int textWidth = w;
                    int textHeight = w;
                    painter->drawText(QRect(0,tpy-textHeight/2,textWidth*0.95,textHeight),
                                      Qt::AlignVCenter|Qt::AlignRight,
                                      QString("%1").arg(tv),NULL);
                } else {
                    int textWidth = w-tickWidth-tickWidth*0.2;
                    int textHeight = w;
                    painter->drawText(QRect(0,tpy-textHeight/2,textWidth,textHeight),
                                      Qt::AlignVCenter|Qt::AlignRight,
                                      QString("%1").arg(tv),NULL);
                }
            }
        }
        painter->setClipping(false);

        // Draw value box
        {
            // Bounding box
            int valueHeight = w*0.7;
            int valueWidth = valueHeight*0.9;
            int xOffset = 0;
            if(type == SCALE_INDICATOR_TYPE_ALTITUDE) {
                xOffset = -valueHeight*0.5;
                valueWidth = valueHeight*1.1;
            }
            QPainterPath path;
            path.moveTo(xOffset+0,-valueHeight/2);
            path.lineTo(valueHeight,-valueHeight/2);
            path.lineTo(valueHeight,-valueHeight/4);
            path.lineTo(valueHeight+valueHeight/4,0);
            path.lineTo(valueHeight,+valueHeight/4);
            path.lineTo(valueHeight,+valueHeight/2);
            path.lineTo(xOffset+0,+valueHeight/2);
            path.lineTo(xOffset+0,-valueHeight/2);
            painter->setPen(_defaultPen);
            painter->setBrush(Qt::black);
            if(type == SCALE_INDICATOR_TYPE_ALTITUDE) {
                painter->save();
                painter->translate(valueHeight*1.5,0);
                painter->scale(-1,1);
                painter->drawPath(path);
                painter->restore();
            } else {
                painter->drawPath(path);
            }
            // Draw text
            QRect r(-xOffset,-valueHeight/2,valueWidth,valueHeight);
            painter->drawText(r,Qt::AlignVCenter|Qt::AlignRight,QString("%1").arg(intValue),NULL);
        }

        // Draw value
        {
            int valueWidth = w;
            int valueHeight = valueWidth*0.3;
            QRect r(0,-h/2-valueHeight*1.3,valueWidth,valueHeight);
            painter->setPen(_colorValue);
            painter->drawText(r,Qt::AlignVCenter|Qt::AlignCenter,QString("%1").arg(intValue),NULL);
        }

        // Draw speed trend arrow
        if(type == SCALE_INDICATOR_TYPE_AIRSPEED) {
            // Get the height
            float acclH = _airspeedaccl_value*pixelsPerValue*-1;
            float acclX = w*0.86;
            // Draw line from edge of center tick
            painter->setPen(_defaultPen);
            painter->drawLine(acclX,0,acclX,acclH);
            // Draw bottom triangle
            if(_airspeedaccl_value > 2 || _airspeedaccl_value < -2) {
                int arrowDir = 1;
                float arrowSize = _strokeSize*2;
                if(_airspeedaccl_value > 0) arrowDir = -1;
                QPainterPath path;
                path.moveTo(acclX-_strokeSize/2,acclH+arrowSize*2*arrowDir);
                path.lineTo(acclX-arrowSize-_strokeSize/2,acclH);
                path.lineTo(acclX+arrowSize-_strokeSize/2,acclH);
                painter->fillPath(path,_colorStroke);
            }
        }

    } painter->restore();
}

void PFDDisplay::createCompassBackplate(int w, int h) {

    // Cached version - improves performance and allow smoother rending of angled texts

    // Create image
    QImage image = QImage(QSize(w,h), QImage::Format_ARGB32);
    image.fill(0x00ffffff);

    // Init painter
    QPainter painter;
    painter.begin(&image);
    setupPainter(&painter);
    painter.setFont(_tickFont);

    // Translate to center
    painter.translate(w/2,h/2);

    // Draw backplate
    painter.setPen(Qt::transparent);
    painter.setBrush(_colorScale);
    painter.drawEllipse(QPoint(0,0),w/2,w/2);

    // Rotate to heading
    //painter->rotate(_compass_heading_value);

    // Draw ticks
    painter.setPen(_defaultPen);
    int degreesPerTick = 5;
    float r = w/2.0;
    float rtL = r - r*0.1; // Long tick
    float rtS = r - r*0.05; // Short tick
    float textWidth = w*0.1;
    for(int i=0; i<360; i+=degreesPerTick){
        // Rotate
        painter.rotate(i);
        float rt = rtS;
        // Text
        if(i % (degreesPerTick*2) == 0) {
            rt = rtL;
            int displayValue = i / 10;
            QRect r(-textWidth/2,-rtL,textWidth,textWidth/2);
            painter.drawText(r,Qt::AlignVCenter|Qt::AlignCenter,QString("%1").arg(displayValue),NULL);
        }
        // Tick
        painter.drawLine(0,rt,0,r);
        // Undo Rotate
        painter.rotate(-i);
    }


    // Save as pixmap
    painter.end();
    _compass_backplate = QPixmap::fromImage(image, Qt::AutoColor);


}

void PFDDisplay::drawCompass(QPainter *painter, int screenWidth, int screenHeight, int x, int y, int w, int h) {
    Q_UNUSED(screenWidth);
    Q_UNUSED(screenHeight);
    painter->save(); {

        // Move to center and rotate
        painter->translate(x,y);
        painter->rotate(-_compass_heading_value);

        // Draw backplate
        painter->drawPixmap(-_compass_size/2,-_compass_size/2,_compass_backplate);

        // Undo rotate
        painter->rotate(+_compass_heading_value);

        // Draw center line
        float r = w/2.0;
        painter->setPen(QPen(_colorStroke,_strokeSize*1.4,Qt::DotLine));
        painter->drawLine(0,0,0,-r);

        // Draw triangle at top
        float triangleSize = _compass_size*0.02;
        QPainterPath path;
        path.moveTo(0,-r);
        path.lineTo(-triangleSize,-r-triangleSize*1.6);
        path.lineTo(+triangleSize,-r-triangleSize*1.6);
        path.lineTo(0,-r);
        painter->setPen(_defaultPen);
        painter->drawPath(path);

        // Draw value
        {
            painter->translate(-x,-y);
            int intValue = (int)_compass_heading_value;
            int valueWidth = h*0.18;
            int valueHeight = valueWidth*0.3;
            QRect r(x-valueWidth,screenHeight-valueHeight*1.2,valueWidth,valueHeight);
            painter->setPen(_colorValue);
            painter->drawText(r,Qt::AlignVCenter|Qt::AlignCenter,QString("%1H").arg(intValue),NULL);
        }

    } painter->restore();
}

void PFDDisplay::drawVerticalSpeed(QPainter *painter, int screenWidth, int screenHeight, int x, int y, int w, int h) {
    Q_UNUSED(screenWidth);
    Q_UNUSED(screenHeight);
    painter->save(); {

        // Vertical speed is in feet per minute, usually every tick @ 500
        //TODO: Implement that top and bottom tick on a different scale
        //TODO: Use a simulated needle instead (like on older boings)

        // Init
        double ticks = 6;
        int valuePerTick = 500;
        int textEveryXValue = 1000;
        int tickValueDivisor = 1000;
        double pixelsPerTick = h/(ticks*2);
        double pixelsPerValue = h/((ticks*2)*valuePerTick);
        double tickWidth = w*0.4;
        double textWidth = w-tickWidth;
        double textHeight = textWidth/2;

        // Move to position
        painter->translate(x,y);

        // Draw background
        int padding = textHeight;
        QRect mainRect = QRect(0,0-padding,w,h+padding+padding);
        painter->fillRect(mainRect,Qt::gray);

        // Move to center
        painter->translate(0,h/2);

        // Draw ticks
        painter->setPen(_defaultPen);
        for(int t = -ticks; t <= ticks; t++) {
            // Draw text and tick
            double tickY = t * pixelsPerTick;
            double tickS = tickWidth * 0.5;
            int tickValue = abs(t * valuePerTick);
            int displayValue = tickValue / tickValueDivisor;
            if(tickValue % textEveryXValue == 0) {
                tickS = tickWidth;
                QRect r(tickWidth*1.1,tickY-textHeight/2,textWidth,textHeight);
                if(displayValue != 0) painter->drawText(r,Qt::AlignVCenter|Qt::AlignLeft,QString("%1").arg(displayValue),NULL);
            }
            painter->drawLine(0,tickY,tickS,tickY);
        }

        // Draw value line
        double valueY = _verticalspeed_value * pixelsPerValue * -1;
        if(valueY < -h/2) valueY = -h/2;
        else if(valueY > h/2) valueY = h/2;
        painter->setPen(_defaultPen2x);
        painter->drawLine(w/2,valueY,w,valueY);

    } painter->restore();
}


void PFDDisplay::storeSettings(QSettings &settings) {
    DisplayInstrument::storeSettings(settings);

    settings.setValue("style", _style);


}

void PFDDisplay::loadSettings(QSettings &settings) {
    DisplayInstrument::loadSettings(settings);

    setStyle(settings.value("style","0").toInt());

}

void PFDDisplay::createSettings(QGridLayout *layout) {
    DisplayInstrument::createSettings(layout);

    QStringList styles;
    styles << "Generic" << "Boing";
    createComboBoxSetting(layout,"Style",_style,styles,SLOT(setStyle(int)));
    //createSliderSetting(layout,"Number of Labels",0,11,_barLabels,SLOT(setBarLabels(int)));
    //createCheckboxSetting(layout,"Auto Min",_autoMin,SLOT(setAutoMin(bool)));
    //createCheckboxSetting(layout,"Auto Max",_autoMax,SLOT(setAutoMax(bool)));


}

