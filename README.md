[![Jenkins Server Status](https://img.shields.io/badge/dynamic/json.svg?label=Jenkins%20Server&url=http%3A%2F%2Fwww.symboxtra.tk%2Fstatus.php%3Fservers%3Djenkins&query=%24..jenkins&colorB=0b7cbd)](http://jenkins.symboxtra.dynu.net "Jenkins Server Status")
[![](https://jenkins.symboxtra.dynu.net/job/HulaLoop/job/master/badge/icon)](https://jenkins.symboxtra.dynu.net/job/HulaLoop/job/master/ "Jenkins Build Status")
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.opensource.org/licenses/GPL-3.0)
[![CodeFactor](https://www.codefactor.io/repository/github/jmcker/hulaloop/badge)](https://www.codefactor.io/repository/github/jmcker/hulaloop)

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
sudo apt install g++ build-essential libgl1-mesa-dev cmake libasound2 libasound2-dev
```

**IMPORTANT:** Qt must be installed using the GUI installer since ```apt``` does not backport a recent enough version.

If you want to compile documentation:
```bash
sudo apt install python3-sphinx doxygen graphviz
```


#### OSX ####
Install ```brew```:
```bash
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

```bash
brew install g++ qt build-essential cmake portaudio sphinx-doc doxygen graphviz
```

**IMPORTANT:** JACK2 must be installed using the [GUI installer](http://jackaudio.org/downloads/) since ```brew``` only includes JACK1.


#### Windows ####
- Install Qt
- Install CMake
- Install Visual Studio or the MSVC build tools
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

```bash
cd build
ctest -C Debug -V
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
