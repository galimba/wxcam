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

#ifndef _device_H
#define	_device_H

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
#include <string.h>
#include "pwc-ioctl.h"
#include "v4l.h"
#include <wx/arrstr.h>

class Device
{
    public:
        Device();
        ~Device();
        
        void closeDevice();
        bool openDevice(unsigned int &width, unsigned int &height, int format);
        /*binded functions*/
        bool mMap(int w, int h);
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
        /*end binded functions*/
        bool queryCapabilities(struct video_capability *caps); /*VIDIOCGCAP*/
        bool queryCapabilities(struct v4l2_capability *cap); /*VIDIOC_QUERYCAPP*/
        bool getResolutionList(wxArrayString &validResolution);
        bool getGain(int *agc); /* VIDIOCPWCGAGC*/
        bool setGain(int *agc); /* VIDIOCPWCSAGC*/
        bool getShutter(int *shutter); /* VIDIOCPWCGSHUTTER*/
        bool setShutter(int *shutter); /* VIDIOCPWCSSHUTTER*/        
        bool isPWC();
        void setAdjustColors(bool);
        bool setVideoStd(video_std value);
        bool IsV4L2() { return true; }
        
    private:
        std::string numpal2string(int num);
    
    private:
        static int gain;
        static int shutter;
        bool adjustColors;
        bool isV4L2;
        int frameFormat;
        bool isOpen;
	int fd;
        struct video_mmap v;
        VideoForLinux *v4l;    
};

#endif	/* _device_H */
