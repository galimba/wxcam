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

#include <wx/thread.h>
#include <alsa/asoundlib.h>
#include <string>
#include <vector>

class Audio : public wxThread
{
public: 
    Audio();
    ~Audio();
    
    int Open();
    int startAcquisition(int channels = 2, int rate = 44100);
    void stopAcquisition();
    std::vector<std::string> getAudioFrames();
    
private:
    void *Entry();
    
private:    
    unsigned char *buffer;   
    int end;
    std::vector<std::string> queue;
    wxMutex mutex;

    int channels;
    int rate;
    int sampleSize;
    int rc;
    int size;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    snd_pcm_uframes_t frames;    
};
