#include <iostream>
#include <fstream>
#include <string>
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <sstream>  
using namespace std;

string exec(const char* cmd) {
	//stolen off of https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
			result += buffer.data();
	}
	return result;
}

vector<string> getOutputDevices(){
    string outputDevices = exec("aplay -l");
	stringstream ss(outputDevices);
	string device;
	vector<string> devices;
	while(getline(ss, device, '\n')){
		if(!device.find("card "))
			devices.push_back(device);
	}
    return devices;
}

vector<string> getInputDevices(){
	string recordDevices = exec("arecord -l");
	stringstream ss(recordDevices);
	string device;
	vector<string> devices;
	while(getline(ss, device, '\n')){
		if(!device.find("card "))
		    devices.push_back(device);
	}
    return devices;
}

int main()
{
    vector<string> in = getInputDevices();
    vector<string> o = getOutputDevices();
    for(int i = 0; i < in.size(); i++){
	cout << in[i] << endl;
    }
    for(int i = 0; i < o.size(); i++){
	cout << o[i] << endl;
    }
}
