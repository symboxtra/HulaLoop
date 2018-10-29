#include "QMLBridge.h"
//#include "FftRealPair.h"
#include "FftRealPair.cpp"

#include <QUrl>
#include <string>
#include <thread>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <stdlib.h>
#include <thread>     
#include <chrono>


/**
 * Construct a new instance of the QMLBridge class.
 *
 * @param parent Object which QMLBridge should be child of.
 */
QMLBridge::QMLBridge(QObject *parent) : QObject(parent)
{
    transport = new Transport;
}

/**
 * Return the current state of the Transport instance as a QString.
 *
 * @return state State of Transport instance
 */
QString QMLBridge::getTransportState() const
{
    return QString::fromStdString(transport->stateToStr(transport->getState()));
}

/**
 * Trigger record in the Transport and update the UI state via signal.
 */
void QMLBridge::record()
{
    transport->record();
    emit stateChanged();
}

/**
 * Trigger stop in the Transport and update the UI state via signal.
 */
void QMLBridge::stop()
{
    transport->stop();
    emit stateChanged();
}

/**
 * Trigger playback in the Transport and update the UI state via signal.
 */
void QMLBridge::play()
{
    transport->play();
    emit stateChanged();
}

/**
 * Trigger pause in the Transport and update the UI state via signal.
 */
void QMLBridge::pause()
{
    transport->pause();
    emit stateChanged();
    //cout<<"pause clicked\n";
    getData();
    //emit visData(dataFake);
}



/**
 * Match a string that the user chose to the input device list
 * and notify the backend
 */
void QMLBridge::setActiveInputDevice(QString QDeviceName)
{
    cout << "SETACTIVEiDEVICECALLED" << endl;
    string deviceName = QDeviceName.toStdString();
    vector<Device *> iDevices = transport->getController()->getInputDevices();
    for (auto const &device : iDevices)
    {
        if (device->getName() == deviceName)
        {
            transport->getController()->setActiveInputDevice(device);
            Device::deleteDevices(iDevices);
            return;
        }
    }
    //Should not get here should have found the device
    cerr << "Input device not found: " << deviceName << endl;
}

/**
 * Match a string that the user chose to the output device list
 * and notify the backend
 */
void QMLBridge::setActiveOutputDevice(QString QDeviceName)
{
    string deviceName = QDeviceName.toStdString();
    vector<Device *> oDevices = transport->getController()->getOutputDevices();
    for (auto const &device : oDevices)
    {
        if (device->getName() == deviceName)
        {
            transport->getController()->setActiveOutputDevice(device);
            Device::deleteDevices(oDevices);
            return;
        }
    }
    //Should not get here should have found the device
    cerr << "Output device not found: " << deviceName << endl;
}

/**
 * Get the current input devices
 *
 * @return QString containing current input devices
 */
QString QMLBridge::getInputDevices()
{
    string devices;
    vector<Device *> vd = transport->getController()->getInputDevices();
    for (int i = 0; i < vd.size(); i++)
    {
        devices += vd[i]->getName();
        if (i < vd.size() - 1)
        {
            devices += ",";
        }
    }
    Device::deleteDevices(vd);
    return QString::fromStdString(devices);
}

/**
 * Get the current output devices
 *
 * @return QString containing current output devices
 */
QString QMLBridge::getOutputDevices()
{
    string devices;
    vector<Device *> vd = transport->getController()->getOutputDevices();
    for (int i = 0; i < vd.size(); i++)
    {
        devices += vd[i]->getName();
        if (i < vd.size() - 1)
        {
            devices += ",";
        }
    }
    Device::deleteDevices(vd);
    return QString::fromStdString(devices);
}

/**
 * Get the directory the user wants to save to
 *
 * @param QString containing the directory
 *
 * @return String containing the directory
 */
void QMLBridge::saveFile(QString dir)
{
    // create a Qurl so it is easy to convert
    QUrl url(dir);
    string directory = url.path().toStdString();
    transport->exportFile(directory);
}
void QMLBridge::getData(){
    rb=transport->getController()->createAndAddBuffer(.5);
    //std::this_thread::sleep_for (std::chrono::seconds(1));
    //cout<<"in get data\n";
    std::thread visThread(&updateVisualizer,this);
    visThread.detach();

}
void QMLBridge::updateVisualizer(QMLBridge *_this){
    /**CREATE BUFFER
     * WHILE(1)
     * READ FROM BUFFER
     * GET NUM OF BINS
     * PROCESS THE DATA
     * UPDATE VISUALIZER
     * SLEEP
     * REPEAT
     */
    //FOR NOW JUST GIVE RANDOM DATA IN ARRAY
    int maxSize = 512;
    float * temp = new float[maxSize];
    //cout<<"before the for loop\n";
    //for(int i=0;i<10;i++){
    while(1){
        size_t bytesRead=_this->rb->read(temp, maxSize);
        //printf("Read %zu bytes.\n", bytesRead);
        vector<double> actualoutreal;
	    vector<double> actualoutimag;
        for(int i=0;i<bytesRead;i++){
            actualoutimag.push_back(temp[i]);
            actualoutreal.push_back(temp[i]);
        }
        /*printf("before transform real vector:\n");
        for(int i=0;i<bytesRead;i++){
            printf("%f\n",actualoutreal[i]);
        }
         printf("before transform imag vector:\n");
        for(int i=0;i<bytesRead;i++){
            printf("%f\n",actualoutimag[i]);
        }*/
	    Fft::transform(actualoutreal, actualoutimag);
        printf("after transform real vector:\n");
        for(int i=0;i<bytesRead;i++){
            //printf("%f\n",actualoutreal[i]);
            //get every 32 values, sum them, absolute value them, then make them the height
        }
        
         printf("after transform imag vector:\n");
        for(int i=0;i<bytesRead;i++){
            printf("%f\n",actualoutimag[i]);
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(5));
    }
   /* vector<qreal> dataFake;
    for(int i=0;i<10;i++){
        dataFake.push_back((qreal) .5);
    }
    _this->emit visData(dataFake);*/


}
/*
static size_t reverseBits(size_t x, int n) {
	size_t result = 0;
	for (int i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1U);
	return result;
}
void Fft::transform(vector<double> &real, vector<double> &imag) {
	size_t n = real.size();
	if (n != imag.size())
		throw std::invalid_argument("Mismatched lengths");
	if (n == 0)
		return;
	else if ((n & (n - 1)) == 0)  // Is power of 2
		transformRadix2(real, imag);
	else  // More complicated algorithm for arbitrary sizes
		transformBluestein(real, imag);
}
void Fft::transformRadix2(vector<double> &real, vector<double> &imag) {
	// Length variables
	size_t n = real.size();
	if (n != imag.size())
		throw std::invalid_argument("Mismatched lengths");
	int levels = 0;  // Compute levels = floor(log2(n))
	for (size_t temp = n; temp > 1U; temp >>= 1)
		levels++;
	if (static_cast<size_t>(1U) << levels != n)
		throw std::domain_error("Length is not a power of 2");
	
	// Trignometric tables
	vector<double> cosTable(n / 2);
	vector<double> sinTable(n / 2);
	for (size_t i = 0; i < n / 2; i++) {
		cosTable[i] = std::cos(2 * M_PI * i / n);
		sinTable[i] = std::sin(2 * M_PI * i / n);
	}
	
	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++) {
		size_t j = reverseBits(i, levels);
		if (j > i) {
			std::swap(real[i], real[j]);
			std::swap(imag[i], imag[j]);
		}
	}
	
	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2) {
		size_t halfsize = size / 2;
		size_t tablestep = n / size;
		for (size_t i = 0; i < n; i += size) {
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
				size_t l = j + halfsize;
				double tpre =  real[l] * cosTable[k] + imag[l] * sinTable[k];
				double tpim = -real[l] * sinTable[k] + imag[l] * cosTable[k];
				real[l] = real[j] - tpre;
				imag[l] = imag[j] - tpim;
				real[j] += tpre;
				imag[j] += tpim;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}

void Fft::transformBluestein(vector<double> &real, vector<double> &imag) {
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	size_t n = real.size();
	if (n != imag.size())
		throw std::invalid_argument("Mismatched lengths");
	size_t m = 1;
	while (m / 2 <= n) {
		if (m > SIZE_MAX / 2)
			throw std::length_error("Vector too large");
		m *= 2;
	}
	
	// Trignometric tables
	vector<double> cosTable(n), sinTable(n);
	for (size_t i = 0; i < n; i++) {
		unsigned long long temp = static_cast<unsigned long long>(i) * i;
		temp %= static_cast<unsigned long long>(n) * 2;
		double angle = M_PI * temp / n;
		// Less accurate alternative if long long is unavailable: double angle = M_PI * i * i / n;
		cosTable[i] = std::cos(angle);
		sinTable[i] = std::sin(angle);
	}
	
	// Temporary vectors and preprocessing
	vector<double> areal(m), aimag(m);
	for (size_t i = 0; i < n; i++) {
		areal[i] =  real[i] * cosTable[i] + imag[i] * sinTable[i];
		aimag[i] = -real[i] * sinTable[i] + imag[i] * cosTable[i];
	}
	vector<double> breal(m), bimag(m);
	breal[0] = cosTable[0];
	bimag[0] = sinTable[0];
	for (size_t i = 1; i < n; i++) {
		breal[i] = breal[m - i] = cosTable[i];
		bimag[i] = bimag[m - i] = sinTable[i];
	}
	
	// Convolution
	vector<double> creal(m), cimag(m);
	convolve(areal, aimag, breal, bimag, creal, cimag);
	
	// Postprocessing
	for (size_t i = 0; i < n; i++) {
		real[i] =  creal[i] * cosTable[i] + cimag[i] * sinTable[i];
		imag[i] = -creal[i] * sinTable[i] + cimag[i] * cosTable[i];
	}
}

*/
