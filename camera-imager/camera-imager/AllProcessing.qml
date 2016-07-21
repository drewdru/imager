import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.LocalStorage 2.0
//import ThreadProcessor 1.0


Rectangle {
    Layout.fillHeight: true
    Layout.fillWidth:  true
    color: "#80000000"
}

//    Component.onCompleted: {
//     sqlNotes.openDB();
//     sqlNotes.initDB();
//        sqlNotes.setQuery("SELECT * FROM test1");
//        notesView.model = sqlNotes;

////        sqlNoteItem.setQuery("SELECT * FROM notes WHERE id=3");
////        textTitle.text = sqlNoteItem.datasingle("title");
////        textNote.text = sqlNoteItem.datasingle("note");
//    }



//    //    property var db: null
//    //    Component.onCompleted: {
//    //        var distro = getSetting('distro')
//    //        console.log(distro)
//    //        //distroToggle.selectedIndex = (distro === "Debian") ? 1 : 0
//    //        var foo = getSetting('foo')
//    //        console.log(foo)
//    //        //fooToggle.selectedIndex = (foo === "Baz") ? 1 : 0
//    //    }
//    //    ListModel {
//    //        id: sqlModel
//    //        onModelReset:
//    //        {
//    //            openDB()
//    //            sqlModel.append(tx.executeSql("SELECT * FROM settings"))
//    //        }
//    //    }



//    GridLayout
//    {
//        id:grid
//        anchors.fill: parent
//        anchors.margins: 15
//        columns: 2

//        RowLayout {
//            Layout.columnSpan: 2
//            anchors.top: parent.top
//            Text {
//                text: qsTr("Количество потоков")
//            }
//            Slider {
//                minimumValue: 1
//                maximumValue: 8
//                value: 4
//                stepSize: 1
//                onValueChanged: sliderValue.text = value
//            }
//            Text {
//                id: sliderValue
//                text: qsTr("4")
//            }
//        }
//        Rectangle {
//            Layout.fillHeight: true
//            Layout.fillWidth:  true
//            color: "#80ff0000"

//            ListView {
//                id: notesView

//            }

//        }
//        Rectangle {
//            Layout.fillHeight: true
//            Layout.fillWidth:  true
//            color: "#800000ff"

//            //            ListView {
//            //                width: 100; height: 100

//            //                model: threadProcessor.getModel()
//            //                delegate: Rectangle {
//            //                    height: 25
//            //                    width: 100
//            //                    Text { text: modelData }
//            //                }
//            //            }
//            ListView {
//                id: view
//                width: parent.width
//                anchors.top: parent.top
//                anchors.bottom: column.top
//                model: model1
//                spacing: 2
//                delegate: Component {
//                    Rectangle {
//                        width: view.width
//                        implicitHeight: edit.implicitHeight + 10
//                        color: "transparent"
//                        border.color: "red"
//                        border.width: 2
//                        radius: 5
//                        TextInput {
//                            id: edit
//                            anchors.margins: 1.5 * parent.border.width
//                            anchors.fill: parent
//                            // text: "edit" // "edit" role of the model, to break the binding loop
//                            onTextChanged: model.display = text
//                        }
//                    }
//                }
//            }
//            Column {
//                id: column;
//                anchors.bottom: parent.bottom
//                Text { text: "Unit"; }
//                ComboBox {
//                    id: box2
//                    model: model2
//                    textRole: "display"
//                }
//                Text { text: "Type";  }
//                ComboBox {
//                    id: box1
//                    model: model1
//                    textRole: "display"
//                    onCurrentTextChanged: generator.generate(currentText)
//                }

//            }
//        }

//    }

//        function openDB() {
//            if(db !== null) return;

//            // db = LocalStorage.openDatabaseSync(identifier, version, description, estimated_size, callback(db))
//            db = LocalStorage.openDatabaseSync("my_db", "1.0", "Simple example app", 100000)

////            try {
////                db.transaction(function(tx){
////                    tx.executeSql('CREATE TABLE IF NOT EXISTS settings(key TEXT UNIQUE, value TEXT)')
////                    var table  = tx.executeSql("SELECT * FROM settings")
////                    // Seed the table with default values
////                    if (table.rows.length == 0) {
////                        tx.executeSql('INSERT INTO settings VALUES(?, ?)', ["distro", "Ubuntu"])
////                        tx.executeSql('INSERT INTO settings VALUES(?, ?)', ["foo", "Bar"])
////                        console.log('Settings table added')
////                    }
////                })
////            } catch (err) {
////                console.log("Error creating table in database: " + err)
////            }
//        }

//        function saveSetting(key, value) {
//            openDB();
//            db.transaction( function(tx){
//                tx.executeSql('INSERT OR REPLACE INTO test1 VALUES(?, ?)', [key, value])
//            })
//        }
//        function getSetting(key) {
//            openDB()
//            var res = ""
//            db.transaction(function(tx) {
//                var rs = tx.executeSql('SELECT value FROM test1 WHERE key=?;', [key])
//                res = rs.rows.item(0).value
//            })
//            return res
//        }

//}
