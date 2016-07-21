import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import ImageProcessor 1.0
Rectangle {
    Layout.fillHeight: true
    Layout.fillWidth:  true
    color: "#80000000"
    property string sliderValue: "180.00"
    property string cboxValue: "0"
    property bool isChangeCBox: false
    signal timerStart()
    signal timerStop()

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

        Rectangle {
            id:configBar
            width: 300
            visible: true
            Layout.rowSpan: 3
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
                    font.pointSize: 12
                }
                Slider {
                    id:sl
                    Layout.fillWidth: true
                    Layout.margins: 10
                    value: 180.00
                    onValueChanged: {
                        slValue.text = value.toFixed(2)
                        setText(meth.text)
                        sliderValue = value
                    }
                    minimumValue: 0
                    maximumValue: 255
                }
                Text {
                    id: slValue
                    Layout.fillWidth: true
                    Layout.margins: 10
                    text: sl.value
                    font.pointSize: 12
                }

                Text {
                    id: cBoxText
                    Layout.fillWidth: true
                    Layout.margins: 10
                    text: qsTr("text")
                    font.pointSize: 12
                }
                MyComboBox {
                    id:cBox
                    //Layout.fillWidth: true
                    //Layout.margins: 10
                    model: ListModel {
                        id: cbItems
                    }
                    onCurrentIndexChanged: {
                        if(isChangeCBox){
                            setText(menubar.currentText)
                            cboxValue = cBox.currentIndex
                        }
                    }
                }
            }
        }

        Text {
            id: info
            Layout.columnSpan: 2
            Layout.margins: 10
            text: qsTr("Info")
            font.pointSize: 12
        }

        RowLayout {
            Layout.columnSpan: 2
            Image {
                id: photoPreview
                width: parent.width/2
                Layout.fillWidth: true
                Layout.fillHeight: true
                fillMode : "PreserveAspectFit"
                //source: "file:./StreamJapansSeaWEBOCAM/file.bmp"
            }
            Image {
                id: photoPreview2
                x:photoPreview.width+5
                width: parent.width/2//-5
                Layout.fillWidth: true
                Layout.fillHeight: true
                fillMode : "PreserveAspectFit"
                //source: "file:./StreamJapansSeaWEBOCAM/testOUT.bmp"
            }
            //            ColumnLayout {
            //            Text {
            //                Layout.columnSpan: 2
            //                Layout.margins: 15
            //                text: qsTr("FFFFF")
            //            }
            //            Text {
            //                Layout.columnSpan: 2
            //                Layout.margins: 15
            //                text: qsTr("FFFFF")
            //            }
            //            Text {
            //                Layout.columnSpan: 2
            //                Layout.margins: 15
            //                text: qsTr("FFFFF")
            //            }
            //            }
        }
        RowLayout {
            Layout.columnSpan: 2
            MyButton {
                id: playButton
                Layout.margins: 10
                Layout.fillWidth: true
                //width: parent.width/2
                borderWidth: 0
                text: "Play"
                fontSize: 14
                onClicked: {
                    playButton.text = playButton.text == "Play" ? "Stop" : "Play"
                    playButton.text == "Play" ? timerStop() : timerStart()
                    imageProcessor.spCam1()
                }
            }
            MyButton {
                id: playButton2
                Layout.margins: 10
                Layout.fillWidth: true
                //width: parent.width/2
                borderWidth: 0
                text: "Play"
                fontSize: 14
                onClicked: {
                    playButton2.text = playButton2.text == "Play" ? "Stop" : "Play"
                    playButton2.text == "Play" ? timerStop() : timerStart()
                    imageProcessor.spCam2()
                }
            }
        }
        //        Text {
        //            Layout.columnSpan: 2
        //            Layout.margins: 15
        //            text: qsTr(" ")
        //        }
    }
    function setText(text) {
        meth.text = text
        info.text = imageProcessor.pluginInfo(text)
        var jsonObject = imageProcessor.processImage(text, sl.value,cBox.currentIndex)

        photoPreview.source = "file:./file.bmp"
        photoPreview.update()
        photoPreview2.source = "file:./file.bmp"
        photoPreview2.update()
        photoPreview.source = "file:./cam1.bmp"
        photoPreview2.source = "file:./cam2.bmp"

        myFunction(jsonObject)

    }
    function myFunction(response) {
        isChangeCBox = false
        console.log(response)
        var arr = JSON.parse(response);
        var currIndex = cBox.currentIndex
        for(var i = 0; i< arr.length;i++)
        {

            if(arr[i].name === undefined) {
                configBar.visible = false;
                configBarElements.visible = false
                return;
            }
            else {
                configBar.visible = true;
                configBarElements.visible = true

                slMethod.visible = false
                sl.visible = false
                slValue.visible = false
                cBox.visible = false
                cBoxText.visible = false
                cbItems.clear()
            }
            if(arr[i].type==="slider")
            {
                slMethod.text = arr[i].name
                sl.minimumValue = arr[i].min
                sl.maximumValue = arr[i].max

                if(arr[i].step === undefined)
                    sl.stepSize = 1.0;
                else
                    sl.stepSize = arr[i].step

                slMethod.visible = true
                sl.visible = true
                slValue.visible = true
            }
            if(arr[i].type==="cbox")
            {
                cBoxText.text = arr[i].name
                for(var j = 0; j< arr[i].count;j++)
                    cbItems.append({"text": arr[i]["var"+j]})
                cBox.visible = true
                cBoxText.visible = true
                cBox.currentIndex = currIndex
            }
        }
        isChangeCBox = true
        //        }
        //        for(var i = 0; i < arr.length; i++) {
        //            listview.model.append( {listdata: arr[i].type +" "+ arr[i].name +" "+ arr[i].min +" "+ arr[i].max })
        //        }
    }

    Timer {
        id:timer
        interval: 1000; running: false; repeat: true
        onTriggered: {
            photoPreview.source = "file:" + imageProcessor.getCam1File()
            photoPreview2.source = "file:" + imageProcessor.getCam2File()
            //            timer.stop()
        }

    }
}

