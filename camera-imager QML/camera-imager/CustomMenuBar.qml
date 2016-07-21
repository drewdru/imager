import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import ImageProcessor 1.0

Rectangle {
    id: menuBar
    color: "#3C3C3C"
    border.color: "#282828"
    width: parent.width
    height: 60
    property string text: "value"
    signal curentMethodChange(string txt)
    signal getDllDir()

    RowLayout
    {
        id:qwer
        anchors.fill: parent
        anchors.margins: 0

        /*Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }*/
        Rectangle {
            color: "white"
            Layout.margins: 10
            width: 30
            height: 30
            radius: 30
            Image {
                source: "image/icon"
                sourceSize.width: parent.width
                sourceSize.height: parent.height-2
            }
        }

        Text{
            color: "white"
            text: qsTr("Method:")
            Layout.margins: 10
        }

        MyComboBox {
            id:methodList
            Layout.margins: 10
//             Layout.fillWidth: true
            model: imageProcessor.getMethodsList()
            onCurrentTextChanged: {
//                imageProcessor.processImage(currentText)
//                photoPreview2.source = "file:./testOUT.bmp"
//                menuBar.curentMethod("ASD")
                menuBar.curentMethodChange(currentText)
            }
        }
        MyButton {
            Layout.topMargin:  10
            Layout.rightMargin: 0
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            Layout.fillHeight: true
            width:90
            borderWidth: 0
            text: "AddDLL"
            fontSize: 14
            onClicked: {
                menuBar.getDllDir()
            }
        }

        MoveAppWindow {
            id:rect
        }
//        MyButton {
//            id: minimizeButton
//            Layout.topMargin:  10
//            Layout.rightMargin: 0
//            Layout.bottomMargin: 10
//            Layout.fillHeight: true
//            width: menuBar.height
//            borderWidth: 0

//            text: "_"
//            onClicked: app.showMinimized()
//        }
//        MyButton {
//            id: maximizeButton
//            Layout.topMargin:  10
//            Layout.rightMargin: 0
//            Layout.leftMargin: 0
//            Layout.bottomMargin: 10
//            Layout.fillHeight: true
//            width: menuBar.height
//            borderWidth: 0
//            text: "◇◆"
//            onClicked: rect.state = rect.state === "maximized" ? "normal" : "maximized"
//        }
//        MyButton {
//            id: closeButton
//            Layout.topMargin:  10
//            Layout.rightMargin: 10
//            Layout.leftMargin: 0
//            Layout.bottomMargin: 10
//            Layout.fillHeight: true
//            width: menuBar.height
//            borderWidth: 0
//            text: "X"
//            onClicked: Qt.quit()
//        }
    }
    function addDLL(path) {
        imageProcessor.addDLL(path)
        methodList.model = imageProcessor.getMethodsList()
    }
}


