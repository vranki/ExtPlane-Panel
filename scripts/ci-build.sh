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

rm -rf output
mkdir -p output

echo "Building for linux.."

qmake -r
make clean distclean
qmake -r
make
cp qmlui/extplane-panel output

make clean distclean

echo "Building debian package.."
make clean distclean
rm ../*.deb
dpkg-buildpackage -rfakeroot -b


echo "Debian packages built and should be in ..:"
ls ..
cp -v ../*.deb output
echo "Output dir now:"
ls output
make clean distclean

echo "Building snap.."
rm -rf parts prime stage snap
snapcraft
cp *.snap output

# Build for windows..
./scripts/cross-compile-win64-from-lin.sh

# Zip the results for release
cp qmlui/release/extplane-panel.exe output

pushd output
zip -r extplane-panel.zip extplane-panel extplane-panel.exe *.deb *.snap
popd

# Clean up
make clean distclean
