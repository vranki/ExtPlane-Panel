#include "gaugecover.h"

#include <QtCore/qmath.h>

REGISTER_WITH_PANEL_ITEM_FACTORY(GaugeCover,"misc/cover/gauges");

GaugeCover::GaugeCover(QObject *parent, ExtPlaneConnection *conn) : PanelItem(parent) {
    // Init
    _gaugesH = 4;
    _gaugesV = 2;
    _gaugesSize = 100;
    _grainEnabled = true;
    _grainStrengh = 5;
    _grainZoom = 1;
    _grainPersistance = 50;
    _grainQuality = 6;
    _backgroundColor = "#BB9D7E";
    setSize(400,400);
}

void GaugeCover::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);

    painter->drawImage(QPoint(0,0),_coverImage);

    PanelItem::paint(painter, option, widget);
}

void GaugeCover::itemSizeChanged(float w, float h) {
    drawCoverImage();
    update();
}

void GaugeCover::setEditMode(bool em) {
    PanelItem::setEditMode(em);
    //drawCoverImage();
    //update();
}

void GaugeCover::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    //settings.setValue("label", _label);
}

void GaugeCover::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    //setButtonId(settings.value("id", 0).toInt());
}



void GaugeCover::createSettings(QGridLayout *layout) {
    createSliderSetting(layout,"Gauge Columns",1,10,_gaugesH,SLOT(setGaugesH(int)));
    createSliderSetting(layout,"Gauge Rows",1,10,_gaugesV,SLOT(setGaugesV(int)));
    createSliderSetting(layout,"Gauge Size",1,200,_gaugesSize,SLOT(setGaugesSize(int)));
    createLineEditSetting(layout,"Cover Color",_backgroundColor,SLOT(setBackgroundColor(QString)));
    createCheckboxSetting(layout,"Grain Enabled",_grainEnabled,SLOT(setGrainEnabled(bool)));
    createSliderSetting(layout,"Grain Strength",1,100,_grainStrengh,SLOT(setGrainStrength(int)));
    createSliderSetting(layout,"Grain Persistance",1,200,_grainPersistance,SLOT(setGrainPersistance(int)));
    createSliderSetting(layout,"Grain Quality",1,10,_grainQuality,SLOT(setGrainQuality(int)));
    createSliderSetting(layout,"Grain Roughness",1,200,_grainZoom,SLOT(setGrainZoom(int)));

}

void GaugeCover::settingChanged() {
    drawCoverImage();
    update();
}


inline double findnoise2(double x,double y)
{
 int n=(int)x+(int)y*57;
 n=(n<<13)^n;
 int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
 return 1.0-((double)nn/1073741824.0);
}


inline double interpolate1(double a,double b,double x)
{
 double ft=x * 3.1415927;
 double f=(1.0-cos(ft))* 0.5;
 return a*(1.0-f)+b*f;
}



double noise2(double x,double y)
{
 double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
 double floory=(double)((int)y);
 double s,t,u,v;//Integer declaration
 s=findnoise2(floorx,floory);
 t=findnoise2(floorx+1,floory);
 u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
 v=findnoise2(floorx+1,floory+1);
 double int1=interpolate1(s,t,x-floorx);//Interpolate between the values.
 double int2=interpolate1(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
 return interpolate1(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}





//performs perlin noise function
void drawPerlinNoise(QPainter *painter, int octaves, double p, double zoom, int width, int height) {
    //long seed = 38383;
    int w = width;
    int h = height;




    for(int y=0;y<h;y++)
    {//Loops to loop trough all the pixels
     for(int x=0;x<w;x++)
     {
      double getnoise =0;
      for(int a=0;a<octaves-1;a++)//This loops trough the octaves.
      {
       double frequency = pow(2,a);//This increases the frequency with every loop of the octave.
       double amplitude = pow(p,a);//This decreases the amplitude with every loop of the octave.
       getnoise += noise2(((double)x)*frequency/zoom,((double)y)/zoom*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
      }//											It gives a decimal value, you know, between the pixels. Like 4.2 or 5.1
      int color= (int)((getnoise*128.0)+128.0); //Convert to 0-256 values.
      if(color>255) color=255;
      if(color<0) color=0;
      painter->setPen(QColor(color,color,color));
      painter->drawPoint(x,y);
     }
    }

}


void GaugeCover::drawCoverImage() {
    qDebug() << Q_FUNC_INFO << width() << height() << _gaugesH << _gaugesV;

    // Init
    int w = width();
    int h = height();
    QImage newCover = QImage(w,h,QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&newCover);
    setupPainter(&painter);

    // Draw background
    painter.fillRect(0,0,w,h,QColor(_backgroundColor));
    painter.setOpacity(_grainStrengh/100.0);
    if(_grainEnabled) drawPerlinNoise(&painter,_grainQuality,_grainStrengh/100.0,_grainZoom,w,h);
    painter.setOpacity(1.0);

    // Draw gauge fittings
    int gaugeSlotWidth = w / _gaugesH;
    int gaugeSlotHeight = h / _gaugesV;
    for(int y = 0; y < _gaugesV; y++) {
        for(int x = 0; x < _gaugesH; x++) {
            // Get translated coords
            int xx = x * gaugeSlotWidth + (gaugeSlotWidth/2);
            int yy = y * gaugeSlotHeight + (gaugeSlotHeight/2);
            drawCoverImageGuageFitting(&painter,xx,yy,200);
        }
    }


    // Flip buffers
    painter.end();
    _coverImage = newCover;
}

void GaugeCover::drawCoverImageGuageFitting(QPainter *painter, int centerX, int centerY, int size) {

    painter->save(); {

        // Translage/scale/size
        int fullSize = size;
        int holeSize = fullSize-20;
        painter->translate(centerX,centerY);
        painter->scale(_gaugesSize/100.0,_gaugesSize/100.0);

        // Draw fitting color
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(_backgroundColor).darker());
        painter->setOpacity(0.5);
        painter->drawEllipse(-fullSize/2,-fullSize/2,fullSize,fullSize);
        painter->setOpacity(1.0);

        // Create the hole
        painter->setCompositionMode(QPainter::CompositionMode_Clear);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::black);
        painter->drawEllipse(-holeSize/2,-holeSize/2,holeSize,holeSize);
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    } painter->restore();
}

void GaugeCover::drawCoverImageLightFitting(QPainter *painter, int centerX, int centerY, int width, int height) {

    painter->save(); {

        // Translage/scale/size
        int fullSize = 200;
        int holeSize = fullSize-20;
        painter->translate(centerX,centerY);

        // Draw fitting color
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(_backgroundColor).darker());
        painter->setOpacity(0.5);
        painter->drawEllipse(-fullSize/2,-fullSize/2,fullSize,fullSize);
        painter->setOpacity(1.0);

        // Create the hole
        painter->setCompositionMode(QPainter::CompositionMode_Clear);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::black);
        painter->drawEllipse(-holeSize/2,-holeSize/2,holeSize,holeSize);
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    } painter->restore();
}
