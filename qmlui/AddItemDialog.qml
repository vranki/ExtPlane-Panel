import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Dialog {
    id: addItemDialog
    modality: Qt.NonModal
    property string previewItem: null
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
                onStatusChanged: console.log("Loader state", status)
            }
        }
    }
    onAccepted: {
        if(previewItem) {
            // Destroy the preview before adding
            var newItem = previewItem
            previewItem = null
            addItem(newItem)
        }
    }
    onVisibleChanged: previewItem = null

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
        ListElement {
            name: "GPS Map"
            itemName: "efis/GpsMap"
            description: "Generic GPS map"
        }
        ListElement {
            name: "HSI"
            itemName: "hsi/HsiK55"
            description: "HSI KS55"
        }
        ListElement {
            name: "Turn & Bank indicator"
            itemName: "needle/TurnBank"
            description: "Generic turn & bank indicator"
        }
        ListElement {
            name: "Variometer"
            itemName: "needle/Variometer"
            description: "Generic variometer, with total energy support"
        }
        ListElement {
            name: "Altimeter"
            itemName: "needle/Altimeter"
            description: "Generic altimeter, in feet or meters"
        }
        ListElement {
            name: "Slip indicator"
            itemName: "SlipIndicator"
            description: "Generic slip indicator. Ball in a tube."
        }
        ListElement {
            name: "Turn & Slip indicator"
            itemName: "TurnAndSlip"
            description: "Turn and slip indicator."
        }
    }
}
