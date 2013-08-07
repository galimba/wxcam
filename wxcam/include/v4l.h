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

#ifndef _VideoForLinux_H
#define	_VideoForLinux_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#ifdef HAVE_CAMV4L 
#include <linux/videodev.h>
#else
#include <libv4l1-videodev.h>
#endif
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define VIDEO_PALETTE_JPEG 21
#define VIDEO_PALETTE_MJPEG 22

const unsigned int PALETTE_NUMBER = 7;

enum video_std {
    MODE_PAL,
    MODE_NTSC,
    MODE_SECAM,
    NONE
};

class VideoForLinux
{   
public:    
    
    VideoForLinux()
    {
        isMapped=false;
        canMapDevice=true;
        adjustColors=false;
        video_palette[0] = VIDEO_PALETTE_RGB24;
        video_palette[1] = VIDEO_PALETTE_RGB32;
        video_palette[2] = VIDEO_PALETTE_YUV420P;
        video_palette[3] = VIDEO_PALETTE_YUV420;
        video_palette[4] = VIDEO_PALETTE_YUYV;
        video_palette[5] = VIDEO_PALETTE_JPEG;
        video_palette[6] = VIDEO_PALETTE_MJPEG;
        map=NULL;
    }
    
    virtual ~VideoForLinux() {};
    
    void setParameters(int fd, int format)
    {
        this->fd=fd;
        frameFormat=format;
        this->adjustColors=adjustColors;
    }
    
    void setAdjustColors(bool adjustColors)
    {
        this->adjustColors=adjustColors;
    }
    
    virtual bool mMap(int w, int h, int frameRate=0) = 0;
    virtual bool getFrame(char **buffer) = 0; 
    virtual bool setResolution(unsigned int width, unsigned int height, unsigned int frameRate=0) = 0;
    virtual bool getResolution(unsigned int &width, unsigned int &height, unsigned int &frameRate) = 0;
    virtual bool setBrightness(unsigned int value) = 0;
    virtual bool getBrightness(unsigned int &value) = 0;
    virtual bool setContrast(unsigned int value) = 0;
    virtual bool getContrast(unsigned int &value) = 0;
    virtual bool setGamma(unsigned int value) = 0;
    virtual bool getGamma(unsigned int &value) = 0;
    virtual bool setSaturation(unsigned int value) = 0;
    virtual bool getSaturation(unsigned int &value) = 0;
    virtual bool setVideoStd(video_std std) = 0;
    
protected:    
    int video_palette[PALETTE_NUMBER];
    int fd;
    bool isMapped;
    bool canMapDevice;
    int mmap_size;
    int frameFormat;
    int width;
    int siz;
    bool adjustColors;   
    void *map;
            
};

#endif	//_VideoForLinux_H

