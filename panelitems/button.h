#ifndef BUTTON_H
#define BUTTON_H

#include "panelitem.h"

class Button : public PanelItem {
    Q_OBJECT
public:
    Q_INVOKABLE Button(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:

public slots:
    void setButtonId(float id);
    void setLabel(QString txt);
    void setIsKey(bool is);
private:
    ExtPlaneClient _client;
    int _buttonId;
    QString _label;
    bool _isKey, _isHeld;
};

#endif // BUTTON_H
