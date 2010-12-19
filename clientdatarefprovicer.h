#ifndef CLIENTDATAREFPROVICER_H
#define CLIENTDATAREFPROVICER_H

#include "clientdataref.h"

class ClientDataRefProvicer
{
public:
    ClientDataRefProvicer();
    virtual ClientDataRef *subscribeDataRef(QString name, double accuracy=0)=0;
    virtual void unsubscribeDataRef(ClientDataRef *ref)=0;
    virtual void keyPress(int id)=0;
    virtual void buttonPress(int id)=0;
    virtual void buttonRelease(int id)=0;
};

#endif // CLIENTDATAREFPROVICER_H
