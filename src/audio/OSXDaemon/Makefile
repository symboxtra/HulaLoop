OUTPUT_NAME=HulaLoopOSXDaemon

all: HulaLoopDaemon

HulaLoopDaemon:
	g++ -Wall -std=c++11 -D__MACOSX_CORE__ -o ${OUTPUT_NAME} JackBridge.cpp jackClient.cpp -framework CoreMIDI -framework CoreAudio -framework CoreFoundation -ljack

HulaLoopDaemonMidi:
	g++ -Wall -std=c++11 -D__MACOSX_CORE__ -D_WITH_MIDI_BRIDGE_ -o ${OUTPUT_NAME}Midi JackBridge.cpp jackClient.cpp -framework CoreMIDI -framework CoreAudio -framework CoreFoundation -ljack -lrtmidi

clean:
	rm -rf ${OUTPUT_NAME}