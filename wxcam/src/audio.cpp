/***************************************************************************
 *   Copyright (C) 2007 by Marco Lorrai                                    *
 *   marco.lorrai@abbeynet.it                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "audio.h"
#include "setting.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

Audio::Audio() 
{
    end=1;    
}

Audio::~Audio()
{
    
}

int Audio::Open()
{
    /* Open PCM device for recording (capture). */
    rc = snd_pcm_open(&handle, "default",
            SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        return 1;
    }

    return 0;
}

int Audio::startAcquisition(int channels, int rate)
{
    this->channels = channels;
    this->rate = rate;
    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(handle, params,
            SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(handle, params,
            SND_PCM_FORMAT_S16_LE);

    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(handle, params, channels);

    /* 44100 bits/second sampling rate (CD quality) */
    val = rate;
    snd_pcm_hw_params_set_rate_near(handle, params,
            &val, &dir);

    /* Set period size to 32 frames. */
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle,
            params, &frames, &dir);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr,
                "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params,
            &frames, &dir);
    size = frames * 2 * channels; /* 2 bytes/sample, 2 channels */
    buffer = (unsigned char *) malloc(size);

    snd_pcm_hw_params_get_period_time(params,
            &val, &dir);
    return size;
}

void* Audio::Entry()
{    
    while (true) {
        rc = snd_pcm_readi(handle, buffer, frames);
        if (rc == -EPIPE) {
            /* EPIPE means overrun */
            fprintf(stderr, "overrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr,
                    "error from read: %s\n",
                    snd_strerror(rc));
        } else if (rc != (int) frames) {
            fprintf(stderr, "short read, read %d frames\n", rc);
        }        

        mutex.Lock();
        if (!end) {
            mutex.Unlock();
            break;
        }
        mutex.Unlock();                

        //queing frames
        if (rc > 0) {
            mutex.Lock();
            queue.push_back(std::string((char*) buffer, rc * 2 * channels));
            mutex.Unlock();
        }
        
    }
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);
}

std::vector<std::string> Audio::getAudioFrames()
{
    mutex.Lock();
    std::vector<std::string> ret = queue;    
    queue.clear();
    mutex.Unlock();
    return ret;
}

void Audio::stopAcquisition()
{
    mutex.Lock();
    end = 0;
    mutex.Unlock();
}


