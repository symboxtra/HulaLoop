#include <string>
#include <nan.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <hlaudio/hlaudio.h>

#define HL_NODE_BUFFER_SIZE 1024

class NodeAddon : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init)
        {
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->SetClassName(Nan::New("HulaLoop").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(6);

            SetPrototypeMethod(tpl, "startCapture", startCapture);
            SetPrototypeMethod(tpl, "getDevices", getDevices);
            SetPrototypeMethod(tpl, "setSampleRate", setSampleRate);
            SetPrototypeMethod(tpl, "setInput", setInput);
            SetPrototypeMethod(tpl, "readBuffer", readBuffer);
            SetPrototypeMethod(tpl, "stopCapture", stopCapture);

            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("HulaLoop").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());

            // TODO: Figure out how to set the exports
            // Nan::SetMethod(module, "exports", New);
        }

    private:
        /**
         * HulaLoop Controller
         */
        hula::Controller *c = nullptr;
        /**
         * HulaLoop RingBuffer
         */
        hula::HulaRingBuffer *rb = nullptr;
        /**
         * Reference to JavaScript function used to
         * pass events/updates (not audio) from HulaLoop to Node.
         * This does not have a defined purpose yet.
         */
        Nan::Callback *dataCallback;
        /**
         * Reference to JavaScript function used to
         * pass error messages from HulaLoop to Node.
         */
        Nan::Callback *errorCallback;

        /**
         * Construct a new HulaLoop Node Addon.
         *
         * This addon exposes the basic features needed
         * to capture looback audio to a Node-based client
         * such as Electron.
         *
         * Options object:
         * @code
         * const opts = {
         *    input: "deviceName"
         * }
         * @endcode
         *
         * @param dataCallback JavaScript function used to
         * pass events/updates (not audio) from HulaLoop to Node.
         * This does not have a defined purpose yet.
         *
         * @param errorCallback JavaScript function used to
         * pass error messages from HulaLoop to Node.
         *
         * @param options JavaScript object containing options
         * for initial configuration of HulaLoop. As of now,
         * the object has the options listed above.
         */
        explicit NodeAddon(
            Nan::Callback *dataCallback,
            Nan::Callback *errorCallback,
            v8::Local<v8::Object> &options
        )
        {
            this->dataCallback = dataCallback;
            this->errorCallback = errorCallback;

            this->c = new hula::Controller();
            this->rb = c->createBuffer(1);

            if (options->IsObject())
            {
                // Handle input device in options object
                v8::Local<v8::Value> inputDevName = options->Get(Nan::New<v8::String>("input").ToLocalChecked());
                if (inputDevName->IsString())
                {
                    v8::String::Utf8Value s(inputDevName);
                    bool ret = this->setInputDevice(*s);
                }
            }
        }

        /**
         * Destroy HulaLoop objects that won't be garbage
         * collected by v8.
         */
        ~NodeAddon()
        {
            delete this->c;
            delete this->rb;
        }

        /**
         * Search for the device by name and try to set it as active.
         *
         * Any error message or failure generated is passed back to the
         * browser via the errorCallback function that was registered at
         * construction.
         *
         * @return True on success. False on failure.
         */
        bool setInputDevice(const std::string &deviceName)
        {
            bool ret = false;
            hula::Device *dev = this->c->findDeviceByName(deviceName);
            if (dev != nullptr)
            {
                try
                {
                    ret = this->c->setActiveInputDevice(dev);
                }
                catch(const hula::AudioException &e)
                {
                    v8::Local<v8::Value> argv[1] = { Nan::New(e.getMessage().c_str()).ToLocalChecked() };
                    this->errorCallback->Call(1, argv);
                }

                delete dev;
            }
            else
            {
                // Let the browser know that the device was not found
                v8::Local<v8::Value> argv[1] = { Nan::New(HL_ERROR_PREFIX "Device not found.").ToLocalChecked() };
                this->errorCallback->Call(1, argv);
            }

            return ret;
        }

        static NAN_METHOD(New)
        {
            if (info.IsConstructCall())
            {
                Nan::Callback *dataCallback = new Nan::Callback(info[0].As<v8::Function>());
                Nan::Callback *errorCallback = new Nan::Callback(info[1].As<v8::Function>());
                v8::Local<v8::Object> options = info[2].As<v8::Object>();

                NodeAddon *obj = new NodeAddon(
                    dataCallback,
                    errorCallback,
                    options
                );

                obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());
            }
            else
            {
                const int argc = 3;
                v8::Local<v8::Value> argv[argc] = {info[0], info[1], info[2]};
                v8::Local<v8::Function> cons = Nan::New(constructor());
                v8::Local<v8::Object> instance = Nan::NewInstance(cons, argc, argv).ToLocalChecked();
                info.GetReturnValue().Set(instance);
            }
        }

        /**
         * Start receiving samples from HulaLoop.
         */
        static NAN_METHOD(startCapture)
        {
            NodeAddon *_this = Nan::ObjectWrap::Unwrap<NodeAddon>(info.Holder());
            _this->rb->clear();
            _this->c->addBuffer(_this->rb);
        }

        /**
         * @brief Construct a new nan method object
         *
         */
        static NAN_METHOD(getDevices)
        {
            NodeAddon *_this = Nan::ObjectWrap::Unwrap<NodeAddon>(info.Holder());

            std::vector<hula::Device *> devices = _this->c->getDevices(hula::DeviceType::INPUT);
            v8::Local<v8::Array> devNames = Nan::New<v8::Array>(devices.size());

            for (int i = 0; i < devices.size(); i++)
            {
                devNames->Set(i, Nan::New(devices[i]->getName().c_str()).ToLocalChecked());
            }

            info.GetReturnValue().Set(devNames);
        }

        /**
         * Set the sample rate for HulaLoop.
         *
         * This should MUST match the sample rate of the Web Audio context.
         * If it does not, audio will either be intermittent or have incorrect pitch.
         *
         * Only 44.1kHz and 48kHz are supported right now.
         */
        static NAN_METHOD(setSampleRate)
        {
            float rate = info[0]->NumberValue();
            NodeAddon *_this = Nan::ObjectWrap::Unwrap<NodeAddon>(info.Holder());

            bool ret = _this->c->setSampleRate(rate);

            info.GetReturnValue().Set(ret);
        }

        /**
         * Set the input device by name.
         *
         * Any error messages generated by this action
         * are received via the registered errorCallback.
         *
         * @return true if successful, false if not
         */
        static NAN_METHOD(setInput)
        {
            v8::String::Utf8Value deviceName(info[0]->ToString());
            NodeAddon *_this = Nan::ObjectWrap::Unwrap<NodeAddon>(info.Holder());

            // Any errors will be passed via error callback
            bool ret = _this->setInputDevice(*deviceName);

            info.GetReturnValue().Set(ret);
        }

        /**
         * Read audio data from the HulaLoop buffer.
         *
         * This function takes in a JavaScript ArrayBuffer
         * and fills it with samples available from HulaLoop.
         *
         * Should HulaLoop not have enough samples available,
         * the remaining position in the ArrayBuffer are
         * filled with 0's (silence).
         *
         * Samples are stored as interleaved 32-bit floating point.
         *
         * Note: startCapture must be called before any non-silence
         * samples will become available.
         *
         * @return undefined
         */
        static NAN_METHOD(readBuffer)
        {
            v8::Local<v8::ArrayBuffer> jsBuffer = info[0].As<v8::ArrayBuffer>();
            NodeAddon *_this = Nan::ObjectWrap::Unwrap<NodeAddon>(info.Holder());

            SAMPLE *jsRawBuff = (SAMPLE *)jsBuffer->GetContents().Data();
            size_t maxSamples = jsBuffer->ByteLength() / sizeof(SAMPLE);
            size_t samplesRead = 0;

            samplesRead = _this->rb->read(jsRawBuff, maxSamples);

            if (samplesRead == 0)
            {
                hlDebug() << "NodeAddon: Received empty buffer" << std::endl;
            }

            // Finish off with silence if we weren't able to get enough samples
            // double angle = 0;
            for (size_t i = samplesRead; i < maxSamples; i++)
            {
                hlDebug() << "Silence filled" << std::endl;
                jsRawBuff[i] = 0.0f;

                // Generate a pure sine wave
                // jsRawBuff[i] = sin(angle);
                // angle += (2 * M_PI) / 256;
            }
        }

        /**
         * Stop receiving samples from HulaLoop.
         */
        static NAN_METHOD(stopCapture)
        {
            NodeAddon *_this = Nan::ObjectWrap::Unwrap<NodeAddon>(info.Holder());
            _this->c->removeBuffer(_this->rb);
            _this->rb->clear();
        }

        static inline Nan::Persistent<v8::Function> &constructor()
        {
            static Nan::Persistent<v8::Function> construct;
            return construct;
        }
};

NODE_MODULE(hulaloop, NodeAddon::Init)