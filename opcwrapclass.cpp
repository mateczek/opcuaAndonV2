#include "opcwrapclass.h"
#include <QFile>
#include<QTimer>
opcWrapClass::opcWrapClass(QObject *parent)
    : QObject{parent}
{
    opcUaBackend=new opcuaBackend(this);
    connect(opcUaBackend,&opcuaBackend::finished,this,&opcWrapClass::backendThreadFinisch);
    QFile plik("setings.txt");
    if(plik.open(QIODevice::Text|QIODevice::ReadOnly )){
        QTextStream ts (&plik);
        ts>> opcuaserver >> _gauge_nd_nr[0]>> _gauge_nd_nr[1] >> _gauge_nd_nr[2];
        QStringList sl;
        for (int i =0;i<3;i++){
            sl <<"ND"+QString::number(_gauge_nd_nr[i])+".plc.precentFill"
                                                     //".plc.precentFill"
               <<"ND"+QString::number(_gauge_nd_nr[i])+".plc.precentCool"
                                                     //".plc.precentCool"
               <<"ND"+QString::number(_gauge_nd_nr[i])+".plc.remainingTime"
                                                     //".plc.remainingTime"

               <<"ND"+QString::number(_gauge_nd_nr[i])+".plc.NdStop";
        };
        opcUaBackend->setNodes(sl);
        opcUaBackend->setUaEndpoint(opcuaserver);
        connect(opcUaBackend,SIGNAL(dataChangeGauge(int,opcuaBackend::Gauge)),this,SLOT(updateData(int, opcuaBackend::Gauge)));
        opcUaBackend->start();

    }


}

opcWrapClass::~opcWrapClass()
{
   disconnect(opcUaBackend,SIGNAL(dataChangeGauge(int,opcuaBackend::Gauge)),this,SLOT(updateData(int, opcuaBackend::Gauge)));
   opcUaBackend->stopThread();
   opcUaBackend->wait();
}

void opcWrapClass::updateData(int gaugenr, opcuaBackend::Gauge gaugeData){
    switch (gaugenr) {
        case 0:
            emit updateGauge1(gaugeData.precentFill,gaugeData.precentCool,gaugeData.remanignTime,gaugeData.enableMC);
        break;
        case 1:
            emit updateGauge2(gaugeData.precentFill,gaugeData.precentCool,gaugeData.remanignTime,gaugeData.enableMC);
        break;
        case 2:
            emit updateGauge3(gaugeData.precentFill,gaugeData.precentCool,gaugeData.remanignTime,gaugeData.enableMC);
        break;
    }
}

void opcWrapClass::backendThreadFinisch()
{
    qDebug()<<"próba odnowienia połączenia za 10 sekund";
    QTimer::singleShot(5000,[this](){
       opcUaBackend->start();

    });
}
