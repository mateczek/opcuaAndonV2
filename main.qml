import QtQuick 2.15
import QtQuick.Window 2.15
Window {
    visible: true
    visibility: "FullScreen"
    title: qsTr("Hello World")
    Timer{
        interval: 100
        running: true
        repeat: true
        onTriggered: {
            const date = new Date;
            const minute = date.getMinutes()
            const hour =date.getHours()
            zegarek.text=((hour<10)?"0"+hour:hour)+":"+((minute<10)?"0"+minute:minute);
        }
    }

    Image {
        id: tlo
        anchors.fill: parent
        source:"background.jpg"
    }
    Rectangle{
        width: 500
        height: 150
        radius: 50
        anchors.horizontalCenter: parent.horizontalCenter
        color:"#77777777"
        y:50;
        Text {
            anchors.centerIn: parent
            id: zegarek
            color: "#adff11"
            font.bold: true
            font.pixelSize: 130

            text: "00:00";
        }
    }

    GaugeAndon{
        id:gaug1
        anchors.leftMargin: 15
        anchors.left: parent.left
        ndNr: uaBackend.gauge1_nd_nr
    }
    GaugeAndon{
        id:gaug2
        anchors.horizontalCenter:parent.horizontalCenter
        ndNr: uaBackend.gauge2_nd_nr
        y:250
    }
    GaugeAndon{
        id:gaug3
        ndNr: uaBackend.gauge3_nd_nr
        anchors.rightMargin: 15
        anchors.right: parent.right
    }
    Connections{
        target: uaBackend
        function onUpdateGauge1( precFill, precCool,timReamCycle, enable) {
            gaug1.precFill=precFill;
            gaug1.precCoolling=precCool;
            gaug1.remaningTime= timReamCycle;
            if (timReamCycle==0) gaug1.flip =false;
            else gaug1.flip=true;
            gaug1.en =enable;
        }
        function onUpdateGauge2( precFill, precCool,timReamCycle,enable) {
            gaug2.precFill=precFill;
            gaug2.precCoolling=precCool;
            gaug2.remaningTime= timReamCycle;
            if (timReamCycle==0) gaug2.flip =false;
            else gaug2.flip=true;
            gaug2.en =enable;

        }
        function onUpdateGauge3( precFill, precCool,timReamCycle, enable) {
            gaug3.precFill=precFill;
            gaug3.precCoolling=precCool;
            gaug3.remaningTime= timReamCycle;
            if (timReamCycle==0) gaug3.flip =false;
            else gaug3.flip=true;
            gaug3.en =enable;

        }


    }
}
