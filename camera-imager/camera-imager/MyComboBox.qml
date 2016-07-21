import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Private 1.0

ComboBox {
    //FontLoader { id: customFont; source:"qrc:/resources/fonts/OpenSans-Regular.ttf"}

    style: ComboBoxStyle {
        background: Rectangle {
            implicitWidth: 300
            implicitHeight: 40
            color: "#FFFFFF"
            border.width: 1
            border.color: !control.enabled ? "#DADAD9" : control.activeFocus ? "#3C3C3B" : "#9D9C9C"
            antialiasing: true
            Rectangle {
                id: glyph
                width: 10
                height: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: !control.enabled ? "#DADAD9" : control.hovered ? "#1D5086" : "#5791CD"
                states: State {
                    name: "inverted"
                    when: __popup.__popupVisible
                    PropertyChanges { target: glyph; rotation: 180 }
                }
                transitions: Transition {
                    RotationAnimation { duration: 50; direction: RotationAnimation.Clockwise }
                }
            }
        }
        label: Label {
            verticalAlignment: Qt.AlignVCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            text: control.currentText
            color: !control.enabled ? "#DADAD9" : "#6F6E6E"
            anchors.fill: parent
            font.pointSize: 14
            //font.pixelSize: 12
            //font.family: customFont.name
        }
        selectionColor : "#FF6666"
        selectedTextColor : "black"
    }
}
