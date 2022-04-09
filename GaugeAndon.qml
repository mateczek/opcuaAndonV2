import QtQuick 2.9
Item {
    id:rootGauge
    width: 650
    height: 930
    antialiasing: true
    property real precFill: 80
    property real precCoolling: 30
    property int  remaningTime: 100
    property int ndNr: 17
    property bool en: true
    property bool flip:false

    property real gaugeFill: -169+precFill*1.645
    property real gaugeCooling: 6+precCoolling*1.645
    Behavior on gaugeFill{
        SmoothedAnimation {
            velocity: 0.1
            duration: 1200
        }
    }
    Behavior on gaugeCooling{
        SmoothedAnimation {
            velocity: 0.1
            duration: 1200
        }
    }
    function sclaling (x){
        return x*rootGauge.width/400;
    }
    ArcItem {
        id: fill_gauge
        anchors.centerIn: parent
        width: parent.width-20
        height: width
        visible: true
        fillColor: "#00000000"
        strokeColor:en ? "#ff0000" :"#77777777"
        antialiasing: true
        strokeStyle: 1
        begin: -170
        end: gaugeFill
        strokeWidth: sclaling(28)
        capStyle: 32
    }

    ArcItem {
        id: cooling_gauge
        width: parent.width-20
        height: width
        fillColor: "#00000000"
        strokeColor: en ?"#84f697":"#77777777"
        antialiasing: true
        roundEnd: false
        begin: 5
        strokeStyle: 1
        end: gaugeCooling
        strokeWidth: sclaling(28)
        anchors.centerIn: parent
        capStyle: 32
    }
    ArcItem {
        anchors.centerIn: parent
        width: parent.width-15
        height: width
        visible: true
        fillColor: "#00000000"
        strokeColor:"#777777"
        antialiasing: true
        strokeStyle: 1
        begin: -175
        end: 175
        opacity:0.3
        strokeWidth: sclaling(32)
    }
    function secToString (ms){
        let outstring;
        let temp=Math.floor(ms/60);
        if  (temp>=10) outstring=temp+":";
        else outstring = "0"+temp+":";
        temp =ms%60;
        if  (temp>=10) outstring+=temp;
        else outstring += "0"+temp;
        return outstring;
    }
    Flipable{
        id:flipable
        opacity:en?1:0.3
        width: parent.width-50;
        height: width
        anchors.centerIn: parent
        front:Text {
            id: timer
            color: en ? "#edff11":"#777777"
            anchors.centerIn: flipable
            text: secToString(remaningTime);
            font.pixelSize: sclaling(80)
            font.family: "Eurostile"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }
        back: Image {

            id:img
            width: parent.width
            height: width
            anchors.centerIn: flipable
            source: "wheel.png"

        }
        transform: Rotation {
                id: rotation
                axis.x: 0; axis.y: 1; axis.z: 0
                origin.x: flipable.width/2
                origin.y: flipable.height/2
                // set axis.y to 1 to rotate around y-axis
                angle: flip?0:180
                Behavior on angle {
                    NumberAnimation{
                       duration: 500
                    }
                }
                // the default angle
       }
    }


    Text {
        id: nd_number
        anchors.horizontalCenter: parent.horizontalCenter
        y: 30
        text: qsTr("ND "+ndNr)
        color: en ?"#ffffffff":"#77777777"
        font.pixelSize: sclaling(55)

        horizontalAlignment: Text.AlignHCenter
    }
}
