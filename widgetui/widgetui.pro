TEMPLATE = app
TARGET = extplane-panel

# Qt Modules
QT += gui network widgets

# Qt Mobility, used to disable screensaver
CONFIG += mobility
CONFIG += c++11
MOBILITY += systeminfo

contains(MOBILITY_VERSION, 1.1.1) {
    DEFINES += QTMOBILITY
}

# Place ExtPlane plugin to a directory next to or inside build directory, or
# define the directory here:
EXTPLANE_PLUGIN_PATH=../../ExtPlane
!exists($$EXTPLANE_PLUGIN_PATH/clients/extplane-client-qt) {
        EXTPLANE_PLUGIN_PATH=../ExtPlane
        !exists($$EXTPLANE_PLUGIN_PATH/clients/extplane-client-qt) {
		error("You don't have ExtPlane checked out in directory next to this. Place it there or build will fail.")
	}
}

EXTPLANE_CLIENT_PATH=$$EXTPLANE_PLUGIN_PATH/clients/extplane-client-qt
INCLUDEPATH += $$EXTPLANE_CLIENT_PATH
#INCLUDEPATH += $$EXTPLANE_PLUGIN_PATH/util

CONFIG(debug, debug|release) {
    # Debug
} else {
    # Release
    DEFINES += QT_NO_DEBUG
    DEFINES += QT_NO_DEBUG_OUTPUT
}

unix: {
    DEFINES += TERMIOS_AVAILABLE
}

android::DEFINES += MOBILE_DEVICE

ICON = ../extplane-panel.icns
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build

target.path = /usr/bin

INSTALLS += target

desktop.files = ../extplane-panel.desktop
desktop.path = /usr/share/applications

unix: INSTALLS += desktop

SOURCES += \
    $$EXTPLANE_PLUGIN_PATH/util/basictcpclient.cpp \
    $$EXTPLANE_CLIENT_PATH/extplaneconnection.cpp \
    $$EXTPLANE_CLIENT_PATH/extplaneclient.cpp \
    $$EXTPLANE_CLIENT_PATH/clientdataref.cpp \
    $$EXTPLANE_CLIENT_PATH/clientdatarefprovider.cpp \
    $$EXTPLANE_CLIENT_PATH/simulateddatarefs/simulateddataref.cpp \
    $$EXTPLANE_CLIENT_PATH/simulateddatarefs/fixedsimulateddataref.cpp \
    $$EXTPLANE_CLIENT_PATH/simulateddatarefs/alternatingsimulateddataref.cpp \
    $$EXTPLANE_CLIENT_PATH/simulatedextplaneconnection.cpp \
    main.cpp \
    panelitems/panelitem.cpp \
    panelwindow.cpp \
    panelitems/airspeedindicator.cpp \
    menubutton.cpp \
    panelitems/needleinstrument.cpp \
    panelitems/compass.cpp \
    dialogs/settingsdialog.cpp \
    dialogs/edititemdialog.cpp \
    widgets/velocityunitcombobox.cpp \
    widgets/numberinputlineedit.cpp \
    panelitems/variometer.cpp \
    panelitems/altimeter.cpp \
    panelitems/di.cpp \
    panelitems/attitudeindicator.cpp \
    panelitems/turnbank.cpp \
    panelitems/hsi_ks55.cpp \
    panelitems/engine_rpm.cpp \
    widgets/distanceunitcombobox.cpp \
    panelitems/button.cpp \
    panelitems/switch.cpp \
    panelitems/rotaryknob.cpp \
    panelitemfactory.cpp \
    needles/needle.cpp \
    needles/triangleneedle.cpp \
    needles/gabalancedneedle.cpp \
    dialogs/panelitemselectiondialog.cpp \
    panelitems/slipindicator.cpp \
    dialogs/hardwaredialog.cpp \
    panelitems/gpsmap.cpp \
    panelitems/radarmap.cpp \
    panelitems/mapinstrument.cpp \
    panelitems/datarefdebugger.cpp \
    panelitems/gaugecover.cpp \
    panel.cpp \
    panelitems/indicatorlight.cpp \
    widgets/colorselector.cpp \
    panelitems/enginedisplay.cpp \
    panelitems/displayinstrument.cpp \
    panelitems/navdisplay.cpp \
    dialogs/panelsdialog.cpp \
    panelitems/dial.cpp \
    dialogs/bindingcurvedialog.cpp \
    ../settings.cpp \
    ../util/units.cpp \
    ../util/valueinterpolator.cpp \
    ../util/perlinnoise.cpp \
    ../util/interpolation.cpp \
    ../hardware/nulloutputdevice.cpp \
    ../hardware/chromaoutputdevice.cpp \
    ../hardware/outputdevice.cpp \
    ../hardware/servoblasteroutputdevice.cpp \
    ../hardware/pololuoutputdevice.cpp \
    ../hardware/hardwarebinding.cpp \
    ../hardware/hardwaremanager.cpp \
    panelitems/engine_fuel_p.cpp \
    panelitems/tankleveler.cpp \
    panelitems/engine_battery.cpp \
    panelitems/genericlittlegauge.cpp \
    panelitems/pfddisplay.cpp \
    panelitems/indicatordisplay.cpp

HEADERS += \
    $$EXTPLANE_CLIENT_PATH/extplaneconnection.h \
    $$EXTPLANE_CLIENT_PATH/extplaneclient.h \
    $$EXTPLANE_CLIENT_PATH/clientdataref.h \
    $$EXTPLANE_CLIENT_PATH/clientdatarefprovider.h \
    $$EXTPLANE_CLIENT_PATH/simulateddatarefs/simulateddataref.h \
    $$EXTPLANE_CLIENT_PATH/simulateddatarefs/fixedsimulateddataref.h \
    $$EXTPLANE_CLIENT_PATH/simulateddatarefs/alternatingsimulateddataref.h \
    $$EXTPLANE_CLIENT_PATH/simulatedextplaneconnection.h \
    $$EXTPLANE_PLUGIN_PATH/util/basictcpclient.h \
    panelitems/panelitem.h \
    panelwindow.h \
    panelitems/airspeedindicator.h \
    menubutton.h \
    panelitems/needleinstrument.h \
    panelitems/compass.h \
    dialogs/settingsdialog.h \
    dialogs/edititemdialog.h \
    widgets/velocityunitcombobox.h \
    widgets/numberinputlineedit.h \
    panelitems/variometer.h \
    panelitems/altimeter.h \
    panelitems/di.h \
    panelitems/attitudeindicator.h \
    panelitems/turnbank.h \
    panelitems/engine_rpm.h \
    panelitems/hsi_ks55.h \
    widgets/distanceunitcombobox.h \
    panelitems/button.h \
    panelitems/switch.h \
    panelitems/rotaryknob.h \
    panelitemfactory.h \
    needles/needle.h \
    needles/triangleneedle.h \
    needles/gabalancedneedle.h \
    dialogs/panelitemselectiondialog.h \
    panelitems/slipindicator.h \
    dialogs/hardwaredialog.h \
    panelitems/gpsmap.h \
    panelitems/radarmap.h \
    panelitems/mapinstrument.h \
    panelitems/datarefdebugger.h \
    panelitems/gaugecover.h \
    panel.h \
    panelitems/indicatorlight.h \
    widgets/colorselector.h \
    panelitems/enginedisplay.h \
    panelitems/displayinstrument.h \
    panelitems/navdisplay.h \
    dialogs/panelsdialog.h \
    panelitems/dial.h \
    dialogs/bindingcurvedialog.h \
    ../settings.h \
    ../util/valueinterpolator.h \
    ../util/units.h \
    ../util/perlinnoise.h \
    ../util/console.h \
    ../util/interpolation.h \
    ../hardware/nulloutputdevice.h \
    ../hardware/chromaoutputdevice.h \
    ../hardware/outputdevice.h \
    ../hardware/servoblasteroutputdevice.h \
    ../hardware/pololuoutputdevice.h \
    ../hardware/hardwarebinding.h \
    ../hardware/hardwaremanager.h \
    panelitems/engine_fuel_p.h \
    panelitems/tankleveler.h \
    panelitems/engine_battery.h \
    panelitems/genericlittlegauge.h \
    panelitems/pfddisplay.h \
    panelitems/indicatordisplay.h

FORMS += dialogs/settingsdialog.ui \
    dialogs/edititemdialog.ui \
    dialogs/panelitemselectiondialog.ui \
    dialogs/hardwaredialog.ui \
    dialogs/panelsdialog.ui \
    dialogs/bindingcurvedialog.ui



RESOURCES += ../applicationresources.qrc
