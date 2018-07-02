# ExtPlane-Panel #

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

* Linux 32bit, 15MB
    * [http://dankrusi.com/downloads/ExtPlane-Panel-v0.1-Linux32.zip](http://goo.gl/BNzoeZ)
* Linux 64bit, 15MB
    * [http://dankrusi.com/downloads/ExtPlane-Panel-v0.1-Linux64.zip](http://goo.gl/akBtl4)
* Windows 32bit, 12MB
    * [http://dankrusi.com/downloads/ExtPlane-Panel-v0.1-Win32.zip](http://goo.gl/Nupczn)
* Windows 64bit, 13MB
    * [http://dankrusi.com/downloads/ExtPlane-Panel-v0.1-Win64.zip](http://goo.gl/6EHtei)
* OS-X 64bit, 15MB
    * [http://dankrusi.com/downloads/ExtPlane-Panel-v0.1-OSX64.zip](http://goo.gl/8kLzJg)

Windows users: If the panel fails to start due to missing MSVCR100.dll, install Microsoft Visual 
C++ 2010 SP1 Redistributable Package from https://www.microsoft.com/en-us/download/details.aspx?id=13523


## Instruments ##

There are currently a number of instruments available, including:

- Basic compass
- Direction indicator with the compass rose
- Airspeed indicator
- Altimeter
- Attitude indicator
- Variometer
- RPM gauge
- HSI
- GPS/Radar map
- Light indicator
- Engine display
- Navigation display

Most instruments look like general aviation instruments and can be customized and adjusted.
You can also add and customize covers for your instruments. In addition, there are also
debugging tools which can help when developing with the X-Plane API.



## Hardware Devices ##

Currently supported output devices:

* Chroma USB Servo controller (http://electronics.chroma.se/usbsb.php) 
* Raspberry Pi's ServoBlaster module which allows RPi's GPIO pins to output PWM
* Pololu SSC04A (or compatible) serial servo controller


## Usage ##

### Profiles and Panels ###

You can save your work when setting up your ExtPlane-Panel through the use of profiles. A profile
is a file that can be saved to disk (ending with `.ini`). At first launch, ExtPlane-Panel will
automatically create and load a profile in your documents folder called `ExtPlane-Panel-Default.ini`.

Each profile may contain multiple panels, which can be switched using arrow keys, or through
the panel manager, or automatically when changing aircraft (if enabled in app settings). Only a single
panel is loaded at any time.

### Command Line ###

Command line arguments override application settings. This can be useful when you wish to launch
multiple panels on multiple monitors, each panel using a preset configuration.

* **--fullscreen**                forces the app to launch in fullscreen mode
* **--screen {number}**           sets the panel to a specific monitor/screen
* **--width {number}**            set the width in pixels of the panel (if not fullscreen)
* **--height {number}**           set the height in pixels of the panel (if not fullscreen)
* **--profile {filepath}**        loads the panel profile specified by filepath
* **--hidden-gui {true|false}**   when true, removes any visible extra UI (such as the yellow tab for opening menu)
* **--simulate {true|false}**     when true, simulates the ExtPlane connection
* **--panelrotation {0-360}**     sets the global panel rotation
* **--serveraddress {host:port}** sets the ExtPlane plugin host and port
* **--interpolate {true|false}**  when true, enables value interpolation
* **--antialias {true|false}**    when true, enables rendering antialias
* **--autopanels {true|false}**    when true, a panel will automatically be loaded (or created) when the aircraft changes
* **--adjustpower {true|false}**   when true, panel items will be dimmed or powered off when avionics or battery is off
* **--fontsize {number}**          sets the default font size for all panel items
* **--remembersizeandposition {true|false}**  when true, the panel window will always be positioned at the same place
* **--panelupdateinterval {seconds}** sets the update interval in seconds

### User Interface ###

* Click inside the **yellow tab** at top left to open the main menu.

* **Edit Panel** makes the instruments become selectable with a click.
It is then possible to move the instruments around. Double-click on an
instrument to see or modify its properties or delete it. Edit mode can
be also toggled with space key.

* **Add Item** allows new instruments to be added.

* **Save Profile** saves all the panel profile settings to a file.

* **Save Profile As...** saves a panel profile to a different file or location.

* **Load Profile** loads any given panel profile from disk.

* **New Profile** creates an empty new profile with a single panel.

* **Manage Panels** allows you to remove, add, rename and copy panels.

* **Hardware** opens the hardware window where you can configure various physical devices.

* **App Settings** has various settings for the application:
 - IP address or hostname of the X-Plane machine
 - Fullscreen
 - Screen rotation
 - Simulated connection - use for testing new instruments without real X-Plane connection
 - Interpolate values - you can disable this on slow devices
 - ExtPlane update interval - time (in seconds) how often X-Plane should send updates at maximum
 - Panel update interval - time (in seconds) how often the panel should be redrawn
 - Default font size - affects all instruments
 - Automatically create and load panels - a panel will automatically be loaded (or created) when the aircraft changes
 - Adjust panel brightness and power - panel items will be dimmed or powered off when avionics or battery is off

### Keyboard Shortcuts ###

* **Space** toggles edit mode
* **F** sets fullscreen mode
* **Delete** deletes selected panel items
* **Left** selects the previous panel (if any)
* **Right** selects the next panel (if any)

### Setting up hardware instruments ###

Currently servo-like devices are supported. ExtPlane-panel has concept
of output devices which contain one or more outputs (usually servo outputs)
and bindings which contain mapping between X-Plane dataref and one output on
a output device.

**Example:** Making a servo that displays indicated airspeed with range of 0-200 knots.

 * Press hardware button. Enable the output device you want to use on Devices tab.
 * Open Bindings tab. Click "New".
 * Enter following details:
   * Name: ASI
   * Input dataref: sim/cockpit2/gauges/indicators/airspeed_kts_pilot
   * Dateref accuracy: 0.5 (or as high value as possible)
   * Input min: 0
   * Input max: 200 (this sets the range of ASI in knots)
   * Output device: the device you enabled
   * Output num: Number of output on your device. Usually 0=first servo, 1=second servo..
   * Open the output curve dialog and set:
   * Output min: 45 (device dependent; set this to the value that moves servo to zero position)
   * Output max: 250 (device dependent; set this to the value that moves servo to 200kt position)
   * Interpolation speed: 3 (Makes the movement less jerky. Set to 0 for no interpolation)
 * Click Save changes. Servo should now start moving.
 * If you modify values, click Save changes again. Remember to save the panel to save these settings!



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

Adding items is really easy if you know some C++ programming. Subclass
PanelItem class and implement painting and subscription to datarefs as needed.
See Button class for a really simple example.
If your instrument is round gauge with a needle, you might want to subclass
NeedleInstrument class. See Airspeed or Variometer classes for example.

Code should be reasonably well documented. If something is not clear, please
report it.

GPL licence does not *require* you to release any changes you do to this
application if you only use them yourself. Despite this we *ask* you to
release any new instruments you create or any other improvements you make
into the application.

Preferred way is to create a fork in github and send a pull request when
you are finished. If you don't want to do this, we're happy with any
other way to deliver the changes.



## Coding Guidelines ##

### Headers ###
Always group headers in a meaningful format (ie all Qt headers should be grouped together,
and all ExtPlane headers grouped together). In addition, make sure that headers
are always fully relative (ie use `../util/header.h` instead of `util/header.h`).
This is required to build across all platforms.

### Platform-Dependent Code ###
When writing code which uses new features currently not implemented, always make
sure to first try to use Qt cross-platform classes and libraries. When using
platform dependent code, make sure to `#ifdef` the sections of code which will
only work on a specific platform. You can use standard Qt defines, or additional
defines such as `TERMIOS_AVAIALABLE` to help with this.

## Widget UI vs QML ui ##

Currently ExtPlane-Panel is written using Qt Widgets in c++. It would be
better to change to QML based UI as it is far superior in many ways.

* widgetui directory contains the current widget ui.
* qmlui directory ontains the new QML based ui in progress.

## Contact / Feedback ##

Original Author:
- Ville Ranki <ville.ranki@iki.fi>

Contributors:
- Dan Krusi <dan.krusi@nerves.ch>
- Bob Gates
- Nicolas Montarnal

