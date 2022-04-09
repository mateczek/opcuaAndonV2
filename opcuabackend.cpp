#include "opcuabackend.h"
#include<QDebug>
void opcuaBackend::run()
{
    count_inactivity=0;
    UA_Client *client = UA_Client_new();
    UA_ClientConfig *cc = UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(cc);
    cc->subscriptionInactivityCallback=opcuaBackend::subscriptionInactivityCallback;
    cc->stateCallback = opcuaBackend::statecalback;
    UA_StatusCode retval = UA_Client_connect(client, opcuaEndpoind.data());
    runing=retval == UA_STATUSCODE_GOOD;
    while(runing) {
            UA_Client_run_iterate(client, 500);
    };
    UA_Client_delete(client); /* Disconnects the client internally */
}

QStringList opcuaBackend::nodes;
opcuaBackend* opcuaBackend::instance;
bool opcuaBackend::runing =false;
opcuaBackend::Gauge opcuaBackend::gauges[3];
int opcuaBackend::count_inactivity=0;

void opcuaBackend::statecalback(UA_Client *client, UA_SecureChannelState channelState,
                                UA_SessionState sessionState, UA_StatusCode recoveryStatus)
{
        switch(channelState) {
        case UA_SECURECHANNELSTATE_FRESH:
        case UA_SECURECHANNELSTATE_CLOSED:
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "The client is disconnected");
            runing=false;
            break;
        case UA_SECURECHANNELSTATE_HEL_SENT:
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Waiting for ack");
            break;
        case UA_SECURECHANNELSTATE_OPN_SENT:
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Waiting for OPN Response");
            break;
        case UA_SECURECHANNELSTATE_OPEN:
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A SecureChannel to the server is open");
            break;
        default:
            break;
        }
        switch(sessionState) {
            case UA_SESSIONSTATE_ACTIVATED: {
                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A session with the server is activated");
                UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
                UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request, NULL, NULL, deleteSubscriptionCallback);

                if(response.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
                    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"Create subscription succeeded, id %u",response.subscriptionId);
                else{
                    runing =false;
                    return;
                }
                for(int i=0;i<nodes.size();i++){
                    UA_NodeId node = UA_NODEID_STRING(2,nodes[i].toLatin1().data());

                    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(node);
                    UA_MonitoredItemCreateResult res= UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,UA_TIMESTAMPSTORETURN_BOTH, monRequest,NULL, handler_currentTimeChanged, NULL);

                    if (res.statusCode==UA_STATUSCODE_GOOD){
                         // runing=false;

                    }
                }
            }
            break;
            case UA_SESSIONSTATE_CLOSED:
                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "sesia rozłączona");
                runing=false;
            break;
            default:
            break;
        }
}

void opcuaBackend::handler_currentTimeChanged(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value)
{
    int gaugeCounter = (monId-1)/4;
    int gaugeIndexValue =(monId-1)%4;
    if (value->status!= UA_STATUSCODE_GOOD){
        gauges[gaugeCounter].precentFill =0;
        gauges[gaugeCounter].precentCool =0;
        gauges[gaugeCounter].remanignTime=0;
        gauges[gaugeCounter].enableMC =false;
        emit instance->dataChangeGauge(gaugeCounter,gauges[gaugeCounter]);
        return;

    }
    if (gaugeIndexValue==3){
        gauges[gaugeCounter].enableMC=*((UA_Boolean*)value->value.data);
        if (gauges[gaugeCounter].enableMC==false){
            gauges[gaugeCounter].precentFill =0;
            gauges[gaugeCounter].precentCool =0;
            gauges[gaugeCounter].remanignTime=0;
        }
         emit instance->dataChangeGauge(gaugeCounter,gauges[gaugeCounter]);
         return;
    };
    if (gauges[gaugeCounter].enableMC==false) return;

    switch (gaugeIndexValue) {
        case 0:
            gauges[gaugeCounter].precentFill= *((UA_Float*)value->value.data);
        break;
        case 1:
             gauges[gaugeCounter].precentCool= *((UA_Float*)value->value.data);
        break;
        case 2:
             gauges[gaugeCounter].remanignTime= *((UA_UInt32*)value->value.data);
             emit instance->dataChangeGauge(gaugeCounter,gauges[gaugeCounter]);
        break;
    }
}

opcuaBackend::opcuaBackend(QObject *parent)
    : QThread{parent}
{

    opcuaBackend::instance=this;

}

void opcuaBackend::setNodes(const QStringList &sl)
{
     opcuaBackend::nodes = sl;
}

