# ExtPlane-Panel 2

An external, networked cockpit panel for flight simulators supported by
ExtPlane, such as X-Plane, Condor and FlightGear.

![Screenshot](/screenshots/extplane-panel.png?raw=true)

![CDU](/screenshots/737-cdu.jpg?raw=true)

License: GNU GPLv3

It is possible to run this panel on the same computer or another computer
on the network to display cockpit instruments from remote ExtPlane.

Almost all instruments are drawn using scalable vector graphics for
perfect scaling and high resolutions.

The application also allows running hardware instruments with X-Plane
datarefs. These are used to control servos using PWM. Other, more complex
output devices can be implemented later. Currently Chroma is the most
tested device but the others should also work.

The app is written using Qt and is platform independent - it runs on
different Linux variants, Mac and Windows. X-Plane SDK is NOT required
to build ExtPlane-Panel.

Target hardware is:
* Desktop PC's
* Android
* Raspberry Pi
* anything that will run Qt!

## Download

### ExtPlane Panel

Download at releases page:

https://github.com/vranki/ExtPlane-Panel/releases

Releases should contain

* Linux binary
* Windows binary
* Debian/Ubuntu .deb package
* Snap package for various distributions

Note: package version numbers are not updated when doing automated releases.

### ExtPlane Plugin

You'll need ExtPlane plugin for X-Plane from:

https://github.com/vranki/ExtPlane/releases/

See installation instructions at https://github.com/vranki/ExtPlane

## Instruments

### Instruments in ExtPlane-Panel 2

* Annunciator
* Button
* CDU
* GPS Map (Doesn't seem to work)
* HSI
* Generic Airspeed Indicator
* APU EGT
* Fuel Temperature
* Turn & Bank indicator
* Toggle switch (normal & 3-way)
* Attitude Indicator
* Compass (basic digital)
* Whisky compass
* Airspeed indicator
* Variometer
* Altimeter
* Turn & Slip indicator
* Slip indicator
* Direction indicator
* Engine RPM
* Multi-positon dial
* G Meter
* Fuel gauge
* Rotary knob
* Battery voltage
* Fuel pressure
* Image

Most instruments look like general aviation instruments and can be customized and adjusted.
Some can be used with airliners and some are specifically for x737.

You can use Image element to load any jpg/png/whatever file and show it in cockpit. This
allows having a bitmap background and/or foreground image in addition to the instrument displays.

### Compability with ExtPlane-Panel 1.x

The whole UI has been changed in version 2 from oldschool widgets to QML for
much quicker development. Some instruments have been rewritten in QML and
some wrap the old widget code. Here's a list of panel items and their statuses.

N = New rewrite in QML

W = Wrapped in QML

! = Not done, either rewrite or wrap

? = Might get removed

* N airspeedindicator -> AirspeedIndicator
* W attitudeindicator -> AttitudeIndicator
* N compass -> CompassBasic
* N dial -> Dial
* W enginedisplay (Does not work)
* N engine_rpm -> EngineRpm
* ? genericlittlegauge (Needs rewrite)
* W hsi_ks55 -> HsiK55
* N indicatorlight -> Annunciator
* W navdisplay (Does not work)
* W radarmap.h (Does not work)
* N slipindicator -> SlipIndicator
* N tankleveler -> FuelGauge
* N variometer -> Variometer
* N altimeter -> Altimeter
* N button -> ButtonGeneric
* N datarefdebugger -> Label (can show dr value)
* W di
* N engine_battery -> BatteryVoltage
* N engine_fuel_p -> FuelPressure
* N gaugecover -> Image
* W gpsmap -> GpsMap
* N indicatordisplay -> Annunciator
* W pfddisplay (Does not work)
* N rotaryknob -> RotaryKnob
* N switch -> ToggleSwitch
* W turnbank -> TurnBank

## Usage

Learn these keys:

* Tab - Show main menu
* Space - toggle edit mode
* s - save all panels
* f - toggle fullscreen mode
* a - Add new item
* Delete - delete selected item
* d - duplicate selected item (settings not copied yet yet)

### Edit mode

In edit mode you can drag and resize panel items with mouse.

Double click on item to open it's settings (if any available).

### Panels

You can have multiple panels numbered from 0 to 42.

You can modify your panel freely. Changes are *not* saved
automatically - you need to save the panel after editing.

TODO: Panels can be exported to a .ini file to be copied to
other computers or versioned.

### Z value

Panel items have z-value property. This is used to decide
which items are painted on top of each other. Set z to higher
to draw over other items.

### Command Line

Nothing yet.

## Building

ExtPlane-Panel uses the Qt Framework for graphics and cross-platform compatibility. Before building you'll
need to setup Qt 5 or greater and Qt Mobility to compile. You'll also need to check out the ExtPlane
plugin sources (https://github.com/vranki/ExtPlane) to the directory next to the ExtPlane-Panel directory.

The requirements for ExtPlane-Panel are as follows:
* C++ Toolchain
* Qt Framework 5
* ExtPlane (contains the reusable client Qt code)

### Linux

```bash
# 1: (Debian/Ubuntu) Install required libraries and tools (see debian/rules for up to date list)
sudo apt-get install git build-essential qtbase5-dev qt5-default qtmobility-dev debhelper cdbs devscripts

# 2: Download source code from GitHub and enter the ExtPlane directory
git clone https://github.com/vranki/ExtPlane.git
git clone https://github.com/vranki/ExtPlane-Panel.git
cd ExtPlane-Panel

# 3a: (Debian/Ubuntu) Build and install debian package
dpkg-buildpackage -rfakeroot -b
sudo dpkg -i ../extplane-panel*.deb

# 3b: (NOT Debian/Ubuntu) Build binary
qmake
make

```

### OS X
```bash
# 1: Install required libraries and tools
# Download and install X-Code and Developer Tools from http://developer.apple.com
# Download and install Qt5 from http://qt-project.org/downloads

# 2: Download source code from GitHub
git clone https://github.com/vranki/ExtPlane.git
git clone https://github.com/vranki/ExtPlane-Panel.git

# 3: Build project
cd ExtPlane-Panel
qmake
make

# Note: If qmake is not on your command path, add the Qt bin directory to your path using
# export.
```

### Windows
```bash
# 1: Install required libraries and tools
# Download and install Qt5 from http://qt-project.org/downloads
# Download and install Git from http://git-scm.com/downloads

# 2: Download source code from GitHub
git clone https://github.com/vranki/ExtPlane.git
git clone https://github.com/vranki/ExtPlane-Panel.git

# 3: Build project
cd ExtPlane-Panel
qmake
nmake

# Note: If you do not have a C++ compiler, you can install one of the following:
# MinGW version of Qt: http://qt-project.org/downloads
# Windows 7 SDK: http://www.microsoft.com/en-us/download/confirmation.aspx?id=8279
# Windows 8 SDK: http://msdn.microsoft.com/en-us/library/windows/desktop/hh852363.aspx
# Visual Studio Express: http://www.microsoft.com/visualstudio/eng/downloads#d-2012-express
```

### Docker

If you can run docker, you can build both win & lin versions with these
commands:
```bash
# Build docker image for compiling
docker build -t extplane .
# Run the continous integration build script
docker run -it -v $PWD:/ExtPlane-Panel -w /ExtPlane-Panel extplane ./scripts/ci-build.sh
# Now you should have output/extplane-panel.zip
```


## Creating new Panel Items

See wiki page at:

https://github.com/vranki/ExtPlane-Panel/wiki/Implementing-Panel-Items

Code should be reasonably well documented. If something is not clear, please
report it.

GPL licence does not *require* you to release any changes you do to this
application if you only use them yourself. Despite this we *ask* you to
release any new instruments you create or any other improvements you make
into the application.

Preferred way is to create a fork in github and send a pull request when
you are finished. If you don't want to do this, we're happy with any
other way to deliver the changes.

## Contact / Feedback

Original Author:
- Ville Ranki <ville.ranki@iki.fi>

Contributors:
- Dan Krusi <dan.krusi@nerves.ch>
- Bob Gates
- Nicolas Montarnal

## Resources used

* B612 font by Airbus - see details and license at https://b612-font.com/
