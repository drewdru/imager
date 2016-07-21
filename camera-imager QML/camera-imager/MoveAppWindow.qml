import QtQuick 2.0
import QtQuick.Layouts 1.1
Item {
    id:rect
    Layout.fillWidth: true
    Layout.fillHeight: true
    state: "normal"
    property string lastState: "normal"
    MouseArea {
        anchors.fill: parent
        property variant previousPosition
        onPressed: {
            previousPosition = Qt.point(mouseX, mouseY)
        }
        onPositionChanged: {
            if (pressedButtons == Qt.LeftButton) {
                if(rect.state==="maximized")
                {
                    rect.state = "normal"
                    previousPosition.x = mouseX
                    previousPosition.y = mouseY
                }
                var dx = mouseX - previousPosition.x
                var dy = mouseY - previousPosition.y
                app.x = app.x + dx
                app.y = app.y + dy
            }
        }
        onDoubleClicked: {
            rect.state = rect.state === "maximized" ? "normal" : "maximized"
        }
    }
    states: [
        State {
            name: "maximized"
            onCompleted: app.showMaximized()
        },
        State {
            name: "normal"
            onCompleted: app.showNormal()
        }
    ]
}


//    Rectangle {//ИЗ-ЗА растяжения других эл. -- НЕТ не так ОШИБКА Я "НИПАНИМАТ"!
//        anchors.fill: parent
//        color: "#00FFFFFF"

//        border.width: 10
//        border.color: "#0FFF0000"
//        MouseArea {
//            anchors.fill: parent
//            property variant startPosition
//            onPressed: {
//                startPosition = Qt.point(mouseX, mouseY)
//                /*console.log(startPosition.x)
//                console.log(startPosition.y)*/
//            }
//            onPositionChanged: {
//                var pb = parent.border.width
//                if (pressedButtons == Qt.LeftButton) {
//                    if(startPosition.x>app.width-pb && startPosition.y<app.height-pb)
//                    {
//                        app.width = mouseX
//                        startPosition.x = mouseX > app.minimumWidth ? mouseX : app.minimumWidth
//                    console.log("FUCK X")
//                    }
//                    else if(startPosition.x<app.width-pb && startPosition.y>app.height-pb)
//                    {
//                        app.height = mouseY
//                        startPosition.y = mouseY > app.minimumHeight ? mouseY : app.minimumHeight
//                    console.log("FUCK Y")
//                    }
//                    else if(startPosition.x>=app.width-pb && startPosition.y>=app.height-pb)
//                    {
//                        app.width = mouseX
//                        app.height = mouseY
//                        startPosition.x = mouseX > app.minimumWidth ? mouseX : app.minimumWidth
//                        startPosition.y = mouseY > app.minimumHeight ? mouseY : app.minimumHeight

//                    }
//                    /*else if(mouseX>app.width-pb && mouseY<app.height-pb)
//                        app.width = mouseX
//                    else if(mouseX<app.width-pb && mouseY>=app.height-pb)
//                        app.height = mouseY
//                    else if(mouseX>app.width-pb && mouseY>=app.height-pb)
//                    {
//                        app.width = mouseX
//                        app.height = mouseY
//                    }*/
//                    else {
//                        /*console.log(app.width)
//                        console.log(app.height)*/

//                        /*console.log(startPosition.x)
//                        console.log(startPosition.y)*/
//                    }
//                }
//            }
//        }
//    }


