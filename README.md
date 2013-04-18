# ExtPlane-Panel #

![Screenshot](http://s23.postimg.org/8xhypdei3/extplane_panel_screenshot_v2.png)

An external, networked cockpit panel for the X-Plane flight simulator.

License: GNU GPLv3

It is possible to run this panel on the same computer or another computer
on the network to display cockpit instruments from remote X-Plane.

Almost all instruments are drawn using scalable vector graphics for
perfect scaling and high resolutions.

The application also allows running hardware instruments with X-Plane
datarefs. Currently it supports two output devices:

* Raspberry Pi's ServoBlaster module which allows RPi's GPIO pins to output PWM
* Pololu SSC04A (or compatible) serial servo controller

Both of these are used to control servos using PWM. Other, more complex
output devices can be implemented later.


The app is written using Qt and is platform independent - it runs on
different Linux variants, Mac and Windows. X-Plane SDK is NOT required
to build ExtPlane-Panel.

Target hardware is:
* Desktop PC's
* Android
* Nokia Maemo 4 & 5,
* MeeGo
* Raspberry Pi
* anything that will run Qt!

## Building ##

ExtPlane-Panel uses the Qt Framework for graphics and cross-platform compatibility. Before building you'll need to setup Qt 4.5 or greater and Qt Mobility to compile. You'll also need to check out the ExtPlane plugin sources (https://github.com/vranki/ExtPlane) to the directory next to the ExtPlane-Panel directory.

The requirements for ExtPlane-Panel are as follows:
* C++ Toolchain
* Qt Framework 4.5+
* ExtPlane

### Debian Linux ###
```bash
# 1: Install required libraries and tools
sudo apt-get install git build-essential libqt4-dev qtmobility-dev

# 2: Download source code from GitHub
git clone https://github.com/vranki/ExtPlane.git
git clone https://github.com/vranki/ExtPlane-Panel.git

# 3: Build project
cd ExtPlane-Panel
qmake
make
```



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

Most instruments look like general aviation instruments. You can also add and customize covers for your instruments.



## Usage ##

### Command Line ###

Command line arguments override application settings. This can be useful when you wish to launch multiple panels on multiple monitors, each panel using a preset configuration.

* **--fullscreen** forces the app to launch in fullscreen mode
* **--screen [number]** sets the panel to a specific monitor/screen
* **--profile [filepath]** loads the panel profile specified by filepath

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

* **App Settings** has various settings for the application:
 - IP address or hostname of the X-Plane machine
 - Fullscreen
 - Screen rotation
 - Simulated connection - Use for testing new instruments without real X-Plane connection
 - Interpolate values - You can disable this on slow devices.
 - ExtPlane update interval - time (in seconds) how often X-Plane should send updates at maximum
 - Panel update interval - time (in seconds) how often the panel should be redrawn
 - Default font size - affects all instruments

### Setting up hardware instruments ###

Currently servo-like devices are supported. ExtPlane-panel has concept
of output devices which contain one or more outputs (usually servo outputs)
and bindings which contain mapping between X-Plane dataref and one output on
a output device.

Example: Making a servo that displays indicated airspeed with range of 0-200 knots.

 * Press hardware button. Enable the output device you want to use on Devices tab.
 * Open Bindings tab. Click "New".
 * Enter following details:
   * Name: ASI
   * Input dataref: sim/cockpit2/gauges/indicators/airspeed_kts_pilot
   * Dateref accuracy: 0.5 (or as high value as possible)
   * Input min: 0
   * Input max: 200 (this sets the range of ASI in knots)
   * Output device: the device you enabled
   * Output num: Number of output on your device. Usually 0=first servo, 2=second servo..
   * Output min: 45 (set this to the value that moves servo to zero position)
   * Output max: 250 (set this to the value that moves servo to 200kt position)
 * Click Save changes. Servo should now start moving.
 * If you modify values, click Save changes again. Remember to save the panel to save these settings!

Note: If you need to reverse the servo direction, swap input min and input max values.


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



## Contact / Feedback ##

Original Author:
- Ville Ranki <ville.ranki@iki.fi>

Contributors:
- Dan Krusi <dan.krusi@nerves.ch>
- Bob Gates
