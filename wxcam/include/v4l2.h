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

#ifndef _VideoForLinux2_H
#define	_VideoForLinux2_H

#include "v4l.h"
#include <string>

#define HEADERFRAME1 0xaf

struct buffer {
    void* start; 
    size_t length;
};


struct controls {
    int minbrightness, maxbrightness;
    int mincontrast, maxcontrast;
    int mingamma, maxgamma;
    int minsaturation, maxsaturation;
};

class VideoForLinux2 : public VideoForLinux
{   
public:    
    VideoForLinux2();
    ~VideoForLinux2();
    
    void closeDevice();
    bool mMap(int w, int h, int frameRate=0);
    bool getFrame(char **buffer);
    bool setResolution(unsigned int width, unsigned int height, unsigned int frameRate=0);
    bool getResolution(unsigned int &width, unsigned int &height, unsigned int &frameRate);
    bool setBrightness(unsigned int value);
    bool getBrightness(unsigned int &value);
    bool setContrast(unsigned int value);
    bool getContrast(unsigned int &value);
    bool setGamma(unsigned int value);
    bool getGamma(unsigned int &value); 
    bool setSaturation(unsigned int value);
    bool getSaturation(unsigned int &value); 
    bool setVideoStd(video_std std);

    int getPixelFormat() {return pixelFormat;}
        
private:    
    bool init_mmap();
    void setControls();
    void enumerateMenu(struct v4l2_queryctrl &queryctrl, struct v4l2_querymenu querymenu);
    void enumerateControls();
    
private:    
    bool controlsSetted;
    std::string dev_name;
    struct buffer* buffers;
    unsigned int n_buffers;
    int pixelFormat;
    unsigned int width, height;
    struct controls controls;
            
};

#endif //_VideoForLinux2_H
