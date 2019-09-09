import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Dialog {
    id: addItemDialog
    modality: Qt.NonModal
    property string previewItem: ""
    signal addItem(string itemName)
    standardButtons: StandardButton.Cancel | StandardButton.Ok
    width: 600
    height: 400
    Row {
        anchors.fill: parent

        ScrollView {
            width: parent.width / 2
            height: parent.height
            clip: true
            ListView {
                model: panelItemModel
                delegate: Button {
                    text: name + " - " + description
                    checkable: true
                    checked: itemName === previewItem
                    onClicked: previewItem = itemName
                    width: parent.width * 0.9
                }
            }
        }
        Rectangle {
            color: "black"
            border.color: "yellow"
            width: parent.width / 2
            height: parent.height

            Loader {
                source: previewItem ? "qrc:///panelitems/" + previewItem + ".qml" : ""
                width: parent.width * 0.8
                height: parent.height * 0.8
                anchors.centerIn: parent
            }
        }
    }
    onAccepted: if(previewItem) addItem(previewItem)

    // Todo: Auto-generate this list from the directory contents.
    ListModel {
        id: panelItemModel
        ListElement {
            name: "Compass"
            itemName: "CompassBasic"
            description: "Simple digital compass"
        }
        ListElement {
            name: "Airspeed indicator"
            itemName: "needle/AirspeedIndicator"
            description: "Generic airspeed indicator"
        }
        ListElement {
            name: "Attitude indicator"
            itemName: "AttitudeIndicator"
            description: "Generic attitude indicator"
        }
        ListElement {
            name: "Button"
            itemName: "buttons/ButtonGeneric"
            description: "Generic button"
        }
        ListElement {
            name: "Toggle Switch"
            itemName: "switches/ToggleSwitch"
            description: "Generic toggle switch"
        }
        ListElement {
            name: "3-way Switch"
            itemName: "switches/ThreeWaySwitch"
            description: "Generic three-way switch"
        }
        ListElement {
            name: "APU EGT temperature"
            itemName: "needle/ApuEgt"
            description: "Shows APU exhaust gas temperature in c"
        }
        ListElement {
            name: "Fuel temperature (x737)"
            itemName: "needle/FuelTemp"
            description: "Shows fuel temperature. Only available in x737."
        }
        ListElement {
            name: "Annunciator"
            itemName: "annunciators/Annunciator"
            description: "Generic annunciator"
        }
        ListElement {
            name: "Label"
            itemName: "decoration/Label"
            description: "Label, static text and/or dataref value"
        }
        ListElement {
            name: "CDU"
            itemName: "efis/Cdu"
            description: "Control Display Unit (CDU)"
        }
    }
}
