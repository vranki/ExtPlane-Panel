# Qt Modules
QT += gui network widgets

# Widgets only gives warning on Qt 4.x.

# Qt Mobility
CONFIG += mobility
MOBILITY += systeminfo

# This doesn't seem to work; whines "Cannot find feature mobilityconfig"
# load(mobilityconfig)
contains(MOBILITY_VERSION, 1.1.1) {
    DEFINES += QTMOBILITY
}

# Place ExtPlane plugin to a directory next to or inside build directory, or
# define the directory here:
EXTPLANE_PLUGIN_PATH=../ExtPlane
!exists($$EXTPLANE_PLUGIN_PATH/client/extplane-client-qt) {
	EXTPLANE_PLUGIN_PATH=ExtPlane
	!exists($$EXTPLANE_PLUGIN_PATH/client/extplane-client-qt) {
		error("You don't have ExtPlane checked out in directory next to this. Place it there or build will fail.")
	}
}

EXTPLANE_CLIENT_PATH=$$EXTPLANE_PLUGIN_PATH/client/extplane-client-qt
INCLUDEPATH += $$EXTPLANE_CLIENT_PATH

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

maemo5: { 
    QT += dbus
    DEFINES += MAEMO
    DEFINES += MOBILE_DEVICE
}
exists(/etc/maemo_version) { 
    QT += dbus
    DEFINES += MAEMO
    DEFINES += MOBILE_DEVICE
}

android::DEFINES += MOBILE_DEVICE
meego::DEFINES += MOBILE_DEVICE

ICON = extplane-panel.icns
TARGET = extplane-panel
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
TEMPLATE = app

target.path = /usr/bin

INSTALLS += target

desktop.files = extplane-panel.desktop
desktop.path = /usr/share/applications

unix: INSTALLS += desktop

SOURCES += \
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
    units.cpp \
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
    valueinterpolator.cpp \
    panelitems/rotaryknob.cpp \
    panelitemfactory.cpp \
    needles/needle.cpp \
    needles/triangleneedle.cpp \
    needles/gabalancedneedle.cpp \
    dialogs/panelitemselectiondialog.cpp \
    panelitems/slipindicator.cpp \
    dialogs/hardwaredialog.cpp \
    hardware/hardwarebinding.cpp \
    hardware/hardwaremanager.cpp \
    settings.cpp \
    hardware/outputdevice.cpp \
    hardware/servoblasteroutputdevice.cpp \
    hardware/pololuoutputdevice.cpp \
    panelitems/gpsmap.cpp \
    panelitems/radarmap.cpp \
    panelitems/mapinstrument.cpp \
    panelitems/datarefdebugger.cpp \
    panelitems/gaugecover.cpp \
    panel.cpp \
    panelitems/indicatorlight.cpp \
    widgets/colorselector.cpp \
    util/perlinnoise.cpp \
    panelitems/enginedisplay.cpp \
    panelitems/displayinstrument.cpp \
    panelitems/navdisplay.cpp \
    hardware/chromaoutputdevice.cpp \
    dialogs/panelsdialog.cpp \
    panelitems/dial.cpp \
    dialogs/bindingcurvedialog.cpp \
    util/interpolation.cpp \
    hardware/nulloutputdevice.cpp \
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
    panelitems/panelitem.h \
    panelwindow.h \
    panelitems/airspeedindicator.h \
    units.h \
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
    valueinterpolator.h \
    panelitems/rotaryknob.h \
    panelitemfactory.h \
    needles/needle.h \
    needles/triangleneedle.h \
    needles/gabalancedneedle.h \
    dialogs/panelitemselectiondialog.h \
    panelitems/slipindicator.h \
    dialogs/hardwaredialog.h \
    hardware/hardwarebinding.h \
    hardware/hardwaremanager.h \
    settings.h \
    hardware/outputdevice.h \
    hardware/servoblasteroutputdevice.h \
    hardware/pololuoutputdevice.h \
    panelitems/gpsmap.h \
    panelitems/radarmap.h \
    panelitems/mapinstrument.h \
    panelitems/datarefdebugger.h \
    panelitems/gaugecover.h \
    panel.h \
    panelitems/indicatorlight.h \
    widgets/colorselector.h \
    util/perlinnoise.h \
    util/console.h \
    panelitems/enginedisplay.h \
    panelitems/displayinstrument.h \
    panelitems/navdisplay.h \
    hardware/chromaoutputdevice.h \
    dialogs/panelsdialog.h \
    panelitems/dial.h \
    dialogs/bindingcurvedialog.h \
    util/interpolation.h \
    hardware/nulloutputdevice.h \
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

# Lot of files for Android build, ignore them if not used
OTHER_FILES += android/AndroidManifest.xml \
    android/res/values-ja/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-de/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-nb/strings.xml \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/values-rs/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/values-ru/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-es/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-ms/strings.xml \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/version.xml
OTHER_FILES += README.md debian/rules debian/changelog debian/control debian/extplane-panel.install debian/extplane-panel-dbg.install


RESOURCES += \
    applicationresources.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
