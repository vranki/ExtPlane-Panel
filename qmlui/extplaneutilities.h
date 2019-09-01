#ifndef EXTPLANEUTILITIES_H
#define EXTPLANEUTILITIES_H

#include <QObject>

class ExtplaneUtilities : public QObject
{
    Q_OBJECT
public:
    explicit ExtplaneUtilities(QObject *parent = nullptr);
    Q_INVOKABLE static void copySettings(QObject *from, QObject *to);
signals:

public slots:
};

#endif // EXTPLANEUTILITIES_H
