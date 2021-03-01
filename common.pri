CONFIG += c++11

QT += quick

# Place ExtPlane plugin to a directory next to or inside build directory, or
# define the directory here:

# Snapcraft
# HACKHACKHACK: SNAPCRAFT_PROJECT_DIR doesn't seem to work, so we just do it the ugly way
#EXTPLANE_PLUGIN_PATH=$$(SNAPCRAFT_PROJECT_DIR)/extplane/src
EXTPLANE_PLUGIN_PATH=$$(PWD)/../../extplane/src

!exists($$EXTPLANE_PLUGIN_PATH/clients/extplane-client-qt) {
        EXTPLANE_PLUGIN_PATH=$$absolute_path(../../ExtPlane)
        !exists($$EXTPLANE_PLUGIN_PATH/clients/extplane-client-qt) {
                EXTPLANE_PLUGIN_PATH=$$absolute_path(../ExtPlane)
                !exists($$EXTPLANE_PLUGIN_PATH/clients/extplane-client-qt) {
                       error("You don't have ExtPlane checked out in directory next to this. Place it there or build will fail.")
                }
        }
}
message(plugin path $$EXTPLANE_PLUGIN_PATH)

EXTPLANE_CLIENT_PATH=$$EXTPLANE_PLUGIN_PATH/clients/extplane-client-qt

include($$EXTPLANE_CLIENT_PATH/extplane-client-qt.pri)
