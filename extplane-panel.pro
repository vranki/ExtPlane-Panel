QT       += gui network

maemo5: {
    QT += dbus
    DEFINES += MAEMO
    DEFINES += MOBILE_DEVICE
}

android: {
    DEFINES += MOBILE_DEVICE
}

meego: {
    DEFINES += MOBILE_DEVICE
}

TARGET = extplane-panel
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
TEMPLATE = app
SOURCES += main.cpp \
    panelitems/panelitem.cpp \
    extplaneconnection.cpp \
    panelwindow.cpp \
    extplaneclient.cpp \
    clientdataref.cpp \
    clientdatarefprovicer.cpp \
    panelitems/airspeedindicator.cpp \
    units.cpp \
    menubutton.cpp \
    panelitemfactory.cpp \
    panelitems/needleinstrument.cpp \
    panelitems/compass.cpp \
    dialogs/settingsdialog.cpp \
    dialogs/edititemdialog.cpp \
    widgets/velocityunitcombobox.cpp \
    widgets/numberinputlineedit.cpp \
    panelitems/variometer.cpp \
    panelitems/altimeter.cpp \
    widgets/distanceunitcombobox.cpp \
    panelitems/button.cpp \
    panelitems/switch.cpp \
    simulateddatarefs/simulateddataref.cpp \
    valueinterpolator.cpp \
    panelitems/rotaryknob.cpp \
    simulatedextplaneconnection.cpp
HEADERS += panelitems/panelitem.h \
    extplaneconnection.h \
    panelwindow.h \
    extplaneclient.h \
    clientdataref.h \
    clientdatarefprovicer.h \
    panelitems/airspeedindicator.h \
    units.h \
    menubutton.h \
    panelitemfactory.h \
    panelitems/needleinstrument.h \
    panelitems/compass.h \
    dialogs/settingsdialog.h \
    dialogs/edititemdialog.h \
    widgets/velocityunitcombobox.h \
    widgets/numberinputlineedit.h \
    panelitems/variometer.h \
    panelitems/altimeter.h \
    widgets/distanceunitcombobox.h \
    panelitems/button.h \
    panelitems/switch.h \
    simulateddatarefs/simulateddataref.h \
    valueinterpolator.h \
    panelitems/rotaryknob.h \
    simulatedextplaneconnection.h
FORMS += dialogs/settingsdialog.ui \
    dialogs/edititemdialog.ui


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

OTHER_FILES += README
