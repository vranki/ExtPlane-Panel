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

echo "Building for linux.."

rm -rf build
qmake -r
make clean distclean
qmake -r
make
mkdir -p build
cp qmlui/extplane-panel build

make clean distclean

echo "Building debian package.."
make clean distclean
rm ../*.deb
dpkg-buildpackage -rfakeroot -b
cp ../*.deb build
make clean distclean

# Build for windows..
./scripts/cross-compile-win64-from-lin.sh

# Zip the results for release
cp qmlui/release/extplane-panel.exe build

pushd build
zip -r extplane-panel.zip extplane-panel extplane-panel.exe *.deb
popd
