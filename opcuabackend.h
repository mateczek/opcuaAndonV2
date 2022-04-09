#ifndef OPCUABACKEND_H
#define OPCUABACKEND_H

#include <QThread>
#include <open62541/client_config_default.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>

#include<QDebug>
class opcuaBackend : public QThread
{
    Q_OBJECT
public:
    struct Gauge{
      int precentFill;
      int precentCool;
      int remanignTime;
      bool enableMC;
    };
    explicit opcuaBackend(QObject *parent = nullptr);
    void setNodes(const QStringList &sl);
    void setUaEndpoint(const QString s){
        opcuaEndpoind=s.toStdString();
    }
    void stopThread(){
        runing=false;
    }
private:
    static Gauge gauges[3];
    static bool runing;
    static int count_inactivity;
    std::string opcuaEndpoind{"opc.tcp://127.0.0.1:49320"};
    void run();


    static void stopHandler(int sign) {
       // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Ctrl-C");
        qDebug()<<"stop opcua";
        runing = false;
    }
    static void deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId, void *subContext) {
        //runing =false;
        //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    //"Subscription Id %u was deleted", subscriptionId);
    }
    static void subscriptionInactivityCallback (UA_Client *client, UA_UInt32 subId, void *subContext) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Inactivity for subscription %u", subId);
        count_inactivity++;

       if (count_inactivity>5) runing=false;
    }

    static void statecalback(UA_Client *client, UA_SecureChannelState channelState,
                             UA_SessionState sessionState, UA_StatusCode recoveryStatus);
    static void handler_currentTimeChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
                               UA_UInt32 monId, void *monContext, UA_DataValue *value);
    static QStringList nodes;
    static opcuaBackend *instance;


signals:
    void dataChangeGauge(int,opcuaBackend::Gauge);
};

#endif // OPCUABACKEND_H
