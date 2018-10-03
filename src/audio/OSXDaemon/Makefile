OUTPUT_NAME=HulaLoopOSXDaemon

all: HulaLoopOSXDaemon

HulaLoopOSXDaemon: OSXDaemon.cpp JackClient.cpp
	g++ -Wall -std=c++11 -D__MACOSX_CORE__ -o ${OUTPUT_NAME} $^ -framework CoreMIDI -framework CoreAudio -framework CoreFoundation -ljack

clean:
	rm -rf ${OUTPUT_NAME}
