QT       += gui network

maemo5: {
 QT += dbus
 DEFINES += MAEMO
}

TARGET = extplane-panel

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
    panelitems/button.cpp

HEADERS += \
    panelitems/panelitem.h \
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
    panelitems/button.h

FORMS += \
    dialogs/settingsdialog.ui \
    dialogs/edititemdialog.ui
