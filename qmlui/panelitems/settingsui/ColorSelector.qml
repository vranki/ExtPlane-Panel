import QtQuick 2.0
import QtQuick.Controls 2.2

SpinBox {
    property var colors: []
    to: colors.length - 1
    value: settings.annunciatorColor;
    background: Rectangle { color: colors[value]; opacity: 0.5; anchors.fill: parent }
}
