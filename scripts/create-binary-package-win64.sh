
# This script creates a package for Windows Platform with all dependencies.
# It is based on http://qt-project.org/doc/qt-5.0/qtdoc/deployment-windows.html
# This is what it does:
#    * creates a package folder for the version and platform specified below
#    * copies ExtPlane resources
#    * copies needed Qt resources (dlls)
#    * copies needed Qt plugins
#    * copies needed MSVC plugins
#    * creates a ZIP file of the whole thing and cleans up
# The results are a package that contains everything needed to run ExtPlane on a fresh
# install of Windows.

# Package config
VERSION="0.1"
PLATFORM="win64"

# Tools config
MSVC_PATH="resources"
QT_BIN_PATH="C:/Qt/Qt5.0.2/5.0.2/msvc2012_64/bin"
QT_PLUGIN_PATH="C:/Qt/Qt5.0.2/5.0.2/msvc2012_64/plugins"
EXTPLANE_PROJECT_PATH="C:/Users/admin/ExtPlane-Panel"
EXTPLANE_BIN_PATH="C:/Users/admin/build-extplane-panel-Qt5_64bit-Release/bin"
EXTPLANE_PACKAGE_PATH="C:/Users/admin/ExtPlane-Packages/ExtPlane-Panel-v$VERSION-$PLATFORM"

# Show config
echo " "
echo "VERSION = $VERSION";
echo "PLATFORM = $PLATFORM";
echo "MSVC_PATH = $MSVC_PATH";
echo "QT_BIN_PATH = $QT_BIN_PATH";
echo "QT_PLUGIN_PATH = $QT_PLUGIN_PATH";
echo "EXTPLANE_BIN_PATH = $EXTPLANE_BIN_PATH";
echo "EXTPLANE_PACKAGE_PATH = $EXTPLANE_PACKAGE_PATH";

# Verify
echo " "
echo "Press any key to continue"
echo " "
read

echo "Creating package folder..."
rm -Rf $EXTPLANE_PACKAGE_PATH
mkdir -p $EXTPLANE_PACKAGE_PATH

echo "Copying ExtPlane-Panel resources..."
cp -f $EXTPLANE_BIN_PATH/extplane-panel.exe $EXTPLANE_PACKAGE_PATH
cp -f $EXTPLANE_BIN_PATH/extplane-panel.exe.embed.manifest $EXTPLANE_PACKAGE_PATH
cp -f $EXTPLANE_PROJECT_PATH/README.md $EXTPLANE_PACKAGE_PATH/README.txt

echo "Copying Qt resources..."
cp -f $QT_BIN_PATH/Qt5Widgets.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/Qt5Gui.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/Qt5Core.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/Qt5Network.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/libGLESv2.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/libEGL.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/d3dcompiler_46.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/icuin49.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/icuuc49.dll $EXTPLANE_PACKAGE_PATH
cp -f $QT_BIN_PATH/icudt49.dll $EXTPLANE_PACKAGE_PATH

echo "Copying Qt plugins..."
mkdir -p $EXTPLANE_PACKAGE_PATH/platforms
cp -f $QT_PLUGIN_PATH/platforms/qminimal.dll $EXTPLANE_PACKAGE_PATH/platforms
cp -f $QT_PLUGIN_PATH/platforms/qwindows.dll $EXTPLANE_PACKAGE_PATH/platforms

echo "Copying MSVC resources..."
cp -f $MSVC_PATH/msvcp110.dll.x86_64 $EXTPLANE_PACKAGE_PATH/msvcp110.dll
cp -f $MSVC_PATH/msvcr110.dll.x86_64 $EXTPLANE_PACKAGE_PATH/msvcr110.dll

echo "Creating archive..."
rm -f $EXTPLANE_PACKAGE_PATH.zip
tools/7za.exe a $EXTPLANE_PACKAGE_PATH.zip $EXTPLANE_PACKAGE_PATH

echo "Removing package folder..."
rm -Rf $EXTPLANE_PACKAGE_PATH
