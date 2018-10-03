OUTPUT_NAME=HulaLoopOSXDaemon

all: HulaLoopOSXDaemon

HulaLoopOSXDaemon:
	g++ -Wall -std=c++11 -D__MACOSX_CORE__ -o ${OUTPUT_NAME} JackBridge.cpp jackClient.cpp -framework CoreMIDI -framework CoreAudio -framework CoreFoundation -ljack

clean:
	rm -rf ${OUTPUT_NAME}