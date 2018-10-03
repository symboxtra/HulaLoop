/*
MIT License

Copyright (c) 2016 Shunji Uno <madhatter68@linux-dtm.ivory.ne.jp>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <errno.h>
#include <jack/jack.h>

#include <cstdio>

#include "../HulaAudioError.h"
#include "JackClient.hpp"

/**********************************************************************
 private functions
**********************************************************************/
int JackClient::process_callback(jack_nframes_t nframes)
{
    return 0;
}

int JackClient::_process_callback(jack_nframes_t nframes, void * arg)
{
    JackClient * obj = (JackClient *)arg;
    return obj->process_callback(nframes);
}

int JackClient::sync_callback(jack_transport_state_t state, jack_position_t * pos)
{
    return 0;
}

int JackClient::_sync_callback(jack_transport_state_t state, jack_position_t * pos, void * arg)
{
    JackClient * obj = (JackClient *)arg;
    return obj->sync_callback(state, pos);
}

void JackClient::timebase_callback(jack_transport_state_t state, jack_nframes_t nframes,
                                   jack_position_t * pos, int new_pos)
{
}

void JackClient::_timebase_callback(jack_transport_state_t state, jack_nframes_t nframes,
                                    jack_position_t * pos, int new_pos, void * arg)
{
    JackClient * obj = (JackClient *)arg;
    obj->timebase_callback(state, nframes, pos, new_pos);
}

/**********************************************************************
 public functions
**********************************************************************/
JackClient::JackClient(const char * name, uint32_t flags)
{
    jack_status_t jst;

    // TODO: Setting Jack optiosn here might be useful
    // http://jackaudio.org/files/docs/html/types_8h.html#a396617de2ef101891c51346f408a375e

    // Try to open the default Jack server
    // If the user already has it running, we should join gracefully
    client = jack_client_open(name, JackNoStartServer, &jst);
    if (!client)
    {
        fprintf(stderr, "\n%sCould not connect to default JACK server. Error code: %x\n", HL_ERROR_PREFIX, jst);
        fprintf(stderr, "%sTrying again using name '%s'...\n\n", HL_ERROR_PREFIX, HL_JACK_SERVER_NAME);

        // Try to open our own named server
        client = jack_client_open(name, JackServerName, &jst, HL_JACK_SERVER_NAME);
        if (!client)
        {
            fprintf(stderr, "%sFailed to connect/start JACK server '%s'. Error code: %x\n", HL_ERROR_PREFIX, HL_JACK_SERVER_NAME, jst);
            return;
        }
    }

    SampleRate = jack_get_sample_rate(client);
    BufSize = jack_get_buffer_size(client);
    cb_flags = flags;
}

JackClient::~JackClient()
{
    jack_client_close(client);
}

int JackClient::register_ports(const char * nameAin[], const char * nameAout[])
{
    int i;

    // create Audio input ports
    nAudioIn = 0;
    if (nameAin)
    {
        for (i = 0; (nameAin[i] != NULL) && (i < MAX_PORT_NUM); i++)
        {
            audioIn[i] = jack_port_register(client, nameAin[i], JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
        }
        nAudioIn = i;
    }

    // create Audio output ports
    nAudioOut = 0;
    if (nameAout)
    {
        for (i = 0; (nameAout[i] != NULL) && (i < MAX_PORT_NUM); i++)
        {
            audioOut[i] = jack_port_register(client, nameAout[i], JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        }
        nAudioOut = i;
    }

    return 0;
}

void JackClient::activate()
{
    if (cb_flags & JACK_PROCESS_CALLBACK)
    {
        jack_set_process_callback(client, _process_callback, this);
    }
    // jack_set_freewheel_callback(client, _freewheel_callback, arg);
    // jack_on_shutdown(client, _on_shutdown, arg);

    if (cb_flags & JACK_SYNC_CALLBACK)
    {
        if (jack_set_sync_callback(client, _sync_callback, this) != 0)
        {
            fprintf(stderr, "%sjack_set_sync_callback() failed\n", HL_ERROR_PREFIX);
        }
    }

    if (cb_flags & JACK_TIMEBASE_CALLBACK)
    {
        if (jack_set_timebase_callback(client, 1, _timebase_callback, this) != 0)
        {
            fprintf(stderr, "%sUnable to take over JACK timebase.\n", HL_ERROR_PREFIX);
        }
    }

    jack_activate(client);

    // Automatically connect our outputs to our inputs so that we can loopback
    for (int i = 0; i < nAudioIn && i < nAudioOut; i++)
    {
        int ret = jack_connect(client, jack_port_name(audioOut[i]), jack_port_name(audioIn[i]));
        if (ret != 0 && ret != EEXIST)
        {
            fprintf(stderr, "%sUnable to connect %s to %s. Error code: %d\n", HL_ERROR_PREFIX, jack_port_name(audioOut[i]), jack_port_name(audioIn[i]), ret);
        }
    }
}

void JackClient::monitor()
{
    for (int i = 0; i < nAudioIn && i < nAudioOut; i++)
    {
        int ret = jack_connect(client, jack_port_name(audioOut[i]), (i % 2 == 0) ? "system:playback_1" : "system:playback_2");
        if (ret != 0 && ret != EEXIST)
        {
            fprintf(stderr, "%sUnable to connect %s to %s. Error code: %d\n", HL_ERROR_PREFIX, jack_port_name(audioOut[i]), (i % 2 == 0) ? "system:playback_1" : "system:playback_2", ret);
        }
    }
}

// Jack APIs
// Transport APIs
void JackClient::transport_start()
{
    jack_transport_start(client);
}

void JackClient::transport_stop()
{
    jack_transport_stop(client);
}

jack_transport_state_t JackClient::transport_query(jack_position_t * pos)
{
    return jack_transport_query(client, pos);
}

int JackClient::transport_reposition(const jack_position_t * pos)
{
    return jack_transport_reposition(client, pos);
}
