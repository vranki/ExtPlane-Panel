# ExtPlane-Panel 2 #

![Screenshot](http://s23.postimg.org/8xhypdei3/extplane_panel_screenshot_v2.png)

An external, networked cockpit panel for flight simulators supported by
ExtPlane, such as X-Plane, Condor and FlightGear.

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

## Download ##

### Ext-Plane Plugin + Panel ###

TODO: Check travis builds

## Instruments ##

### Instruments in ExtPlane-Panel 2.0

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
* Compass
* Airspeed indicator
* Variometer
* Altimeter
* Turn & Slip indicator
* Slip indicator
* Direction indicator
* Engine RPM
* Multi-positon dial

Most instruments look like general aviation instruments and can be customized and adjusted.
You can also add and customize covers for your instruments. In addition, there are also
debugging tools which can help when developing with the X-Plane API.

### Compability with ExtPlane-Panel 1.x

The whole UI has been changed in version 2 from oldschool widgets to QML for
much quicker development. Some instruments have been rewritten in QML and
some wrap the old widget code. Here's a list of panel items and their statuses.

N = New rewrite in QML
W = Wrapped in QML
! = Not done, either rewrite or wrap
? = Might get removed

N airspeedindicator -> AirspeedIndicator
W attitudeindicator -> AttitudeIndicator
N compass -> CompassBasic
N dial -> Dial
W enginedisplay (Does not work)
N engine_rpm -> EngineRpm
? genericlittlegauge (Needs rewrite)
W hsi_ks55 -> HsiK55
N indicatorlight -> Annunciator
W navdisplay (Does not work)
W radarmap.h (Does not work)
N slipindicator -> SlipIndicator
! tankleveler (Needs rewrite)
N variometer -> Variometer
N altimeter -> Altimeter
N button -> ButtonGeneric
! datarefdebugger (Needs rewrite)
W di
! engine_battery
! engine_fuel_p
! gaugecover
W gpsmap -> GpsMap
N indicatordisplay -> Annunciator
W pfddisplay (Does not work)
! rotaryknob
N switch -> ToggleSwitch
W turnbank -> TurnBank

If your favorite instrument is missing, don't upgrade yet. If you're a coder,
please code it.

## Usage ##

Learn these keys:

* Tab - Show main menu
* Space - toggle edit mode
* s - save panels
* f - go to fullscreen mode
* a - Add new item
* Delete - delete selected item

In edit mode you can drag and resize panel items.

Double click on item to open it's settings (if any available).

### Panels ###

You can have multiple panels numbered from 0 to 42.

You can modify your panel freely. Changes are *not* saved
automatically - you need to save the panel after editing.

TODO: Panels can be exported to a .ini file to be copied to
other computers or versioned.

### Command Line ###

TODO: implement

### Keyboard Shortcuts ###

* **Space** toggles edit mode
* **F** sets fullscreen mode
* **Delete** deletes selected panel items
* **Left** selects the previous panel (if any)
* **Right** selects the next panel (if any)

## Building ##

ExtPlane-Panel uses the Qt Framework for graphics and cross-platform compatibility. Before building you'll
need to setup Qt 5 or greater and Qt Mobility to compile. You'll also need to check out the ExtPlane
plugin sources (https://github.com/vranki/ExtPlane) to the directory next to the ExtPlane-Panel directory.

The requirements for ExtPlane-Panel are as follows:
* C++ Toolchain
* Qt Framework 5
* ExtPlane (contains the reusable client Qt code)

### Linux ###

```bash
# 1: (Debian/Ubuntu) Install required libraries and tools
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

### OS X ###
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

### Windows ###
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

## Creating new Panel Items ##

TODO: Document

Code should be reasonably well documented. If something is not clear, please
report it.

GPL licence does not *require* you to release any changes you do to this
application if you only use them yourself. Despite this we *ask* you to
release any new instruments you create or any other improvements you make
into the application.

Preferred way is to create a fork in github and send a pull request when
you are finished. If you don't want to do this, we're happy with any
other way to deliver the changes.

## Contact / Feedback ##

Original Author:
- Ville Ranki <ville.ranki@iki.fi>

Contributors:
- Dan Krusi <dan.krusi@nerves.ch>
- Bob Gates
- Nicolas Montarnal

## Resources used

* B612 font by Airbus - see details and license at https://b612-font.com/
