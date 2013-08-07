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

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <linux/videodev2.h>
#include "errno.h"
#include "setting.h"
#include "device.h"
#include "v4l1.h"
#include "v4l2.h"

using namespace std;

int Device::gain = -1;
int Device::shutter = -1;

Device::Device() 
{
    isOpen=false;
    adjustColors=false;
}

Device::~Device() 
{
//    if( isMapped )
//        munmap( NULL, mmap_size );
    if (v4l)
        delete v4l;
    if ( isOpen )
        close( fd ); 
}

bool Device::openDevice( unsigned int &width, unsigned int &height, int format ) 
{
    frameFormat=format;
    v.frame=0;
    
    if ( isOpen ) {
        close( fd );
    }
    
    std::string device = Setting::GetInstance()->GetDeviceFile();
    fd=open( device.c_str(), O_RDWR );  // open video device
    if ( fd<0 ) {
        isOpen=false;
        perror( device.c_str() );
        return false;
    }
    
    isOpen = true;
    
    std::string driver = Setting::GetInstance()->GetDriverAPI();
    if(driver == "Video4Linux1") {
        cout<<"Using video4linux 1 API"<<endl;
        v4l = new VideoForLinux1();
        isV4L2 = false;
    }
    else if(driver == "Video4Linux2") {
        cout<<"Using video4linux 2 API"<<endl;
        v4l = new VideoForLinux2();
        isV4L2 = true;
    }
    else {
        cout<<"Determining video4linux API version..."<<endl;
        struct v4l2_capability cap;    
        if( /*!isPWC() && */queryCapabilities(&cap) ) {
            cout<<"Using video4linux 2 API"<<endl;
            v4l = new VideoForLinux2();
            isV4L2 = true;
        }
        else {
            cout<<"Using video4linux 1 API"<<endl;
            v4l = new VideoForLinux1();
            isV4L2 = false;
        }
    }

    if(isV4L2) {
        struct v4l2_frmsizeenum frmsize;
        memset(&frmsize, 0, sizeof(struct v4l2_frmsizeenum));
        /*if(ioctl( fd, VIDIOC_ENUM_FRAMESIZES, &frmsize)) {
            perror( "VIDIOC_ENUM_FRAMESIZES" );
            isV4L2 = false; // I will get resolution in v4l1 way
        }*/
    }

    if( !width || !height ) { //setting max resolution
        if(!isV4L2) {
            struct video_capability caps;
            if( queryCapabilities(&caps) ) {
                width = caps.maxwidth;
                height = caps.maxheight;
            }
            else {
                width = 160;
                height = 120;
            }
            cout<<"Resolution setted to "<<width<<"x"<<height<<endl;
            ostringstream s;
            s<<width<<"x"<<height;
            Setting::GetInstance()->SetResolution(s.str());
	}
        else {
            int maxW = -1, maxH = -1;
            int resw, resh;
            struct v4l2_format fmt;

            std::stringstream ss;
            std::string str;

            resw = 160;
            resh = 120;

            while (resw < 3000) {
                memset(&fmt, 0, sizeof (struct v4l2_format));
                fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

                fmt.fmt.pix.width = resw;
                fmt.fmt.pix.height = resh;
                fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
                fmt.fmt.pix.pixelformat = static_cast<VideoForLinux2*> (v4l)->getPixelFormat();

                if (ioctl(fd, VIDIOC_TRY_FMT, &fmt) == -1) {
                    perror("VIDIOC_TRY_FMT");
                    printf("Resolution %dx%d not valid\n", resw, resh);
                } else {
                    if ((fmt.fmt.pix.width == resw) && (fmt.fmt.pix.height == resh)) {
                        printf("Resolution %dx%d valid\n", fmt.fmt.pix.width, fmt.fmt.pix.height);                        
                        maxW = resw;
                        maxH = resh;
                    }
                }
                resh = resh * 1.5;
                resw = resh * 4 / (float) 3;
            }

            resw = 160;
            resh = 120;

            while (resw < 3000) {
                memset(&fmt, 0, sizeof (struct v4l2_format));
                fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

                fmt.fmt.pix.width = resw;
                fmt.fmt.pix.height = resh;
                fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
                fmt.fmt.pix.pixelformat = static_cast<VideoForLinux2*> (v4l)->getPixelFormat();

                if (ioctl(fd, VIDIOC_TRY_FMT, &fmt) == -1) {
                    perror("VIDIOC_TRY_FMT");
                    printf("Resolution %dx%d not valid\n", resw, resh);
                } else {
                    if ((fmt.fmt.pix.width == resw) && (fmt.fmt.pix.height == resh)) {
                        printf("Resolution %dx%d valid\n", fmt.fmt.pix.width, fmt.fmt.pix.height);                        
                        maxW = resw;
                        maxH = resh;
                    }
                }
                resh = resh * 2;
                resw = resh * 4 / (float) 3;
            }


            resw = 160;
            resh = 90;

            while (resw < 3000) {
                memset(&fmt, 0, sizeof (struct v4l2_format));
                fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

                fmt.fmt.pix.width = resw;
                fmt.fmt.pix.height = resh;
                fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
                fmt.fmt.pix.pixelformat = static_cast<VideoForLinux2*> (v4l)->getPixelFormat();

                if (ioctl(fd, VIDIOC_TRY_FMT, &fmt) == -1) {
                    perror("VIDIOC_TRY_FMT");
                    printf("Resolution %dx%d not valid\n", resw, resh);
                } else {
                    if ((fmt.fmt.pix.width == resw) && (fmt.fmt.pix.height == resh)) {
                        printf("Resolution %dx%d valid\n", fmt.fmt.pix.width, fmt.fmt.pix.height);                        
                        maxW = resw;
                        maxH = resh;
                    }
                }
                resh = resh * 1.5;
                resw = resh * 16 / (float) 9;
            }

            resw = 160;
            resh = 90;

            while (resw < 3000) {
                memset(&fmt, 0, sizeof (struct v4l2_format));
                fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

                fmt.fmt.pix.width = resw;
                fmt.fmt.pix.height = resh;
                fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
                fmt.fmt.pix.pixelformat = static_cast<VideoForLinux2*> (v4l)->getPixelFormat();

                if (ioctl(fd, VIDIOC_TRY_FMT, &fmt) == -1) {
                    perror("VIDIOC_TRY_FMT");
                    printf("Resolution %dx%d not valid\n", resw, resh);
                } else {
                    if ((fmt.fmt.pix.width == resw) && (fmt.fmt.pix.height == resh)) {
                        printf("Resolution %dx%d valid\n", fmt.fmt.pix.width, fmt.fmt.pix.height);                        
                        maxW = resw;
                        maxH = resh;
                    }
                }
                resh = resh * 2;
                resw = resh * 16 / (float) 9;
            }
            ostringstream s;
            if(maxW == -1) {
                s<<640<<"x"<<480;
                Setting::GetInstance()->SetResolution(s.str());

                width = 640;
                height = 480;

            } else {
                s<<maxW<<"x"<<maxH;
                Setting::GetInstance()->SetResolution(s.str());

                width = maxW;
                height = maxH;
            }
	}
    }


    v4l->setParameters(fd, format);
    mMap( width, height );        
    return true;
}

void Device::setAdjustColors(bool b)
{
    if(v4l)
        v4l->setAdjustColors(b);    
}

bool Device::mMap(int w, int h)
{
    if(v4l)
        return v4l->mMap(w, h, 0);
    else
        return false;
}

bool Device::getFrame(char **buffer)
{
    if(v4l)
        return v4l->getFrame(buffer);
    else
        return false;
}

bool Device::setResolution(unsigned int width, unsigned int height, unsigned int frameRate)
{
    if(v4l)
        return v4l->setResolution(width, height, frameRate);
    else
        return false;
}

bool Device::getResolution(unsigned int &width, unsigned int &height, unsigned int &frameRate)
{
    if(v4l)
        return v4l->getResolution(width, height, frameRate);
    else
        return false;
}

bool Device::setBrightness(unsigned int value)
{
    if(v4l)
        return v4l->setBrightness(value);
    else
        return false;
}

bool Device::getBrightness(unsigned int &value)
{
    if(v4l)
        return v4l->getBrightness(value);
    else
        return false;
}

bool Device::setContrast(unsigned int value)
{
    if(v4l)
        return v4l->setContrast(value);
    else
        return false;
}

bool Device::getContrast(unsigned int &value)
{
    if(v4l)
        return v4l->getContrast(value);
    else
        return false;
}

bool Device::setGamma(unsigned int value)
{
    if(v4l)
        return v4l->setGamma(value);
    else
        return false;
}

bool Device::getGamma(unsigned int &value)
{
    if(v4l)
        return v4l->getGamma(value);
    else
        return false;
}

bool Device::setSaturation(unsigned int value)
{
    if(v4l)
        return v4l->setSaturation(value);
    else
        return false;
}

bool Device::getSaturation(unsigned int &value)
{
    if(v4l)
        return v4l->getSaturation(value);
    else
        return false;
}

bool Device::setVideoStd(video_std value)
{
    if(v4l)
        return v4l->setVideoStd(value);
    else
        return false;
}

bool Device::queryCapabilities( struct video_capability *caps ) /*VIDIOCGCAP*/ 
{
    if (ioctl(fd, VIDIOCGCAP, caps) < 0) {
        perror("VIDIOCGCAP");
        /*v4l2_capability v2caps;
        if (ioctl(fd, VIDIOC_QUERYCAP, &v2caps) == -1) {
            if (EINVAL == errno) {                
                return false;
            }
            else {
                perror("VIDIOC_QUERYCAP");
                return false;
            }
        }
        strcpy(caps->name, (char*)(v2caps.card));*/
        return false;
    }
    return true;
}

bool Device::queryCapabilities(struct v4l2_capability *cap) /*VIDIOC_QUERYCAP*/
{
    std::string dev_name = Setting::GetInstance()->GetDeviceFile();
    if (ioctl (fd, VIDIOC_QUERYCAP, cap) == -1) {
        if (EINVAL == errno) {
            fprintf (stderr, "%s is not a V4L2 device\n",
                     dev_name.c_str());
            return false;
        } 
        else {
            perror("VIDIOC_QUERYCAP");
            return false;
        }
    }
    
    if (!(cap->capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf (stderr, "%s is no video capture device\n",
                dev_name.c_str());
        return false;
    }
    return true;
}

bool Device::getResolutionList(wxArrayString &validResolution) 
{
    if(!isV4L2) {
        struct video_capability caps;
        int minw, minh, maxw, maxh;

        queryCapabilities( &caps );

        minw = caps.minwidth;
        minh = caps.minheight;
        maxw = caps.maxwidth;
        maxh = caps.maxheight;

        int currentw = maxw;
        int currenth = maxh;
        std::stringstream ss;
        std::string str;

        while ( ( currentw >= minw ) && ( currenth >= minh ) ) {
            ss.str( "" );
            ss<<currentw<<"x"<<currenth;
            str = ss.str();
            validResolution.Add( wxString(str.c_str(), wxConvUTF8) );
            currentw /= 2;
            currenth /= 2;
        }
    }
    else {
        /*struct v4l2_frmsizeenum frmsize;
        memset(&frmsize, 0, sizeof(struct v4l2_frmsizeenum));
        if(ioctl( fd, VIDIOC_ENUM_FRAMESIZES, &frmsize)) {
            perror( "VIDIOC_ENUM_FRAMESIZES" );
            return false;
        }
        std::stringstream ss;
        std::string str;
        ss<<frmsize.discrete.width<<"x"<<frmsize.discrete.height;
        str = ss.str();
        validResolution.Add( wxString(str.c_str(), wxConvUTF8) );
        frmsize.index++;        
        while(!ioctl( fd, VIDIOC_ENUM_FRAMESIZES, &frmsize)) {
            ss.str( "" );
            ss<<frmsize.discrete.width<<"x"<<frmsize.discrete.height;
            str = ss.str();            
            validResolution.Add( wxString(str.c_str(), wxConvUTF8) );
            frmsize.index++;
        }*/
        int resw, resh;        
        struct v4l2_format fmt;

        std::stringstream ss;
        std::string str;

        resw = 160;
        resh = 120;        

        while(resw < 3000) {
            memset(&fmt, 0, sizeof(struct v4l2_format));
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            fmt.fmt.pix.width = resw;
            fmt.fmt.pix.height = resh;
            fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
            fmt.fmt.pix.pixelformat = static_cast<VideoForLinux2*>(v4l)->getPixelFormat();

            if (ioctl (fd, VIDIOC_TRY_FMT, &fmt) == -1) {
                perror("VIDIOC_TRY_FMT");
                printf("Resolution %dx%d not valid\n", resw, resh);
            } else {
                if ((fmt.fmt.pix.width == resw) && (fmt.fmt.pix.height == resh)) {
                    printf("Resolution %dx%d valid\n", fmt.fmt.pix.width, fmt.fmt.pix.height);                    
                    ss.str("");
                    ss << resw << "x" << resh;
                    str = ss.str();
                    validResolution.Add(wxString(str.c_str(), wxConvUTF8));
                }
            }            
            resh = resh * 1.5;
            resw = resh * 4 / (float)3;
        }

        resw = 160;
        resh = 120;

        while(resw < 3000) {
            memset(&fmt, 0, sizeof(struct v4l2_format));
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            fmt.fmt.pix.width = resw;
            fmt.fmt.pix.height = resh;
            fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
            fmt.fmt.pix.pixelformat = static_cast<VideoForLinux2*>(v4l)->getPixelFormat();

            if (ioctl (fd, VIDIOC_TRY_FMT, &fmt) == -1) {
                perror("VIDIOC_TRY_FMT");
                printf("Resolution %dx%d not valid\n", resw, resh);
            } else {
                if ((fmt.fmt.pix.width == resw) && (fmt.fmt.pix.height == resh)) {
                    printf("Resolution %dx%d valid\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
                    ss.str("");
                    ss << resw << "x" << resh;
                    str = ss.str();
                    validResolution.Add(wxString(str.c_str(), wxConvUTF8));
                }
            }
            resh = resh * 2;
            resw = resh * 4 / (float)3;
        }


        resw = 160;
        resh = 90;

        while(resw < 3000) {
            memset(&fmt, 0, sizeof(struct v4l2_format));
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            fmt.fmt.pix.width = resw;
            fmt.fmt.pix.height = resh;
            fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
            fmt.fmt.pix.pixelformat = static_cast<VideoForLinux2*>(v4l)->getPixelFormat();

            if (ioctl (fd, VIDIOC_TRY_FMT, &fmt) == -1) {
                perror("VIDIOC_TRY_FMT");
                printf("Resolution %dx%d not valid\n", resw, resh);
            } else {
                if ((fmt.fmt.pix.width == resw) && (fmt.fmt.pix.height == resh)) {
                    printf("Resolution %dx%d valid\n", fmt.fmt.pix.width, fmt.fmt.pix.height);                    
                    ss.str("");
                    ss << resw << "x" << resh;
                    str = ss.str();
                    validResolution.Add(wxString(str.c_str(), wxConvUTF8));
                }
            }
            resh = resh * 1.5;
            resw = resh * 16 / (float)9;
        }

        resw = 160;
        resh = 90;

        while(resw < 3000) {
            memset(&fmt, 0, sizeof(struct v4l2_format));
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            fmt.fmt.pix.width = resw;
            fmt.fmt.pix.height = resh;
            fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
            fmt.fmt.pix.pixelformat = static_cast<VideoForLinux2*>(v4l)->getPixelFormat();

            if (ioctl (fd, VIDIOC_TRY_FMT, &fmt) == -1) {
                perror("VIDIOC_TRY_FMT");
                printf("Resolution %dx%d not valid\n", resw, resh);
            } else {
                if ((fmt.fmt.pix.width == resw) && (fmt.fmt.pix.height == resh)) {
                    printf("Resolution %dx%d valid\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
                    ss.str("");
                    ss << resw << "x" << resh;
                    str = ss.str();
                    validResolution.Add(wxString(str.c_str(), wxConvUTF8));
                }
            }
            resh = resh * 2;
            resw = resh * 16 / (float)9;
        }
    }
}

bool Device::getGain( int *agc ) /* VIDIOCPWCGAGC*/ 
{
    if (gain != -1)
        *agc = gain;
    else {
        if (ioctl(fd, VIDIOCPWCGAGC, agc)) {
            perror("VIDIOCPWCGAGC");
            return false;
        }
    }
    return true;
}

bool Device::setGain( int *agc ) /* VIDIOCPWCSAGC*/ {
    if ( ioctl( fd, VIDIOCPWCSAGC, agc ) ) {
        perror( "VIDIOCPWCSAGC" );
        return false;
    }
    gain = *agc;
    return true;
}

bool Device::getShutter( int *sh ) /* VIDIOCPWCSSHUTTER*/ {
    if (shutter != -1)
        *sh = shutter;
    else {
        if (ioctl(fd, VIDIOCPWCSSHUTTER, sh)) {
            perror("VIDIOCPWCGSHUTTER");
            return false;
        }
    }
    return true;
}

bool Device::setShutter( int *sh ) /* VIDIOCPWCSSHUTTER*/ 
{
    if ( ioctl( fd, VIDIOCPWCSSHUTTER, sh ) ) {
        perror( "VIDIOCPWCSSHUTTER" );
        return false;
    }
    shutter = *sh;
    return true;
}

bool Device::isPWC() 
{
    if ( isOpen ) {
        struct video_capability vcap;
        if ( !ioctl( fd, VIDIOCGCAP, &vcap ) < 0 )
            return false;
        if ( std::string( vcap.name ).find( "Philips" ) != std::string::npos )
            return true;
        else {
            /* No match yet; try the PROBE */
            struct pwc_probe probe;
            if ( ioctl( fd, VIDIOCPWCPROBE, &probe ) == 0 ) {
                if ( strcmp( vcap.name, probe.name ) ) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Device::closeDevice() 
{
    if ( isOpen ) {
        close( fd );
        isOpen=false;
    }
}
