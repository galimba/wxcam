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

#include "xvidcodec.h"
#include <iostream>
#include <wx/event.h>
#include "frame.h"
#include "audio.h"

#define CHANNELS 2
#define RATE 44100

using namespace std;

Xvid::Xvid() : cond(mutex) 
{
    frameNumber=0; 
    audioFramesRecorded=0;
    compression=0.5f;
    audio=NULL;
    ignoreVideo = false;
    endRecord = false;
    max=0;
}

Xvid::~Xvid()
{
}

int Xvid::setup()
{
    if (REVEL_API_VERSION != Revel_GetApiVersion())
    {
        cout<<"ERROR: Revel version mismatch!"<<endl;
        printf("Headers: version %06x, API version %d\n", REVEL_VERSION,
            REVEL_API_VERSION);
        printf("Library: version %06x, API version %d\n", Revel_GetVersion(),
            Revel_GetApiVersion());
        return 1;
    }     
           
    revError = Revel_CreateEncoder(&encoderHandle);
    if (revError != REVEL_ERR_NONE)
    {
        printf("Revel Error while creating encoder: %d\n", revError);
        return 1;
    }
    
    Revel_Params revParams;
    Revel_InitializeParams(&revParams);
    revParams.width = width;
    revParams.height = height;
    revParams.frameRate = fps;
    revParams.quality = compression;
    revParams.codec = REVEL_CD_XVID;

    hasAudio = checkAudio(&revParams);

    revError = Revel_EncodeStart(encoderHandle, filename.c_str(), &revParams);
    if (revError != REVEL_ERR_NONE)
    {
        printf("Revel Error while starting encoding: %d\n", revError);
        return 1;
    }                  
    
    revelFrame.width = width;
    revelFrame.height = height;
    revelFrame.bytesPerPixel = 4;
    revelFrame.pixelFormat = REVEL_PF_RGBA;
    revelFrame.pixels = new int[width*height];
    memset(revelFrame.pixels, 0, width*height*4);        
    
    return 0;
}

void Xvid::addFrame(const char* frame)
{
    if(!frameNumber) {
        if(hasAudio) {
            audioBufferSize = audio->startAcquisition(CHANNELS, RATE);
            if (audio->Create() != wxTHREAD_NO_ERROR ) {
                cout<<"Can't create audio recording thread!";
                return;
            }        
            audio->Run();
            addSilence(Setting::GetInstance()->GetSilence());
        }
    }

    mutex.Lock();
    queue.push_back(std::string(frame, frameSize));
        
        /*revError = Revel_EncodeFrame(encoderHandle, &revelFrame, &frameSize);
        if (revError != REVEL_ERR_NONE)
            printf("Revel Error while writing frame: %d\n", revError);*/
    
    
    /*encoding audio*/    
    if(hasAudio) {        
        std::vector<std::string> vectAudio = audio->getAudioFrames();
        audio_queue.push_back(vectAudio);
        /*int size = vectAudio.size();
        std::vector<std::string>::iterator it = vectAudio.begin();
        for(int i=1; i<=size; i++) {                       
            std::string buffer = *it;  
            ++it;
            audioFramesRecorded++;
            revError = Revel_EncodeAudio(encoderHandle, (void*)buffer.c_str(), buffer.size(), &totalAudioBytes);
            if (revError != REVEL_ERR_NONE) {
                printf("Revel Error while writing audio: %d\n", revError);
            }            
        }
        */

    }
    cond.Signal();
    mutex.Unlock();
    frameNumber++;
}

void* Xvid::Entry() {
    while(true) {
        mutex.Lock();
        while( (!queue.size() || !audio_queue.size())&& !endRecord)
            cond.Wait();
        //cout<<"Queue size: "<<queue_size<<endl<<flush;
        if(endRecord && (!queue.size() || !audio_queue.size())) { //registration terminated
            mutex.Unlock();
            
            break;
        }

        string videoFrame = queue.front();
        queue.pop_front();
        vector<string> audioFrames;        
        if(hasAudio) {
            audioFrames = audio_queue.front();
            audio_queue.pop_front();
        }

        //max not yet initialize, first execution after calling record()
        if(endRecord && !max) {
            max = queue.size();
            if(max > 10) {
                wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, PROGRESS_DIALOG_START );
                wxPostEvent( parent, event );
            }
            else
                max=-1;
        }

        //max already initialized
        if(endRecord && max > 0) {
            wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, PROGRESS_DIALOG_EVENT );
            int n = (int)(((float)max - (float)queue.size()) / (float)max * 100);
            event.SetInt( n );

            // send in a thread-safe way
            wxPostEvent( parent, event );
        }

        mutex.Unlock();

        int frameSize;
        const char* frame = videoFrame.c_str();        
        //writing frame...
        memcpy4( (char*)revelFrame.pixels, frame, width*height*4 );
        revError = Revel_EncodeFrame(encoderHandle, &revelFrame, &frameSize);
        if (revError != REVEL_ERR_NONE)
            printf("Revel Error while writing frame: %d\n", revError);

        if (hasAudio) {
            int totalAudioBytes;
            int size = audioFrames.size();
            std::vector<std::string>::iterator it = audioFrames.begin();
            for (int i = 1; i <= size; i++) {
                std::string buffer = *it;
                ++it;
                audioFramesRecorded++;
                revError = Revel_EncodeAudio(encoderHandle, (void*) buffer.c_str(), audioBufferSize, &totalAudioBytes);
                if (revError != REVEL_ERR_NONE) {
                    printf("Revel Error while writing audio: %d\n", revError);
                }
                //printf("Recorded %d audio frames total\n", size);
            }
        }
    }
    if(max > 0) {
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, PROGRESS_DIALOG_EVENT );

        event.SetInt(-1); // that's all
        wxPostEvent( parent, event );
    }
    wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, STOP_REC );
    event.SetInt(0);
    wxPostEvent( parent, event );
    return NULL;
}

bool Xvid::checkAudio(Revel_Params *revParams)
{
    int ret;

    hasAudio = true;

    if (hasAudio) {
        audio = new Audio();
        ret = audio->Open();
        if (ret < 0) {
            hasAudio = false;
            /*error to screen*/
            wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, AUDIO_ERR);
            event.SetInt(1);
            wxPostEvent(parent, event);
            audio->Delete();
            audio = NULL;
        } else {
            revParams->audioChannels = CHANNELS;
            revParams->audioRate = RATE;
            revParams->audioBits = 16;
            revParams->audioSampleFormat = REVEL_ASF_PCM;
        }
        revParams->hasAudio = hasAudio;
    }

    return hasAudio;
}

void Xvid::record()
{
    /*stopping audio acquisition*/
    if(hasAudio) {
        audio->stopAcquisition();
        audio->Wait();
    }
    
    int totalSize;
    revError = Revel_EncodeEnd(encoderHandle, &totalSize);
    if (revError != REVEL_ERR_NONE)
    {
        printf("Revel Error while ending encoding: %d\n", revError);
        exit(1);
    }
    printf("%s written: %dx%d, %d bytes\n", filename.c_str(), width, height, totalSize);

    // Final cleanup.
    Revel_DestroyEncoder(encoderHandle);
    delete [] (int*)revelFrame.pixels;
    if(audio)
        audio->Delete(); 
    wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, STOP_REC );
    event.SetInt(0);
    wxPostEvent( parent, event );
}

void Xvid::memcpy4(char* dest, const char* src, int size)
{
    int j = 0;
    for(int i=0; i<size; i = i + 4) {
        memcpy(dest + i, src + j, 3);
        dest[i+3] = 0;
        j = j + 3;
    }
}

void Xvid::addSilence(int hundredthsSecond)
{
    int audioBufferSize = hundredthsSecond*882*CHANNELS; //2 is the number of channels;
    int totalAudioBytes;
    char* buffer = new char[audioBufferSize];
    memset(buffer, 0, audioBufferSize);
    revError = Revel_EncodeAudio(encoderHandle, (void*)buffer, audioBufferSize, &totalAudioBytes);
    if (revError != REVEL_ERR_NONE) {
        printf("Revel Error while writing audio: %d\n", revError);
    }
    //else
        //printf("Silence added\n");
    delete [] buffer;
}

void Xvid::setCompressionLevel(float level)
{
    compression = level;
}

