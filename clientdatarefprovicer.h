#ifndef CLIENTDATAREFPROVICER_H
#define CLIENTDATAREFPROVICER_H

#include "clientdataref.h"

class ClientDataRefProvicer
{
public:
    ClientDataRefProvicer();
    virtual ClientDataRef *subscribeDataRef(QString name, double accuracy=0)=0;
    virtual void unsubscribeDataRef(ClientDataRef *ref)=0;
};

#endif // CLIENTDATAREFPROVICER_H
