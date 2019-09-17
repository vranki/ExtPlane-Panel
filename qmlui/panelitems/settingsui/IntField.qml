import QtQuick 2.0
import QtQuick.Controls 2.2

TextField {
    property int value: minValue
    property int minValue: 0
    property int maxValue: 9999999
    text: "0"
    inputMethodHints: Qt.ImhDigitsOnly
    onTextChanged: {
        var nv = parseInt(text) || minValue
        value = limitValue(nv, minValue, maxValue)
    }
}
