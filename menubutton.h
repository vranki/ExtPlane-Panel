#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QGraphicsItem>
class PanelItem;
class SettingsDialog;

/**
 * The main menu button that can be accessed by clicking on this item.
 */
class MenuButton : public QObject, public QGraphicsItem {
    Q_OBJECT
#if QT_VERSION >= 0x040600 // Doesn't work o 4.5
    Q_INTERFACES(QGraphicsItem)
#endif 
public:
    explicit MenuButton(QWidget *parent);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
signals:
    void itemAdded(PanelItem *g);
    void panelRotationChanged(int r);
    void fullscreenChanged(bool fs);
    void setServerAddress(QString host);
public slots:
    void setEditMode(bool em);
    void closeCurrentMenu();
private:
    int side;
    bool editMode;
    QWidget *panelWindow;
    QDialog *currentMenu;
    SettingsDialog *settingsDialog;
};

#endif // MENUBUTTON_H
