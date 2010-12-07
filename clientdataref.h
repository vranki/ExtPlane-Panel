#ifndef CLIENTDATAREF_H
#define CLIENTDATAREF_H

#include <QObject>

class ClientDataRef : public QObject {
    Q_OBJECT
public:
    explicit ClientDataRef(QObject *parent, QString newName, double accuracy=0);
    QString name();
    QString valueString();
    double accuracy();
    void updateValue(QString newValue);
    int subscribers();
    void setSubscribers(int sub);
signals:
    void changed(ClientDataRef *ref);
public slots:

private:
    QString _name;
    QString _value;
    double _accuracy;
    int _subscribers;
};

#endif // CLIENTDATAREF_H
