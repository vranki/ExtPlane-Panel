import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
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
                id: itemListView
                property string filter: ""
                model: panelItemModel
                header: TextField {
                    id: filterText
                    placeholderText: "Search"
                    onTextChanged: itemListView.filter = text
                    Layout.fillWidth: true
                }
                delegate: Button {
                    property bool shown: itemListView.filter.length === 0 || name.toLowerCase().includes(itemListView.filter.toLowerCase())
                    text: name + " - " + description
                    checkable: true
                    checked: itemName === previewItem
                    onClicked: previewItem = itemName
                    width: parent.width * 0.9
                    height: shown ? Button.height : 0
                }
            }
        }
        Rectangle {
            color: "black"
            border.color: "yellow"
            width: parent.width / 2
            height: parent.height

            Loader {
                onLoaded: {
                    item.x = 0
                    item.y = 0
                }
                source: previewItem ? "qrc:///panelitems/" + previewItem + ".qml" : ""
                active: previewItem !== ""
                width: parent.width * 0.8
                height: parent.height * 0.8
                anchors.centerIn: parent
            }
        }
    }
    onAccepted: {
        if(previewItem) {
            // Destroy the preview before adding
            var newItem = previewItem
            previewItem = ""
            addItem(newItem)
        }
    }
    onVisibleChanged: previewItem = ""

    // Todo: Auto-generate this list from the directory contents.
    ListModel {
        id: panelItemModel

        ListElement {
            name: "Compass"
            itemName: "CompassBasic"
            description: "Simple digital compass"
        }
        ListElement {
            name: "Whisky Compass"
            itemName: "CompassWhisky"
            description: "Wet compass used in GA aircraft"
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
        ListElement {
            name: "Direction indicator"
            itemName: "hsi/DirectionIndicator"
            description: "Generic direction indicator."
        }
        ListElement {
            name: "Engine display"
            itemName: "efis/EngineDisplay"
            description: "Displays the engine N1, EPR and EGT for any number of engines for Glass Cockpit."
        }
        ListElement {
            name: "Engine RPM"
            itemName: "needle/EngineRpm"
            description: "Engine RPM gauge."
        }
        ListElement {
            name: "Navigation display"
            itemName: "efis/NavDisplay"
            description: "Navigation display."
        }
        ListElement {
            name: "Radar map"
            itemName: "efis/RadarMap"
            description: "Radar map."
        }
        ListElement {
            name: "Primary Flight Display (PFD)"
            itemName: "efis/Pfd"
            description: "EFIS-style Primary Flight Display (PFD)."
        }
        ListElement {
            name: "Dial"
            itemName: "switches/Dial"
            description: "Multi-position dial. Can be freely configured."
        }
        ListElement {
            name: "G Meter"
            itemName: "needle/GMeter"
            description: "G Meter, needle type"
        }
        ListElement {
            name: "Fuel gauge"
            itemName: "needle/FuelGauge"
            description: "Shows fuel capacity in selected tank in %"
        }
        ListElement {
            name: "Rotary knob"
            itemName: "switches/RotaryKnob"
            description: "Generic knob you can rotate to change a dataref."
        }
        ListElement {
            name: "Battery voltage"
            itemName: "needle/BatteryVoltage"
            description: "Shows battery voltage for selected battery."
        }
        ListElement {
            name: "Fuel pressure"
            itemName: "needle/FuelPressure"
            description: "Shows fuel pressure."
        }
        ListElement {
            name: "Image"
            itemName: "decoration/ImageItem"
            description: "Can show an image file."
        }
        ListElement {
            name: "Needle only gauge"
            itemName: "needle/GaugeLess"
            description: "Shows an arbitrary gauge."
        }
        ListElement {
            name: "B738 generic cap switch"
            itemName: "switches/B738GenericCap"
            description: "Caps that are not backed by an spcecific dataref."
        }
        ListElement {
            name: "Dual Annunciator"
            itemName: "annunciators/DualAnnunciator"
            description: "It has three states off, dim and bright"
        }
    }
}
