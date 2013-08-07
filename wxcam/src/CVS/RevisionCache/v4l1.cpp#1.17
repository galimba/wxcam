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

#include "v4l1.h"
#include "ccvt.h"
#include "filters.h"
#include "pwc-ioctl.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

VideoForLinux1::VideoForLinux1()
{
    /*static int frameOffsetDummy;
    // initialize with default configuration (one frame buffer)
    
    // frame offset of first frame is always 0
    frameOffsets=&frameOffsetDummy;
    frameOffsets[0]=0;*/
    frameCount=1;
    frameOffsets = NULL;
    // no frame is pending yet
    framePending=-1;
}
    
VideoForLinux1::~VideoForLinux1()
{
    if( isMapped )
        munmap( NULL, mmap_size );
}

bool VideoForLinux1::mMap(int w, int h, int frameRate)
{
    frameRate = 0; //ignored
    memset(&v, 0, sizeof(struct video_mmap));
    v.format=frameFormat;
    v.width=w;                     // from 32 to 924 (sometimes limited to 768)
    v.height=h;
    
    if(!frameFormat || frameFormat == VIDEO_PALETTE_JPEG) { //Determining frame format
        std::cout<<"Determining palette format..."<<std::endl;
        struct video_picture p;
        if (ioctl (fd, VIDIOCGPICT, &p) < 0) {
            perror ("VIDIOCGPICT");
            printf("Error getting frame format\n");
            return false;
        }
        for(int i=0; i<PALETTE_NUMBER; i++) {
            p.palette=video_palette[i];            
            if (!ioctl(fd, VIDIOCSPICT, &p)) {
                cout<<"Found palette "<<numpal2string(i)<<endl;
                v.format=frameFormat=video_palette[i];
                break;
            }
        }
    }
    
    if(!frameFormat) {
        printf("Error getting frame format\n");
        return false;
    }
    
    struct video_mbuf vm;
    if ( frameFormat==VIDEO_PALETTE_YUV420P || 
            frameFormat==VIDEO_PALETTE_YUV420)
        siz=v.width*v.height*3/2;
    else if(frameFormat==VIDEO_PALETTE_YUYV|| 
            frameFormat==VIDEO_PALETTE_UYVY)
        siz=v.width*v.height*2;
    else if( frameFormat==VIDEO_PALETTE_RGB24 )
        siz=v.width*v.height*3;
    else //VIDEO_PALETTE_RGB32
        siz=v.width*v.height*4;    
    if ( !getMmapSize( &vm ) ) {
        int maxw, maxh;
        struct video_capability caps;
        if ( ioctl( fd, VIDIOCGCAP, &caps ) <0 ) {
            perror( "VIDIOCGCAP" ); 
            caps.maxwidth = 1280;
            caps.maxheight = 1024;
        }
        maxw = caps.maxwidth;
        maxh = caps.maxheight;
        vm.size = maxw * maxh * 4;
    }
    if ( !map && canMapDevice ) {
        map=mmap( NULL, vm.size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );   // select memory-map
        if ( map==MAP_FAILED ) {
            canMapDevice=false;
            perror( "mmap" );
            //close( fd );
            return false;
        }
        else {
            mmap_size = vm.size;
            isMapped=true;
        }
    }
    return true;
}

bool VideoForLinux1::getFrame(char **buffer)
{
    const unsigned char* currentMap = (const unsigned char*) map;
    
    if ( isMapped ) {

        if (framePending==-1)
        {
            framePending=0;
            v.frame=0;
            if ( ioctl( fd, VIDIOCMCAPTURE, &v ) <0 ) {    // start capturing a frame
                perror( "VIDIOCMCAPTURE 1" );
                return false;
            }
        }
        
        int nextFrame = framePending+1;
        if (nextFrame>=frameCount)
            nextFrame=0;
        if (frameCount>1)
        {
            v.frame=nextFrame;
            if ( ioctl( fd, VIDIOCMCAPTURE, &v ) <0 ) {    // start capturing next frame
                perror( "VIDIOCMCAPTURE 2" );
                return false;
            }
        }
        
        v.frame=framePending;
        if ( ioctl( fd, VIDIOCSYNC, &v.frame ) <0 ) {     // wait for end of frame
            perror( "VIDIOCSYNC" );
            return false;
        }
        // get pointer to area with current frame
        currentMap = ((const unsigned char*) map)+frameOffsets[v.frame];

        // next frame is pending
        framePending=nextFrame;
    }
    else {
        framePending=0;
        map = ( char* ) malloc( siz );
        int len = read( fd, map, siz );
        if ( len < 0 ) {
            perror( "read" );
            free( map );
            return false;
        }
        currentMap = (const unsigned char*) map;
    }
    
    /*converting to colorspace RGB24*/
    
    /*VIDEO_PALETTE_YUV420P*/
    if ( frameFormat==VIDEO_PALETTE_YUV420P ) {
        *buffer = ( char* ) malloc( siz*2 ); //DON'T use new here!!
        if ( *buffer == NULL ) {
            perror( "Allocation error" );
            if ( !isMapped )
                free( map );
            return false;
        }
        //conversion to rgb 24
        if(adjustColors)
            ccvt_420p_bgr24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );
        else
            ccvt_420p_rgb24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );              
        
    }
    /*VIDEO_PALETTE_YUV420*/
    else if( frameFormat==VIDEO_PALETTE_YUV420 ) {
        *buffer = ( char* ) malloc( siz*2 ); //DON'T use new here!!
        if ( *buffer == NULL ) {
            perror( "Allocation error" );
            if ( !isMapped )
                free( map );
            return false;
        }
        //conversion to rgb 24
        if(adjustColors) 
            ccvt_420i_bgr24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );
        else 
            ccvt_420i_rgb24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );                              
    }
    /*VIDEO_PALETTE_YUYV*/
    else if( frameFormat==VIDEO_PALETTE_YUYV ) {
        *buffer = ( char* ) malloc( siz*3/2 ); //DON'T use new here!!
        if ( *buffer == NULL ) {
            perror( "Allocation error" );
            if ( !isMapped )
                free( map );
            return false;
        }
        //conversion to rgb 24
        if(adjustColors) 
            ccvt_yuyv_bgr24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );
        else 
            ccvt_yuyv_rgb24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );                              
    }
    /*VIDEO_PALETTE_UYVY*/
    else if( frameFormat==VIDEO_PALETTE_UYVY ) {
        *buffer = ( char* ) malloc( siz*3/2 ); //DON'T use new here!!
        if ( *buffer == NULL ) {
            perror( "Allocation error" );
            if ( !isMapped )
                free( map );
            return false;
        }
        //conversion to rgb 24
        if(adjustColors) 
            ccvt_uyvy_bgr24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );
        else 
            ccvt_uyvy_rgb24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );                              
    }    
    /*VIDEO_PALETTE_RGB24*/
    else if ( frameFormat==VIDEO_PALETTE_RGB24 ) {
        *buffer = ( char* ) malloc( siz );  //DON'T use new here!!
        if ( *buffer == NULL ) {
            perror( "Allocation error" );
            if ( !isMapped )
                free( map );
            return false;
        }
        memcpy( *buffer, currentMap, siz );
        if(!adjustColors)
            convert2bgr( (unsigned char*)*buffer, siz);   
    }
    /*VIDEO_PALETTE_RGB32*/
    else { 
        *buffer = ( char* ) malloc( (siz*3)/4 );  //DON'T use new here!!
        if ( *buffer == NULL ) {
            perror( "Allocation error" );
            if ( !isMapped )
                free( map );
            return false;
        }
        if(adjustColors)
            rgb32_bgr24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );
        else
            rgb32_rgb24( v.width, v.height, currentMap, ( unsigned char* ) *buffer );   
    }
    
    if ( !isMapped )
        free( map );
        
    if (frameCount==1)
    {
        if ( ioctl( fd, VIDIOCMCAPTURE, &v ) <0 ) {       // start capturing next frame
            perror( "VIDIOCMCAPTURE" );
            return false;
        }
    }
    
    return true;
}

bool VideoForLinux1::setVideoStd(video_std std)
{
    struct video_channel set;
    memset(&set, 0, sizeof(set));
    ioctl(fd, VIDIOCGCHAN, &set);
    
    if(std == MODE_PAL) 
        set.norm = VIDEO_MODE_PAL;
    else if(std == MODE_NTSC)
        set.norm = VIDEO_MODE_NTSC;
    else
        set.norm = VIDEO_MODE_SECAM;


    if (ioctl(fd, VIDIOCSCHAN, &set) == -1) {
        perror( "VIDIOCSCHAN" );
            return false;
    }
    return true;
}

bool VideoForLinux1::setResolution(unsigned int width, unsigned int height, unsigned int frameRate)
{
    struct video_window vw;
    if( !getWindowOption(&vw) )
        return false;
    vw.width=width;
    vw.height=height;
    if(frameRate) {
        vw.flags &= ~PWC_FPS_FRMASK;
        vw.flags |= ( frameRate << PWC_FPS_SHIFT );        
    }
    if ( ioctl( fd, VIDIOCSWIN, &vw ) <0 ) {
        perror( "VIDIOCSWIN" );
        return false;
    }
    width=vw.width;
    // invalidate pending frames since resolution changed
    framePending=-1;
    return mMap( vw.width, vw.height );    
}

bool VideoForLinux1::getResolution(unsigned int &width, unsigned int &height, unsigned int &frameRate)
{
    struct video_window vw;
    if( !getWindowOption(&vw) )
        return false;
    width=vw.width;
    height=vw.height;
    frameRate = ( vw.flags & PWC_FPS_FRMASK ) >> PWC_FPS_SHIFT;
    return true;        
}

bool VideoForLinux1::setBrightness(unsigned int value)
{
    struct video_picture p;
    if( !getPictureFunctions(&p) )
        return false;
    p.brightness=value;
    if ( ioctl( fd, VIDIOCSPICT, &p ) <0 ) {
        perror( "VIDIOCSPICT" );
        return false;
    }
    return true;
}

bool VideoForLinux1::getBrightness(unsigned int &value)
{    
    struct video_picture p;
    if ( ioctl( fd, VIDIOCGPICT, &p ) <0 ) {
        perror( "VIDIOCGPICT" );
        return false;
    }
    value=p.brightness;
    return true;
}

bool VideoForLinux1::setContrast(unsigned int value)
{
    struct video_picture p;
    if( !getPictureFunctions(&p) )
        return false;
    p.contrast=value;
    if ( ioctl( fd, VIDIOCSPICT, &p ) <0 ) {
        perror( "VIDIOCSPICT" );
        return false;
    }
    return true;
}

bool VideoForLinux1::getContrast(unsigned int &value)
{
    struct video_picture p;
    if ( ioctl( fd, VIDIOCGPICT, &p ) <0 ) {
        perror( "VIDIOCGPICT" );
        return false;
    }
    value=p.contrast;
    return true;
}

bool VideoForLinux1::setGamma(unsigned int value)
{
    struct video_picture p;
    if( !getPictureFunctions(&p) )
        return false;
    p.whiteness=value;
    if ( ioctl( fd, VIDIOCSPICT, &p ) <0 ) {
        perror( "VIDIOCSPICT" );
        return false;
    }
    return true;
}

bool VideoForLinux1::getGamma(unsigned int &value)
{
    struct video_picture p;
    if ( ioctl( fd, VIDIOCGPICT, &p ) <0 ) {
        perror( "VIDIOCGPICT" );
        return false;
    }
    value=p.whiteness;
    return true;
}

bool VideoForLinux1::setSaturation(unsigned int value)
{
    struct video_picture p;
    if( !getPictureFunctions(&p) )
        return false;
    p.colour=value;
    if ( ioctl( fd, VIDIOCSPICT, &p ) <0 ) {
        perror( "VIDIOCSPICT" );
        return false;
    }
    return true;
}

bool VideoForLinux1::getSaturation(unsigned int &value)
{
    struct video_picture p;
    if ( ioctl( fd, VIDIOCGPICT, &p ) <0 ) {
        perror( "VIDIOCGPICT" );
        return false;
    }
    value=p.colour;
    return true;
}    

bool VideoForLinux1::getPictureFunctions(struct video_picture *p) /*VIDIOCGPICT*/
{
    if ( ioctl( fd, VIDIOCGPICT, p ) < 0 ) {
        perror( "VIDIOCGPICT" );
        return false;
    }
    return true;
}

bool VideoForLinux1::getWindowOption(struct video_window *vw) /*VIDIOCGWIN*/
{
    if ( ioctl( fd, VIDIOCGWIN, vw ) < 0 ) {
        perror( "VIDIOCGWIN" );
        return false;
    }
    return true;
}

bool VideoForLinux1::getMmapSize( struct video_mbuf *vm ) /*VIDIOCGMBUF*/
{ 
    if ( ioctl( fd, VIDIOCGMBUF, vm ) ) {
        perror( "VIDIOCGMBUF" );
        return false;
    }
    // remember number of frames supported by driver
    frameCount=vm->frames;
    //cout<<"Frame buffer: "<<frameCount<<endl;
    // remember data offsets for each frame
    if(!frameOffsets)
        frameOffsets = (int*) malloc(sizeof(int)*frameCount);
    for (int i=0;i<frameCount;i++) {
        frameOffsets[i]=vm->offsets[i];
        v.frame = i;
        ioctl( fd, VIDIOCSYNC, &v.frame );
    }
 
    return true;
}

std::string VideoForLinux1::numpal2string(int num)
{
    switch(video_palette[num]) {
        case VIDEO_PALETTE_RGB32:
            return "VIDEO_PALETTE_RGB32";
            break;
        case VIDEO_PALETTE_RGB24:
            return "VIDEO_PALETTE_RGB24";
            break;
        case VIDEO_PALETTE_YUV420P:
            return "VIDEO_PALETTE_YUV420P";
            break;
        case VIDEO_PALETTE_YUV420:
            return "VIDEO_PALETTE_YUV420";
            break;
        case VIDEO_PALETTE_YUYV:
            return "VIDEO_PALETTE_YUYV";
            break; 
        case VIDEO_PALETTE_UYVY:
            return "VIDEO_PALETTE_UYVY";
            break; 
        default:
            return "UNKNOWN";
    }
}
    
 
