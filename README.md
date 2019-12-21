[![Jenkins Server Status](https://img.shields.io/badge/dynamic/json.svg?label=Jenkins%20Server&url=http%3A%2F%2Fwww.symboxtra.tk%2Fstatus.php%3Fservers%3Djenkins&query=%24..jenkins&colorB=0b7cbd)](http://jenkins.symboxtra.dynu.net "Jenkins Server Status")
[![](https://jenkins.symboxtra.dynu.net/job/HulaLoop/job/master/badge/icon)](https://jenkins.symboxtra.dynu.net/job/HulaLoop/job/master/ "Jenkins Build Status")
[![codecov](https://codecov.io/gh/symboxtra/HulaLoop/branch/master/graph/badge.svg?token=okLfIHMeEf)](https://codecov.io/gh/symboxtra/HulaLoop "Code Coverage Status")
[![CodeFactor](https://www.codefactor.io/repository/github/symboxtra/hulaloop/badge)](https://www.codefactor.io/repository/github/symboxtra/hulaloop "Code Factor Grade")
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
sudo apt install build-essential cmake libgl1-mesa-dev libpulse-dev libsndfile-dev
```

If you want to compile documentation:
```bash
sudo apt install python-sphinx doxygen graphviz help2man
python -m pip install --user recommonmark
```

If using GNOME (fixes system tray icons and notifications):
```bash
sudo apt install gnome-shell-extension-appindicator
```

#### Debian Qt ####

Check which version of Debian you have by running
```bash
cat /etc/debian_version
```

If you're running Debian Buster, Qt can be installed from ```apt```:
```bash
sudo apt install qt5-default qtdeclarative5-dev libqt5charts5 libqt5quickcontrols2-5
```

**IMPORTANT:** This application requires Qt 5.9.5+. This typically has to be installed using the Qt GUI installer since ```apt``` on Debian Stretch does not backport an up-to-date version. Be sure to install the extra **Qt Charts** package as the GUI relies on it.

#### Redhat-based Linux ####
Required:
```bash
sudo dnf install gcc-c++ make cmake qt5-devel qt5-qtcharts qt5-qtquickcontrols2-devel pulseaudio-libs-devel libsndfile-devel
```

To enable the system tray icon and notifications the ```Topicons plus``` extension must be installed via ```dnf``` and enabled under *Tweaks->Extensions*.
```bash
sudo dnf install gnome-shell-extension-topicons-plus
```

If you want to compile documentation:
```bash
sudo dnf install python-sphinx doxygen graphviz help2man
python -m pip install --user recommonmark
```

If you want to generate RPM packages:
```bash
sudo dnf install rpm-build
```

#### OSX ####
Install ```brew```:
```bash
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

```bash
brew install g++ qt build-essential cmake portaudio libsndfile
```

**IMPORTANT:** JACK2 must be installed using the [GUI installer](http://jackaudio.org/downloads/) since ```brew``` only includes JACK1.

If you want to compile documentation:
```bash
brew install sphinx-doc doxygen graphviz
python -m pip install recommonmark
```

#### Windows ####
- Install [Qt](https://www.qt.io/download) (including Charts)
- Install [libsndfile](http://www.mega-nerd.com/libsndfile/#Download)
- Install [Chocolatey](https://chocolatey.org/docs/installation) (package manager for Windows)
- Install Chocolatey packages from an Administrator PowerShell:

```powershell
choco install visualstudio2019-workload-vctools
choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'
choco install git
choco install doxygen
```

A number of environment variables must be defined before the CMake generation stage will complete.

| Variable      | Example                                 | Description                                                                                        |
|---------------|-----------------------------------------|----------------------------------------------------------------------------------------------------|
| `QT_ROOT`     | `C:\Qt\5.12.4`                          | Path to desired version of Qt. This assumes that the `msvc2017_64` or `msvc2017` subfolders exist. |
| `LIBSND_ROOT` | `C:\Program Files\Mega-Nerd\libsndfile` | Path to `libsndfile` installation.                                                                 |

- Good luck...

### CMake ###

```bash
mkdir build
cd build
cmake ..
cmake --build .
./bin/<executable name>
```

#### Windows Tips ####
We've found the ```msbuild``` output on Windows to be rather unruly.
Passing the ```/verbosity:minimal``` or ```/v:m``` options at build time makes the output much more make-like.
Relevant information, warnings, and errors are still displayed.

```console
cmake --build . -- /verbosity:minimal
cmake --build . -- /v:m
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
