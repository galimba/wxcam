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

#ifndef _xvid_H
#define	_xvid_H

#include "avi.h"
#include "revel.h"
#include "setting.h"
#include <sys/time.h>
#include <time.h>
#include <wx/thread.h>

#include <string>
#include <list>

class Audio;

class Xvid : public Avi
{
public:
    Xvid();
     ~Xvid();
    void addFrame(const char* frame);
    void record();
    void setCompressionLevel(float level);
    int  setup();
    
private:
    void *Entry();
    void memcpy4(char* dest, const char* src, int size);
    void addSilence(int hundredthsSecond);
    bool checkAudio(Revel_Params *revParams);
    
private:
    wxMutex mutex;
    wxCondition cond;
    std::list<std::string> queue;
    std::list< std::vector<std::string> > audio_queue;
    bool endRecord;
    int max;

    Audio *audio;
    int audioBufferSize;
    int frameNumber;
    int audioFramesRecorded;
    float compression;
    bool hasAudio;
    bool ignoreVideo;
    Revel_Error revError;
    Revel_VideoFrame revelFrame;
    int encoderHandle; 
    
};

#endif	/* _xvid */
