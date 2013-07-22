# This script creates a package for OS-X Platform with all dependencies.
# It is based on http://qt-project.org/doc/qt-5.0/qtdoc/deployment-mac.html
# This is what it does:
#    * creates a package folder for the version and platform specified below
#    * copies ExtPlane resources
#    * patches the application for stand-alone Qt
#    * creates a DMG file of the whole thing and cleans up
# The results are a package that contains everything needed to run ExtPlane on a fresh
# install of OS X.

# Config
VERSION="0.1"
PLATFORM="osx64"
QT_BIN_PATH="/Users/dankrusi/Qt5.0.1/5.0.1/clang_64/bin"
EXTPLANE_PROJECT_PATH="/Users/dankrusi/Code/ExtPlane-Panel"
EXTPLANE_BIN_PATH="/Users/dankrusi/Code/extplane-panel-build-Desktop_Qt_5_0_1_clang_64bit-Default_build_Release/bin"
PACKAGE_PATH="/Users/dankrusi/Code/ExtPlane-Packages/ExtPlane-Panel-v$VERSION-$PLATFORM"

# Show config
echo " "
echo "VERSION = $VERSION";
echo "PLATFORM = $PLATFORM";
echo "QT_BIN_PATH = $QT_LIB_PATH";
echo "EXTPLANE_PROJECT_PATH = $EXTPLANE_PROJECT_PATH";
echo "EXTPLANE_BIN_PATH = $EXTPLANE_BIN_PATH";
echo "PACKAGE_PATH = $PACKAGE_PATH";

# Verify
echo " "
echo "Press any key to continue"
echo " "
read

echo "Copying ExtPlane resources..."
rm -rf $PACKAGE_PATH
mkdir -p $PACKAGE_PATH
cp -Rf $EXTPLANE_BIN_PATH/extplane-panel.app $PACKAGE_PATH/ExtPlane-Panel.app
cp -Rf $EXTPLANE_PROJECT_PATH/README.md $PACKAGE_PATH

echo "Patching Frameworks..."
$QT_BIN_PATH/macdeployqt $PACKAGE_PATH/ExtPlane-Panel.app

echo "Creating DMG..."
rm -rf $PACKAGE_PATH-writable.dmg
rm -rf $PACKAGE_PATH.dmg
hdiutil create $PACKAGE_PATH-writable.dmg -ov -volname "ExtPlane-Panel" -fs HFS+ -srcfolder "$PACKAGE_PATH"
hdiutil convert $PACKAGE_PATH-writable.dmg -format UDZO -o $PACKAGE_PATH.dmg

echo "Cleaning up..."
rm -rf $PACKAGE_PATH-writable.dmg
rm -rf $PACKAGE_PATH

echo "Done."

