#include <string>

#include <hlaudio/hlaudio.h>
#include "streaming-worker.h"

#define HL_NODE_BUFFER_SIZE 512

namespace hula {

    class NodeAddon : public StreamingWorker {
        private:
            std::string inputDevName = "";
            std::string outputDevName = "";

        public:

            NodeAddon(Callback *data, Callback *complete, Callback *error,
            v8::Local<v8::Object> &options) : StreamingWorker(data, complete, error)
            {
                // Parse the startup options
                if (options->IsObject())
                {
                    v8::Local<v8::Value> inputDevName = options->Get(New<v8::String>("input").ToLocalChecked());
                    if (inputDevName->IsString())
                    {
                        v8::String::Utf8Value s(inputDevName);
                        this->inputDevName = *s;
                    }

                    v8::Local<v8::Value> outputDevName = options->Get(New<v8::String>("input").ToLocalChecked());
                    if (outputDevName->IsString())
                    {
                        v8::String::Utf8Value s(outputDevName);
                        this->inputDevName = *s;
                    }
                }
            }

            void Execute(const ExecutionProgress &progress)
            {
                Controller c;

                if (inputDevName.length())
                {
                    Device *d = c.findDeviceByName(inputDevName, DeviceType::INPUT);
                    if (d != nullptr)
                    {
                        try
                        {
                            c.setActiveInputDevice(d);
                        }
                        catch (const AudioException &e)
                        {
                            this->SetErrorMessage(e.getMessage().c_str());
                        }

                        delete d;
                    }
                }

                if (outputDevName.length())
                {
                    Device *d = c.findDeviceByName(inputDevName, DeviceType::OUTPUT);
                    if (d != nullptr)
                    {
                        try
                        {
                            c.setActiveOutputDevice(d);
                        }
                        catch (const AudioException &e)
                        {
                            this->SetErrorMessage(e.getMessage().c_str());
                        }

                        delete d;
                    }
                }

                HulaRingBuffer *rb = c.createAndAddBuffer(1);
                Message<float *> msg("audio", nullptr, -1);

                while (!closed())
                {
                    SAMPLE *buff = new SAMPLE[HL_NODE_BUFFER_SIZE];
                    ring_buffer_size_t sampleCount = rb->read(buff, HL_NODE_BUFFER_SIZE);

                    if (sampleCount > 0)
                    {
                        msg.data = buff;
                        msg.length = sampleCount;
                        this->writeToNode(progress, msg);
                    }
                    else
                    {
                        delete [] buff;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
            }
    };
}

StreamingWorker * create_worker(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> & options)
{
  return new hula::NodeAddon(data, complete, error_callback, options);
}

NODE_MODULE(hulaloop, StreamWorkerWrapper::Init)