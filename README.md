[![Jenkins Server Status](https://img.shields.io/badge/dynamic/json.svg?label=Jenkins%20Server&url=http%3A%2F%2Fwww.symboxtra.tk%2Fstatus.php%3Fservers%3Djenkins&query=%24..jenkins&colorB=0b7cbd)](http://jenkins.symboxtra.dynu.net "Jenkins Server Status")
[![](https://jenkins.symboxtra.dynu.net/job/HulaLoop/job/master/badge/icon)](https://jenkins.symboxtra.dynu.net/job/HulaLoop/job/master/ "Jenkins Build Status")
[![codecov](https://codecov.io/gh/jmcker/HulaLoop/branch/master/graph/badge.svg?token=okLfIHMeEf)](https://codecov.io/gh/jmcker/HulaLoop "Code Coverage Status")
[![CodeFactor](https://www.codefactor.io/repository/github/jmcker/hulaloop/badge)](https://www.codefactor.io/repository/github/jmcker/hulaloop "Code Factor Grade")
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.opensource.org/licenses/GPL-3.0)

# HulaLoop #

## Important Commands ##
A rough set of commands needed to build the project.

### Git ###
Clone the project:
```bash
git clone --recurse-submodule https://github.com/symboxtra/HulaLoop
```

Update an already cloned submodule:
```bash
git submodule update --remote --merge
```

### Dependencies ###


#### Debian-based Linux ####
Required:
```bash
sudo apt install g++ build-essential cmake libgl1-mesa-dev libasound2 libasound2-dev
```

**IMPORTANT:** This application requires Qt 5.10+. This typically has to be installed using the Qt GUI installer since ```apt``` does not backport a recent enough version.

If using GNOME (fixes system tray icons and notifications):
```bash
sudo apt install gnome-shell-extension-appindicator
```

If you want to compile documentation:
```bash
sudo apt install python-sphinx doxygen graphviz
python -m pip install --user recommonmark
```

#### RPM-based Linux ####
Required:
```bash
sudo dnf install gcc-c++ make cmake qt5-devel qt5-qtquickcontrols2-devel alsa-lib alsa-lib-devel
```

To enable the system tray icon and notifications the ```Topicons plus``` extension must be installed via ```dnf``` and enabled under *Tweaks->Extensions*.
```bash
sudo dnf install gnome-shell-extension-topicons-plus
```

If you want to compile documentation:
```bash
sudo dnf install python-sphinx doxygen graphviz
python -m pip install --user recommonmark
```

#### OSX ####
Install ```brew```:
```bash
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

```bash
brew install g++ qt build-essential cmake portaudio
```

**IMPORTANT:** JACK2 must be installed using the [GUI installer](http://jackaudio.org/downloads/) since ```brew``` only includes JACK1.

If you want to compile documentation:
```bash
brew install sphinx-doc doxygen graphviz
python -m pip install recommonmark
```

#### Windows ####
- Install [Qt](https://www.qt.io/download)
- Install [Visual Studio](https://visualstudio.microsoft.com/downloads/) or the MSVC build tools
- Install [Chocolatey](https://chocolatey.org/docs/installation) (package manager for Windows)
- Install Chocolatey packages
```powershell
choco install git
choco install cmake
choco install doxygen
```

- Good luck...

### CMake ###

```bash
mkdir build
cd build
cmake ..
cmake --build .
./bin/<executable name>
```

### CTest ###

All tests:
```bash
cd build
ctest -C Debug -V
```

Without checks for memory leaks:
```bash
cd build
ctest -C Debug -V -E memcheck
```

Only memory leak checks:
```bash
cd build
ctest -C Debug -V -R memcheck
```

### Build Tutorials and Documentation ###
```bash
cd docs
make html # Runs Doxygen and Sphinx builds
cd build-docs/html
# python -m SimpleHTTPServer is a simple way to serve these
# Open localhost:8000 in your browser to view
```

### Build Developer Documentation ###
This is built with the first option. To build separetly, use:
```bash
cd docs
doxygen # Finds default Doxyfile
cd build-doxygen/html
```
