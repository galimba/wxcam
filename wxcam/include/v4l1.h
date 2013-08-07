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

#ifndef _VideoForLinux1_H
#define	_VideoForLinux1_H

#include "v4l.h"
#include <string>

class VideoForLinux1 : public VideoForLinux
{   
public:    
    VideoForLinux1();
    ~VideoForLinux1();
    
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
        
private:
    std::string numpal2string(int num);
    bool getPictureFunctions(struct video_picture *p); /*VIDIOCGPICT*/
    bool getWindowOption(struct video_window *vw); /*VIDIOCGWIN*/
    bool getMmapSize(struct video_mbuf *vm); /*VIDIOCGMBUF*/ 
           
private:
    struct video_mmap v;
    int frameCount;
    int *frameOffsets;
    int framePending;
            
};

#endif	//_VideoForLinux1_H
