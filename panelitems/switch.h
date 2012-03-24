#ifndef SWITCH_H
#define SWITCH_H

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include "widgets/numberinputlineedit.h"
#include "extplaneclient.h"
#include "panelitem.h"

class Switch : public PanelItem {
    Q_OBJECT
public:
    Switch(QObject *parent, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    static QString typeNameStatic();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
    virtual void applySettings();
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:

public slots:
    void setLabel(QString txt);
    void setRef(QString ref);
private slots:
    void valueChanged(QString ref, double newValue);
private:
    ExtPlaneClient _client;
    int _buttonId;
    QString _label, _refname;
    bool _value;
    ClientDataRef* _ref;
    int switchWidth;
};

#endif // SWITCH_H
