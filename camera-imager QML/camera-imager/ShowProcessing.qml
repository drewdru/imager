import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import ImageProcessor 1.0
Rectangle {
    Layout.fillHeight: true
    Layout.fillWidth:  true
    color: "#80000000"

    Text {
        visible:false
        id: meth
        text: qsTr("text")
    }

    GridLayout
    {
        id:grid
        anchors.fill: parent
        columns: 3
        anchors.margins: 15
        //flow: GridLayout.TopToBottom

/*        Rectangle {
            id:configBar
            width: 300
            visible: true
            Layout.rowSpan: 2
            Layout.fillHeight: true
            color: "white"
            ColumnLayout {
                id:configBarElements
                Layout.fillWidth: true
                Text {
                    id: slMethod
                    Layout.fillWidth: true
                    Layout.margins: 10
                    text: qsTr("text")
                }
                Slider {
                    id:sl
                    Layout.fillWidth: true
                    Layout.margins: 10
                    onValueChanged: {
                        slValue.text = value
                        setText(meth.text)
                    }
                    minimumValue: 0
                    maximumValue: 255
                    stepSize: 1.0

                }
                Text {
                    id: slValue
                    Layout.fillWidth: true
                    Layout.margins: 10
                    text: sl.value
                }
            }
        }*/

//        Text {
//            id: info
//            Layout.columnSpan: 2
//            Layout.margins: 10
//            text: qsTr("Info")
//        }

        RowLayout {
            Layout.columnSpan: 2
            Image {
                id: photoPreview
                width: parent.width/2
                Layout.fillWidth: true
                Layout.fillHeight: true
                fillMode : "PreserveAspectFit"
                source: "file:./file.bmp"
            }
            Image {
                id: photoPreview2
                x:photoPreview.width+5
                width: parent.width/2//-5
                Layout.fillWidth: true
                Layout.fillHeight: true
                fillMode : "PreserveAspectFit"
                source: "file:./testOUT.bmp"
            }
        }

        Text {
            Layout.columnSpan: 2
            Layout.margins: 15
            text: qsTr(" ")
        }

    }
    function setText(text) {
        meth.text = text
        info.text = imageProcessor.pluginInfo(text)
        var jsonObject = imageProcessor.processImage(text, sl.value)
        photoPreview2.source = "file:./file.bmp"
        photoPreview2.source = "file:./testOUT.bmp"
        myFunction(jsonObject)

    }
    function myFunction(response) {
        var arr = JSON.parse(response);
        if(arr[0].name === undefined) {
            configBar.visible = false;
            configBarElements.visible = false
            return;
        }
        else {
            configBar.visible = true;
            configBarElements.visible = true
        }

        slMethod.text = arr[0].name
        sl.minimumValue = arr[0].min
        sl.maximumValue = arr[0].max
        //for(var i = 0; i < arr.length; i++) {

            //listview.model.append( {listdata: arr[i].type +" "+ arr[i].name +" "+ arr[i].min +" "+ arr[i].max })
        //}
    }
    //    Timer {
    //        id:timer
    //        interval: 5000; running: true; repeat: true
    //        onTriggered: {
    //            photoPreview2.source = "file:./testOUT2.bmp"
    //            photoPreview2.source = "file:./testOUT.bmp"
    //            photoPreview2.update()
    //            console.log("TIMER IS WORK!")
    //            timer.stop()
    //        }
    //    }
}

