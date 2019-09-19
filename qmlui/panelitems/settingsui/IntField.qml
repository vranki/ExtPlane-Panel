import QtQuick 2.0
import QtQuick.Controls 2.2

TextField {
    property int value: minValue
    property int minValue: 0
    property int maxValue: 9999999
    text: "0"
    inputMethodHints: Qt.ImhDigitsOnly
    onTextChanged: {
        var nv = parseInt(text)
        value = limitValue(nv, minValue, maxValue)
        if(parseInt(text) !== value) text = value
    }
    Button {
        id: downButton
        anchors.right: parent.right
        width: height
        height: parent.height
        text: "-"
        onClicked: parent.text = value - 1
    }
    Button {
        id: upButton
        anchors.right: downButton.left
        width: height
        height: parent.height
        text: "+"
        onClicked: parent.text = value + 1
    }
}
