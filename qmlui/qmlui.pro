include(../common.pri)

QT += quick widgets xml svg
QTPLUGIN += qsvg

TARGET = extplane-panel

#CONFIG   -= debug_and_release
#CONFIG += debug

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    extplaneutilities.cpp \
    paintedpanelitem.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

target.path = /usr/bin

INSTALLS += target

desktop.files = ../extplane-panel.desktop
desktop.path = /usr/share/applications

unix: INSTALLS += desktop


HEADERS += \
    extplaneutilities.h \
    paintedpanelitem.h


SOURCES += \
     ../widgetui/panelitems/panelitem.cpp \
     ../widgetui/panelitems/attitudeindicator.cpp \
     ../widgetui/panelitems/mapinstrument.cpp \
     ../widgetui/panelitems/gpsmap.cpp \
     ../widgetui/panelitems/hsi_ks55.cpp \
     ../widgetui/panelitems/di.cpp \
     ../widgetui/panelitems/displayinstrument.cpp \
     ../widgetui/panelitems/enginedisplay.cpp \
     ../widgetui/panelitems/navdisplay.cpp \
     ../widgetui/panelitems/pfddisplay.cpp \
     ../widgetui/panelitems/radarmap.cpp \
     ../util/units.cpp \
     ../widgetui/panelitems/turnbank.cpp \
     ../widgetui/widgets/numberinputlineedit.cpp \
     ../widgetui/widgets/colorselector.cpp \
     ../widgetui/panelitemfactory.cpp

HEADERS += \
     ../widgetui/panelitems/panelitem.h \
     ../widgetui/panelitems/attitudeindicator.h \
     ../widgetui/panelitems/mapinstrument.h \
     ../widgetui/panelitems/gpsmap.h \
     ../widgetui/panelitems/hsi_ks55.h \
     ../widgetui/panelitems/di.h \
     ../widgetui/panelitems/displayinstrument.h \
     ../widgetui/panelitems/enginedisplay.h \
     ../widgetui/panelitems/navdisplay.h \
     ../widgetui/panelitems/pfddisplay.h \
     ../widgetui/panelitems/radarmap.h \
     ../util/units.h \
     ../widgetui/panelitems/turnbank.h \
     ../widgetui/widgets/numberinputlineedit.h \
     ../widgetui/widgets/colorselector.h \
     ../widgetui/panelitemfactory.h
