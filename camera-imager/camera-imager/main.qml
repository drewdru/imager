import QtMultimedia 5.4
import ImageProcessor 1.0
//import SqlQueryModel 1.0
import QtQuick 2.3
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
//import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
//import opencv.components 1.0
//import lcvcore
ApplicationWindow {
    id: app
    //flags: Qt.FramelessWindowHint
    visible: true
    x: initialX
    y: initialY
    width: initialWidth
    height: initialHeight
    minimumWidth: 1280
    minimumHeight: 480
    title: qsTr("Imager")
    color: "#F0F0F0"

    ImageProcessor{
        id: imageProcessor
    }

//    SqlQueryModel{
//        id: sqlNotes
//    }


//    Component.onCompleted: {
//        imageProcessor.startThread()
//    }

//    MyFrameless {
//    }

    CustomMenuBar {
        id:menubar
        Timer {
            id:timer
            interval: 1000; running: false; repeat: true
            onTriggered: {
                //console.log(menubar.currentText)
                shown.setText(menubar.currentText)
            }
        }        
        onCurentMethodChange: {shown.setText(txt)}
        onGetDllDir: fileDialogLoad.open()
        onProcessAll: {
            imageProcessor.processAllClicked(menubar.currentText,shown.sliderValue,shown.cboxValue)
        }
//        onProcessAllClicked: {
//            shown.visible = !shown.visible
//            processAll.visible = !processAll.visible
//        }
    }

    ShowProcessing {
        id:shown
        y:menubar.height
        width: parent.width
        height: parent.height - y
        onTimerStart: {timer.start()}
        onTimerStop: {timer.stop()}
    }
//    AllProcessing {
//        id:processAll
//        y:menubar.height
//        visible: false
//        width: parent.width
//        height: parent.height - y
//    }

    FileDialog {
        id: fileDialogLoad
        folder: "."
        title: "Choose a file to open"
        selectMultiple: false
        nameFilters: [ "DLL files (*.so *.dll *.dylib *.sl *.bundle)", "All files (*)" ]
        onAccepted: {
            function replaced(str, p1, p2) {
                var res = encodeURI(str)
                return res.replace(p1,p2)
            }
            //linux
            //menubar.addDLL(replaced(fileUrl, "file://",""))
            //windows
            menubar.addDLL(replaced(fileUrl, "file:///",""))
        }
    }

}




//    VideoOutput {
//        id: videoOut
//        y: cbox.height+5
//        width: parent.width
//        height: parent.height - y
//        //anchors.fill: parent
//        source: camera
//        focus : visible
//        autoOrientation: true
//        fillMode: Image.PreserveAspectFit
//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                camera.imageCapture.capture();
//            }
//        }
//        //shows captured image
//        Image {
//            anchors.fill: parent
//            id: photoPreview
//            fillMode: Image.PreserveAspectFit
//        }
//    }

//    Camera {
//        id: camera
//        imageProcessing {
//            whiteBalanceMode: CameraImageProcessing.WhiteBalanceTungsten
//            contrast: 0.66
//            saturation: -0.5
//            denoisingLevel: 0.60
//        }
//        captureMode: Camera.CaptureStillImage

//        exposure {
//            exposureCompensation: -1.0
//            exposureMode: Camera.ExposurePortrait
//        }

//        flash.mode: Camera.FlashRedEyeReduction

//        imageCapture {
//            onImageCaptured: {
//                //photoPreview.source = preview
//                imageProcessor.processImage(preview);
//            }
//            onImageSaved: {
//                text.text = qsTr("Last Captured Image (%1):").arg(camera.imageCapture.capturedImagePath)
//            }
//        }
//    }

//    //image processor for further image processing
//    ImageProcessor{
//        id: imageProcessor
//    }


