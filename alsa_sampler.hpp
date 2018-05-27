#pragma once
#include <alsa/asoundlib.h>
#include "alsa_sampler.hpp"
#include <stdexcept>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <cstdio>
#include <alsa/error.h>
#include <array>

static std::string format_string(const char *fmt, ...) {
    char buf[1024];
    va_list vl;
    va_start(vl, fmt);
    vsnprintf(buf, 1024, fmt, vl);
    va_end(vl);
    buf[sizeof(buf) - 1] = 0;
    return std::string(buf);
}


template <unsigned numOfSamples> class alsa_sampler {
    std::array<float, numOfSamples * 32 *2 / 8> buffer; // 32 = snd_pcm_format_width(SND_PCM_FORMAT_FLOAT)
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;

    static const char default_device[];

public:

    alsa_sampler(const unsigned int sample_rate, const char* device_name = alsa_sampler<numOfSamples>::default_device)
    {
        int err;

        if ((err = snd_pcm_open (&capture_handle, device_name, SND_PCM_STREAM_CAPTURE, 0)) < 0)
            throw std::runtime_error(format_string("cannot open audio device %s (%s)\n", device_name, snd_strerror (err)));

        std::cout << "audio interface opened" << std::endl;

        if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0)
            throw std::runtime_error(format_string("cannot allocate hardware parameter structure (%s)\n", snd_strerror (err)));

        std::cout << "hw_params allocated" << std::endl;

        if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0)
            throw std::runtime_error(format_string("cannot initialize hardware parameter structure (%s)\n", snd_strerror (err)));

        std::cout << "hw_params initialized" << std::endl;

        if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
            throw std::runtime_error(format_string("cannot set access type (%s)\n", snd_strerror (err)));

        std::cout << "hw_pareams access setted" << std::endl;

        if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_FLOAT)) < 0)
            throw std::runtime_error(format_string("cannot set sample format (%s)\n", snd_strerror (err)));

        std::cout << "hw_params format setted" << std::endl;

        unsigned int s_rate = sample_rate;
        if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &s_rate, 0)) < 0)
            throw std::runtime_error(format_string("cannot set sample rate (%s)\n", snd_strerror (err)));

        std::cout << "hw_params rate setted" << std::endl;

        if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0)
            throw std::runtime_error(format_string("cannot set channel count (%s)\n", snd_strerror (err)));

        std::cout << "hw_params channels setted" << std::endl;

        if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0)
            throw std::runtime_error(format_string("cannot set parameters (%s)\n", snd_strerror (err)));

        std::cout << "hw_params setted" << std::endl;

        snd_pcm_hw_params_free (hw_params);

        std::cout << "hw_params freed" << std::endl;

        if ((err = snd_pcm_prepare (capture_handle)) < 0)
            throw std::runtime_error(format_string("cannot prepare audio interface for use (%s)\n", snd_strerror (err)));

        std::cout << "audio interface prepared" << std::endl;

        std::cout << "buffer allocated" << std::endl;
    }


    virtual ~alsa_sampler() {
        snd_pcm_close (capture_handle);
        std::cout << "audio interface closed" << std::endl;
    }

    virtual void fillBuffer() {
        int err;

        if ((err = snd_pcm_readi (capture_handle, buffer.begin(), numOfSamples)) != numOfSamples)
            throw std::runtime_error(format_string("read from audio interface failed (%s)\n", snd_strerror (err)));

        for(int i=0; i<numOfSamples*2; i+=2) { //Converting stereo matrix to mono vector
            buffer[i/2]=(buffer[i]+buffer[i-1])/2;
        }
    }


    virtual float* getBuffer() {
        return buffer.begin();
    }
};


//template <unsigned numOfSamples> const char alsa_sampler<numOfSamples>::default_device[] = "front:CARD=XFi,DEV=0";

template <unsigned numOfSamples> const char alsa_sampler<numOfSamples>::default_device[] = "default";
