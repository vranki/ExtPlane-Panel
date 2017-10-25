#include "gaugecover.h"

#include <QDebug>

#include "../util/perlinnoise.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(GaugeCover,"misc/cover/gauges");

GaugeCover::GaugeCover(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeCover, PanelItemShapeRectangular) {
    Q_UNUSED(conn);
    // Init
    _gaugesSize = 100;
    _gaugesInset = 20;
    _grainEnabled = false;
    _grainStrengh = 5;
    _grainZoom = 1;
    _grainPersistance = 50;
    _grainQuality = 6;
    _grainTiles = 1;
    _backgroundColor = QColor("#272727");
    _screwsEnabled = true;
    _screwsSize = 80;
    _screwsOffset = -6;
    setSize(400,400);
}

void GaugeCover::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);

    // Draw the cover image when not editing
    if(!isEditMode()) painter->drawImage(QPoint(0,0),_coverImage);

    PanelItem::paint(painter, option, widget);
}

void GaugeCover::itemSizeChanged(float w, float h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
    if(!isEditMode()) drawCoverImage();
    update();
}

void GaugeCover::setEditMode(bool em) {
    PanelItem::setEditMode(em);
    if(!isEditMode()) drawCoverImage();
    update();
}

void GaugeCover::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("gaugesSize", _gaugesSize);
    settings.setValue("gaugesInset", _gaugesInset);
    settings.setValue("backgroundColor", _backgroundColor.name());
    settings.setValue("grainEnabled", _grainEnabled);
    settings.setValue("grainStrengh", _grainStrengh);
    settings.setValue("grainPersistance", _grainPersistance);
    settings.setValue("grainQuality", _grainQuality);
    settings.setValue("grainZoom", _grainZoom);
    settings.setValue("grainTiles", _grainTiles);
    settings.setValue("screwsEnabled", _screwsEnabled);
    settings.setValue("screwsSize", _screwsSize);
    settings.setValue("screwsOffset", _screwsOffset);
}

void GaugeCover::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setGaugesSize(settings.value("gaugesSize", 100).toInt());
    setGaugesInset(settings.value("gaugesInset", 20).toInt());
    setBackgroundColor(QColor(settings.value("backgroundColor", "#272727").toString()));
    setGrainEnabled(settings.value("grainEnabled", false).toBool());
    setGrainStrength(settings.value("grainStrengh", 5).toInt());
    setGrainPersistance(settings.value("grainPersistance", 50).toInt());
    setGrainQuality(settings.value("grainQuality", 6).toInt());
    setGrainZoom(settings.value("grainZoom", 1).toInt());
    setGrainTiles(settings.value("grainTiles", 1).toInt());
    setScrewsEnabled(settings.value("screwsEnabled", true).toBool());
    setScrewsSize(settings.value("screwsSize", 80).toInt());
    setScrewsOffset(settings.value("screwsOffset", -6).toInt());
}

void GaugeCover::createSettings(QGridLayout *layout) {
    // General
    createSliderSetting(layout,"Gauge Size",1,200,_gaugesSize,SLOT(setGaugesSize(int)));
    createSliderSetting(layout,"Gauge Inset",0,100,_gaugesInset,SLOT(setGaugesInset(int)));
    createColorSetting(layout,"Cover Color",_backgroundColor,SLOT(setBackgroundColor(QColor)));

    // Grain
    createCheckboxSetting(layout,"Grain Enabled",_grainEnabled,SLOT(setGrainEnabled(bool)));
    createSliderSetting(layout,"Grain Strength",1,100,_grainStrengh,SLOT(setGrainStrength(int)));
    createSliderSetting(layout,"Grain Persistance",1,200,_grainPersistance,SLOT(setGrainPersistance(int)));
    createSliderSetting(layout,"Grain Quality",1,10,_grainQuality,SLOT(setGrainQuality(int)));
    createSliderSetting(layout,"Grain Smoothnes",1,200,_grainZoom,SLOT(setGrainZoom(int)));
    createSliderSetting(layout,"Grain Tiles",1,20,_grainTiles,SLOT(setGrainTiles(int)));

    // Screws
    createCheckboxSetting(layout,"Screws Enabled",_screwsEnabled,SLOT(setScrewsEnabled(bool)));
    createSliderSetting(layout,"Screws Size",1,200,_screwsSize,SLOT(setScrewsSize(int)));
    createSliderSetting(layout,"Screws Offset",-40,40,_screwsOffset,SLOT(setScrewsOffset(int)));

}

void GaugeCover::settingChanged() {
    if(!isEditMode()) drawCoverImage();
    update();
}

void GaugeCover::drawCoverImage() {

    // Init
    int w = width();
    int h = height();
    QImage newCover = QImage(w,h,QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&newCover);
    setupPainter(&painter);

    // Draw background
    painter.fillRect(0,0,w,h,_backgroundColor);
    if(_grainEnabled) {
        int tileSize = w/_grainTiles;
        QImage grainTile = QImage(tileSize,tileSize,QImage::Format_ARGB32);
        QPainter grainPainter;
        grainPainter.begin(&grainTile);
        setupPainter(&grainPainter);
        PerlinNoise::draw(&grainPainter,_grainQuality,_grainStrengh/100.0,_grainZoom,tileSize,tileSize);
        QBrush grainBrush = QBrush(grainTile);
        painter.setOpacity(_grainStrengh/100.0);
        painter.fillRect(QRect(0,0,w,h),grainBrush);
        painter.setOpacity(1.0);
    }

    // Find panel items to draw around
    for(int i = 0; i < panel()->items()->count(); i++) {
        PanelItem *item = panel()->items()->at(i);
        if(item->itemType() == PanelItemTypeGauge || item->itemType() == PanelItemTypeSwitch || item->itemType() == PanelItemTypeDisplay) {
            // Get logical coordinates
            int xx = item->x() - this->x();
            int yy = item->y() - this->y();
            int centerX = xx + item->width()/2;
            int centerY = yy + item->height()/2;
            // Draw fitting based on shape
            if(item->itemShape() == PanelItemShapeCircular) {
                int radius = qMin(item->width(),item->height());
                drawCoverImageCircularFitting(&painter,centerX,centerY,radius);
            } else if(item->itemShape() == PanelItemShapeRectangular) {
                drawCoverImageRectangularFitting(&painter,centerX,centerY,item->width(),item->height());
            }
        }
    }

    // Flip buffers
    painter.end();
    _coverImage = newCover;
}

void GaugeCover::drawCoverImageCircularFitting(QPainter *painter, int centerX, int centerY, int diameter) {

    painter->save(); {

        // Translage/scale/size
        int fullSize = diameter+_gaugesInset;
        int holeSize = diameter;
        painter->translate(centerX,centerY);
        painter->scale(_gaugesSize/100.0,_gaugesSize/100.0);

        // Draw fitting color
        QRadialGradient gradient(QPointF(0,0), fullSize, QPoint(-fullSize/2,-fullSize/2));
        gradient.setColorAt(0.0, _backgroundColor.lighter());
        gradient.setColorAt(0.4, _backgroundColor.darker());
        painter->setPen(Qt::NoPen);
        painter->setBrush(gradient);
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

    // Draw screws
    if(_screwsEnabled) {
        int radius = diameter/2;
        // Top left
        drawCoverImageScrew(painter,
                            centerX-radius-_screwsOffset,
                            centerY-radius-_screwsOffset);
        // Top right
        drawCoverImageScrew(painter,
                            centerX+radius+_screwsOffset,
                            centerY-radius-_screwsOffset);
        // Bottom left
        drawCoverImageScrew(painter,
                            centerX-radius-_screwsOffset,
                            centerY+radius+_screwsOffset);
        // Bottom right
        drawCoverImageScrew(painter,
                            centerX+radius+_screwsOffset,
                            centerY+radius+_screwsOffset);
    }
}

void GaugeCover::drawCoverImageRectangularFitting(QPainter *painter, int centerX, int centerY, int width, int height) {

    painter->save(); {

        // Translage/scale/size
        int fullWidth = width + _gaugesInset;
        int fullHeight = height + _gaugesInset;
        int size = qMax(fullWidth,fullHeight);
        qreal bevelY = 20;
        qreal bevelX = 20;
        qreal fittingBevel = 1.0;
        qreal holeBevel = 0.8;
        painter->translate(centerX,centerY);
        painter->scale(_gaugesSize/100.0,_gaugesSize/100.0);

        // Draw fitting color
        QRadialGradient gradient(QPointF(0,0), size, QPoint(-fullWidth/2,-fullHeight/2));
        gradient.setColorAt(0.0, _backgroundColor.lighter());
        gradient.setColorAt(0.4, _backgroundColor.darker());
        painter->setPen(Qt::NoPen);
        painter->setBrush(gradient);
        painter->setOpacity(0.5);
        painter->drawRoundedRect(
                    QRect(-fullWidth/2,-fullHeight/2,fullWidth,fullHeight),
                    bevelX*fittingBevel,
                    bevelY*fittingBevel,
                    Qt::AbsoluteSize
                 );
        painter->setOpacity(1.0);

        // Create the hole
        painter->setCompositionMode(QPainter::CompositionMode_Clear);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::black);
        painter->drawRoundedRect(
                    QRect(-width/2,-height/2,width,height),
                    bevelX*holeBevel,
                    bevelY*holeBevel,
                    Qt::AbsoluteSize
                 );
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    } painter->restore();
}

void GaugeCover::drawCoverImageScrew(QPainter *painter, int centerX, int centerY) {

    painter->save(); {

        // Translage/scale/size
        painter->translate(centerX,centerY);
        painter->scale(_screwsSize/100.0,_screwsSize/100.0);

        // Screw head
        int screwSize = 10;
        QRadialGradient gradient(QPointF(0,0), screwSize, QPoint(-screwSize/2,-screwSize/2));
        gradient.setColorAt(0.0, Qt::white);
        gradient.setColorAt(0.8, Qt::black);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(gradient));
        painter->drawEllipse(QPoint(0,0),screwSize,screwSize);

        // Engravement
        painter->rotate(M_PI*qrand());
        painter->setBrush(Qt::black);
        painter->drawEllipse(QPoint(0,0),screwSize,screwSize/4);
        painter->drawEllipse(QPoint(0,0),screwSize/4,screwSize);

    } painter->restore();
}
