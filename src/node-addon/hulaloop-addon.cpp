#include <string>

#include <hlaudio/hlaudio.h>
#include "streaming-worker.h"

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
                        c.setActiveInputDevice(d);
                        delete d;
                    }
                }

                if (outputDevName.length())
                {
                    Device *d = c.findDeviceByName(inputDevName, DeviceType::OUTPUT);
                    if (d != nullptr)
                    {
                        c.setActiveOutputDevice(d);
                        delete d;
                    }
                }

                for (int i = 0; i < 15; i++)
                {
                    Message tosend("test", "this is a test");
                    this->writeToNode(progress, tosend);
                }
            }
    };
}

StreamingWorker * create_worker(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> & options)
{
  return new hula::NodeAddon(data, complete, error_callback, options);
}

NODE_MODULE(hulaloop, StreamWorkerWrapper::Init)