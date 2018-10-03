OUTPUT_NAME=HulaLoopOSXDaemon

all: HulaLoopOSXDaemon

HulaLoopOSXDaemon: JackBridge.cpp jackClient.cpp
	g++ -Wall -std=c++11 -D__MACOSX_CORE__ -o ${OUTPUT_NAME} $^ -framework CoreMIDI -framework CoreAudio -framework CoreFoundation -ljack

clean:
	rm -rf ${OUTPUT_NAME}
