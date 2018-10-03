# This Makefile is only for compiling the OSXDaemon as a standaloen executable
# IT IS NOT USED BY THE CMAKE BUILD

OUTPUT_NAME=HulaLoopOSXDaemon

all: HulaLoopOSXDaemon

HulaLoopOSXDaemon: main.cpp OSXDaemon.cpp JackClient.cpp
	g++ -Wall -std=c++11 -D__MACOSX_CORE__ -o ${OUTPUT_NAME} $^ -framework CoreAudio -framework CoreFoundation -ljack

clean:
	rm -rf ${OUTPUT_NAME}
