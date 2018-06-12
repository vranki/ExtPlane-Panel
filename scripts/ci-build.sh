#!/bin/bash
#
# This script builds all versions of ExtPlane-anel if
# possible. Currently builds lin & win versions.
#
# Creates a zip package for release. 
#
# Run from ExtPlane-Panel directory.
#

# Build for linux first..
rm -rf build
qmake -r
make clean distclean
qmake -r
make
mkdir -p build
cp qmlui/qmlui build
cp widgetui/bin/extplane-panel build

make clean distclean

# Build for windows..
./scripts/cross-compile-win64-from-lin.sh

# Zip the results for release
cp widgetui/bin/extplane-panel.exe qmlui/qmlui.exe build

pushd build
zip -r extplane-panel.zip qmlui qmlui.exe extplane-panel extplane-panel.exe
popd

