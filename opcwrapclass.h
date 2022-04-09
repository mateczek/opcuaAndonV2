#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <opcuabackend.h>
class opcWrapClass : public QObject
{
    Q_OBJECT
    int _gauge_nd_nr[3];
    QString opcuaserver;

    Q_PROPERTY(void updateGauge1 NOTIFY updateGauge1)
    Q_PROPERTY(void updateGauge2 NOTIFY updateGauge2)
    Q_PROPERTY(void updateGauge3 NOTIFY updateGauge3)
    Q_PROPERTY(int gauge1_nd_nr READ gauge1_nd_nr)
    Q_PROPERTY(int gauge2_nd_nr READ gauge2_nd_nr)
    Q_PROPERTY(int gauge3_nd_nr READ gauge3_nd_nr)
    opcuaBackend *opcUaBackend;

public:
    explicit opcWrapClass(QObject *parent = nullptr);
    ~opcWrapClass();
    int gauge1_nd_nr(){
        return _gauge_nd_nr[0];
    }
    int gauge2_nd_nr(){
        return _gauge_nd_nr[1];
    }
    int gauge3_nd_nr(){
        return _gauge_nd_nr[2];
    }
signals:
    void updateGauge1(double precentFilling, double precentCooling,int ReamingTime,bool enableMC);
    void updateGauge2(double precentFilling, double precentCooling,int ReamingTime,bool enableMC);
    void updateGauge3(double precentFilling, double precentCooling,int ReamingTime,bool enableMC);
private slots:
    void updateData (int gaugenr,opcuaBackend::Gauge gaugeData);
    void backendThreadFinisch();
};

#endif // ENGINE_H
