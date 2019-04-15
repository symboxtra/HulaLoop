#!/bin/bash
mkdir build 
cd build
cmake .. -DGEN_INSTALL=true -DCMAKE_BUILD_TYPE=Release
make -j
cpack ..
#sudo dpkg -i HulaLoop.deb
#rm HulaLoop.deb
cd .. 
