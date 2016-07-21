import QtQuick 2.0

Rectangle {
    anchors.fill: parent
    //color: "#FF101010"

    border.width: 10
    border.color: "#3C3C3C"
    MouseArea {
        anchors.fill: parent
        property variant startPosition
        onPressed: {
            startPosition = Qt.point(mouseX, mouseY)
            /*console.log(startPosition.x)
            console.log(startPosition.y)*/
        }
        onPositionChanged: {
            var pb = parent.border.width
            if (pressedButtons == Qt.LeftButton) {
                if(startPosition.x>app.width-pb && startPosition.y<app.height-pb)
                {
                    app.width = mouseX
                    startPosition.x = mouseX > app.minimumWidth ? mouseX : app.minimumWidth
                }
                else if(startPosition.x<app.width-pb && startPosition.y>app.height-pb)
                {
                    app.height = mouseY
                    startPosition.y = mouseY > app.minimumHeight ? mouseY : app.minimumHeight
                }
                else if(startPosition.x>=app.width-pb && startPosition.y>=app.height-pb)
                {
                    app.width = mouseX
                    app.height = mouseY
                    startPosition.x = mouseX > app.minimumWidth ? mouseX : app.minimumWidth
                    startPosition.y = mouseY > app.minimumHeight ? mouseY : app.minimumHeight
                }
            }
        }
    }
}


