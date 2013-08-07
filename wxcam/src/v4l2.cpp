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

#include "setting.h"
#include "v4l2.h"
#include "ccvt.h"
#include "filters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
#include <assert.h>
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h> 
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <wx/mstream.h>
#include <wx/image.h>
#include <iostream>

#include "pwc-ioctl.h"

VideoForLinux2::VideoForLinux2()
{
    dev_name = Setting::GetInstance()->GetDeviceFile();
    buffers = NULL;
    n_buffers = 0;
    controlsSetted = false;
    pixelFormat = 0;
    isMapped = false;       
}
   
VideoForLinux2::~VideoForLinux2()
{
    closeDevice();
}

void VideoForLinux2::closeDevice()
{
    unsigned int i;
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl (fd, VIDIOC_STREAMOFF, &type) == -1)
        perror("VIDIOC_STREAMOFF");
    
     for (i = 0; i < n_buffers; ++i)
         if (munmap (buffers[i].start, buffers[i].length) == -1)
             perror("munmap");
    
    free(buffers);
}

bool VideoForLinux2::mMap(int w, int h, int frameRate) {
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;
            
    if(!controlsSetted)
        setControls();
    
    memset(&fmt, 0, sizeof(struct v4l2_format));
    memset(&cropcap, 0, sizeof(struct v4l2_cropcap));
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    if (!ioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */
        
        if (ioctl(fd, VIDIOC_S_CROP, &crop) == -1) {
            switch (errno) {
                case EINVAL:
                    /* Cropping not supported. */
                    break;
                default:
                    /* Errors ignored. */
                    break;
            }
        }
    }
    else {
                /* Errors ignored. */
    } 

    
    if(frameFormat==VIDEO_PALETTE_RGB24) {
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24; 
        pixelFormat = V4L2_PIX_FMT_BGR24;
        std::cout<<"Using V4L2_PIX_FMT_BGR24 pixel format"<<std::endl;
    }
    else if(frameFormat==VIDEO_PALETTE_YUYV) {
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        pixelFormat = V4L2_PIX_FMT_YUYV;
        std::cout<<"Using V4L2_PIX_FMT_YUYV pixel format"<<std::endl;
    }
    else if(frameFormat==VIDEO_PALETTE_YUV420P) {
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
        pixelFormat = V4L2_PIX_FMT_YUV420;
        std::cout<<"Using V4L2_PIX_FMT_YUV420 pixel format"<<std::endl;
    }
    else if(frameFormat==VIDEO_PALETTE_JPEG) {
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
        pixelFormat = V4L2_PIX_FMT_JPEG;
        std::cout<<"Using V4L2_PIX_FMT_JPEG pixel format"<<std::endl;
    }
    else if(frameFormat==VIDEO_PALETTE_MJPEG) {
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        pixelFormat = V4L2_PIX_FMT_MJPEG;
        std::cout<<"Using V4L2_PIX_FMT_MJPEG pixel format"<<std::endl;
    }
    else { //AUTO or YUV420
        struct v4l2_fmtdesc argp;
        memset(&argp, 0, sizeof(struct v4l2_fmtdesc));
        argp.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        std::cout<<"Determining pixel format..."<<std::endl;
        for(int index=0; index < 16; index++) {            
            argp.index=index;
            if (ioctl(fd, VIDIOC_ENUM_FMT, &argp) == -1) {
                //perror("VIDIOC_ENUM_FMT");
                break;
            }
            else {
                std::string description = (char*)argp.description;
                std::cout<<"pixel format: "<<description<<std::endl;
                if(argp.pixelformat == V4L2_PIX_FMT_YUV420) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
                    pixelFormat = V4L2_PIX_FMT_YUV420;
                    std::cout<<"Found V4L2_PIX_FMT_YUV420 pixel format"<<std::endl;
                    break;
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_YUYV) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
                    pixelFormat = V4L2_PIX_FMT_YUYV;
                    std::cout<<"Found V4L2_PIX_FMT_YUYV pixel format"<<std::endl;
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_JPEG ) {
                    wxInitAllImageHandlers();
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
                    pixelFormat = V4L2_PIX_FMT_JPEG;
                    std::cout<<"Found V4L2_PIX_FMT_JPEG pixel format"<<std::endl;                     
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_MJPEG) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
                    pixelFormat = V4L2_PIX_FMT_MJPEG;
                    std::cout<<"Found V4L2_PIX_FMT_MJPEG pixel format"<<std::endl;                     
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_SN9C10X) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SN9C10X;
                    pixelFormat = V4L2_PIX_FMT_SN9C10X;
                    std::cout<<"Found V4L2_PIX_FMT_SN9C10X pixel format"<<std::endl;                    
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_SBGGR8) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SBGGR8;
                    pixelFormat = V4L2_PIX_FMT_SBGGR8;
                    std::cout<<"Found V4L2_PIX_FMT_SBGGR8 pixel format"<<std::endl;                    
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_SBGGR16) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SBGGR16;
                    pixelFormat = V4L2_PIX_FMT_SBGGR16;
                    std::cout<<"Found V4L2_PIX_FMT_SBGGR16 pixel format"<<std::endl;                    
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_SGBRG8) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SGBRG8;
                    pixelFormat = V4L2_PIX_FMT_SGBRG8;
                    std::cout<<"Found V4L2_PIX_FMT_SGBRG8 pixel format"<<std::endl;                    
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_SGRBG8) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG8;
                    pixelFormat = V4L2_PIX_FMT_SGRBG8;
                    std::cout<<"Found V4L2_PIX_FMT_SGRBG8 pixel format"<<std::endl;                    
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_SPCA561) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SPCA561;
                    pixelFormat = V4L2_PIX_FMT_SPCA561;
                    std::cout<<"Found V4L2_PIX_FMT_SPCA561 pixel format"<<std::endl;                    
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_BGR24) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
                    pixelFormat = V4L2_PIX_FMT_BGR24;
                    std::cout<<"Found V4L2_PIX_FMT_BGR24 pixel format"<<std::endl;
                    break;
                }
                else if(argp.pixelformat == V4L2_PIX_FMT_UYVY) {
                    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
                    pixelFormat = V4L2_PIX_FMT_UYVY;
                    std::cout<<"Found V4L2_PIX_FMT_UYVY pixel format"<<std::endl;
                    break;
                }                         
            }                
        }
    }
    
    if(!pixelFormat) {
        std::cout<<"Could not determine pixel format or pixel format not yet supported"<<std::endl;
        return false;
    }    
        
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = w;
    fmt.fmt.pix.height      = h;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

    //setting frame rate
    if(frameRate) {

        /*
        //many cams doesn't support these calls
        v4l2_streamparm parm;

        if (ioctl (fd, VIDIOC_G_PARM, &parm) == -1) {
            perror("VIDIOC_G_PARM");
        }

        parm.parm.capture.timeperframe.numerator = 1;
        parm.parm.capture.timeperframe.denominator = frameRate;

        if (ioctl (fd, VIDIOC_S_PARM, &parm) == -1) {
            perror("VIDIOC_S_PARM");            
        }
         */

        fmt.fmt.pix.priv &= ~PWC_FPS_FRMASK;
        fmt.fmt.pix.priv |= ( frameRate << PWC_FPS_SHIFT );
    }
    
    /*enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl (fd, VIDIOC_STREAMOFF, &type) == -1)
        perror("VIDIOC_STREAMOFF"); 
     */

    if (isMapped) {
        closeDevice();
        close(fd);

        std::string device = Setting::GetInstance()->GetDeviceFile();
        fd = open(device.c_str(), O_RDWR); // open video device
    }

    if (ioctl (fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("VIDIOC_S_FMT");
        return false;
    }
    
    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    width = w;
    height = h;
    
    return init_mmap();
    
}

bool VideoForLinux2::init_mmap()
{
    struct v4l2_requestbuffers req;
    enum v4l2_buf_type type;

    memset(&req, 0, sizeof(struct v4l2_requestbuffers));
    
    req.count               = 4;
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory              = V4L2_MEMORY_MMAP;
    
    if (ioctl (fd, VIDIOC_REQBUFS, &req) == -1) {
        if (EINVAL == errno) {
            fprintf (stderr, "%s does not support "
                    "memory mapping\n", dev_name.c_str());
            return false;
        } 
        else {
            perror("VIDIOC_REQBUFS");
            return false;
        }
    }
    
    if (req.count < 2) {
        fprintf(stderr, "Insufficient buffer memory on %s\n",
                dev_name.c_str());
        return false;
    }
    
    /*if(buffers) {
        for (unsigned int i = 0; i < n_buffers; ++i)
            if (munmap (buffers[i].start, buffers[i].length) == -1)
                perror("munmap");
        free(buffers);
    }*/
    
    buffers = (buffer*)calloc (req.count, sizeof (*buffers));
    if (!buffers) { 
        fprintf (stderr, "Out of memory\n");
        return false;
    }
    
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(struct v4l2_buffer));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        if (ioctl (fd, VIDIOC_QUERYBUF, &buf) == -1) {
            perror("VIDIOC_QUERYBUF");
            return false;
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
                mmap (NULL /* start anywhere */,
                      buf.length,
                      PROT_READ | PROT_WRITE /* required */,
                      MAP_SHARED /* recommended */,
                      fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start) {
            perror("mmap");
            return false;
        }
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;
        
        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
            perror("VIDIOC_QBUF");
            return false;
        }
    }
        
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        perror("VIDIOC_STREAMON");
        return false;
    }    
    
    isMapped = true;
    return true;
}

bool VideoForLinux2::getFrame(char **buffer)
{
    unsigned int i;
    
    struct v4l2_buffer buf;
    
    if(!isMapped) {
        std::cout<<"Device not mapped"<<std::endl;
        return false;
    }
    
    memset(&buf, 0, sizeof(struct v4l2_buffer));
    
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    //buf.index  = n_buffers;

    if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
        switch (errno) {
            case EAGAIN:
                return false;
                
            case EIO:
                /* Could ignore EIO, see spec. */
                
                /* fall through */
                
            default:
                perror("VIDIOC_DQBUF");
                return false;
        }
    }

    assert(buf.index < n_buffers);       

    *buffer = ( char* ) malloc( width*height*3 );

    if(pixelFormat == V4L2_PIX_FMT_YUV420) {
        if(adjustColors)
            ccvt_420p_bgr24( width, height, ( const unsigned char* ) buffers[buf.index].start, ( unsigned char* ) *buffer );
        else
            ccvt_420p_rgb24( width, height, ( const unsigned char* ) buffers[buf.index].start, ( unsigned char* ) *buffer );             
    }
    else if(pixelFormat == V4L2_PIX_FMT_UYVY) {
        if(adjustColors)
            ccvt_uyvy_bgr24( width, height, ( const unsigned char* ) buffers[buf.index].start, ( unsigned char* ) *buffer );
        else
            ccvt_uyvy_rgb24( width, height, ( const unsigned char* ) buffers[buf.index].start, ( unsigned char* ) *buffer );             
    }
    else if(pixelFormat == V4L2_PIX_FMT_YUYV) {
        if(adjustColors)
            ccvt_yuyv_bgr24( width, height, ( const unsigned char* ) buffers[buf.index].start, ( unsigned char* ) *buffer );
        else
            ccvt_yuyv_rgb24( width, height, ( const unsigned char* ) buffers[buf.index].start, ( unsigned char* ) *buffer );
    }
    else if(pixelFormat == V4L2_PIX_FMT_MJPEG || pixelFormat == V4L2_PIX_FMT_JPEG) {
        //std::cout<<"Using mjpeg..."<<std::endl;
        if (buf.bytesused <= HEADERFRAME1) {
            // Prevent crash on empty image
            std::cout << "Ignoring empty buffer ..." <<std::endl;
        }
        else {
            unsigned char *tmp_buffer = (unsigned char *)malloc(width*height*3/2);
            mjpegtoyuv420p(( const unsigned char* ) buffers[buf.index].start, tmp_buffer, width, height, buf.length);
            if(adjustColors)
                ccvt_420p_bgr24( width, height, tmp_buffer, ( unsigned char* ) *buffer );
            else
                ccvt_420p_rgb24( width, height, tmp_buffer, ( unsigned char* ) *buffer );
            free (tmp_buffer);
        }
    }
    /*else if(pixelFormat == V4L2_PIX_FMT_JPEG || pixelFormat == V4L2_PIX_FMT_MJPEG) {

        if (buf.bytesused <= HEADERFRAME1) {
            // Prevent crash on empty image
            std::cout << "Ignoring empty buffer ..." <<std::endl;
        } else {
            wxMemoryInputStream in(buffers[buf.index].start, buf.bytesused);
            wxImage frame;
            if (!frame.LoadFile(in, wxBITMAP_TYPE_JPEG)) {
                std::cout << "Error reading jpeg frame" << std::endl;
                delete *buffer;
                *buffer = NULL;
                return false;
            }
            memcpy(*buffer, frame.GetData(), width * height * 3);
            if (adjustColors)
                convert2bgr((unsigned char*) *buffer, width * height * 3);
        }
    }*/
    else if(pixelFormat == V4L2_PIX_FMT_SN9C10X) {  
        unsigned char *tmp_buffer = (unsigned char *)malloc(width*height*4);
        sonix_decompress(tmp_buffer, ( unsigned char* ) buffers[buf.index].start, width, height);
        bayer2rgb24(( unsigned char* ) *buffer, tmp_buffer, width, height); 
        free(tmp_buffer);
    }
    else if(pixelFormat == V4L2_PIX_FMT_SBGGR8 || 
		pixelFormat == V4L2_PIX_FMT_SBGGR16 || 
		pixelFormat == V4L2_PIX_FMT_SGBRG8 ||
		pixelFormat == V4L2_PIX_FMT_SPCA561 || 
		pixelFormat ==  V4L2_PIX_FMT_SGRBG8) {         
        bayer2rgb24(( unsigned char* ) *buffer, ( unsigned char* ) buffers[buf.index].start, width, height); 
    }
    else if(pixelFormat == V4L2_PIX_FMT_BGR24 || pixelFormat == V4L2_PIX_FMT_SN9C10X
            || pixelFormat == V4L2_PIX_FMT_SBGGR8) {
        if(!adjustColors)
            convert2bgr( (unsigned char*)*buffer, width*height*3);   
    }
    else {
        std::cout<<"Unknown pixel format"<<std::endl;
        return false;
    }   
    
    if (ioctl (fd, VIDIOC_QBUF, &buf) == -1) {
        perror("VIDIOC_QBUF");
        return false;
    }
    
    return true;
}

bool VideoForLinux2::setVideoStd(video_std std) 
{
    struct v4l2_input input;
    v4l2_std_id std_id;

    memset(&input, 0, sizeof (input));

    if (-1 == ioctl(fd, VIDIOC_G_INPUT, &input.index)) {
        perror("VIDIOC_G_INPUT");
        return false;
    }

    if (-1 == ioctl(fd, VIDIOC_ENUMINPUT, &input)) {
        perror("VIDIOC_ENUM_INPUT");
        return false;
    }
    
    int mode;
    if(std == MODE_PAL) {
        std_id = V4L2_STD_PAL;
        if (0 == (input.std & V4L2_STD_PAL)) {
            perror("Oops. PAL is not supported");
            return false;
        }
    }
    else if(std == MODE_NTSC) {
        std_id = V4L2_STD_NTSC;
        if (0 == (input.std & V4L2_STD_NTSC)) {
            perror("Oops. NTSC is not supported");
            return false;
        }
    }
    else {
        std_id = V4L2_STD_SECAM;
        if (0 == (input.std & V4L2_STD_SECAM)) {
            perror("Oops. SECAM is not supported");
            return false;
        }        
    }

    if (-1 == ioctl(fd, VIDIOC_S_STD, &std_id)) {
        perror("VIDIOC_S_STD");
        return false;
    }
    return true;
}

bool VideoForLinux2::setResolution(unsigned int width, unsigned int height, unsigned int frameRate)
{   
    //frameRate=0; /*avoiding warning*/
    return mMap(width, height, frameRate);
}

bool VideoForLinux2::getResolution(unsigned int &width, unsigned int &height, unsigned int &frameRate)
{
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    frameRate=0; /*ignored*/
    if (ioctl (fd, VIDIOC_G_FMT, &fmt) == -1) {
        perror("VIDIOC_G_FMT");
        return false;
    }
    width = fmt.fmt.pix.width;
    height = fmt.fmt.pix.height;
    frameRate = ( fmt.fmt.pix.priv & PWC_FPS_FRMASK ) >> PWC_FPS_SHIFT;
    printf("width: %d, height %d\n", width, height);
    return true;
}

bool VideoForLinux2::setBrightness(unsigned int value)
{
    struct v4l2_control control;
    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_BRIGHTNESS;
    control.value = (int)(((float)(controls.maxbrightness - controls.minbrightness)/(float)65535) * (float)value + controls.minbrightness);
   
    if (ioctl (fd, VIDIOC_S_CTRL, &control) == -1) {
        perror ("VIDIOC_S_CTRL");
        return false;
    }
    
    return true;    
}

bool VideoForLinux2::getBrightness(unsigned int &value)
{
    struct v4l2_control control;
    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_BRIGHTNESS;

    if (ioctl (fd, VIDIOC_G_CTRL, &control) == 0) {
        value = (control.value - controls.minbrightness) * (int)((float)65535/(float)(controls.maxbrightness - controls.minbrightness));
        /* Ignore if V4L2_CID_BRIGHTNESS is unsupported */
    } 
    else if (errno != EINVAL) {
        perror ("VIDIOC_G_CTRL");
        return false;
    }        
    return true;
}

bool VideoForLinux2::setContrast(unsigned int value)
{
    struct v4l2_control control;
    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_CONTRAST;
    control.value = (int)(((float)(controls.maxcontrast - controls.mincontrast)/(float)65535) * (float)value + controls.mincontrast);
    
    if (ioctl (fd, VIDIOC_S_CTRL, &control) == -1) {
        perror ("VIDIOC_S_CTRL");
        return false;
    }
    
    return true;      
}

bool VideoForLinux2::getContrast(unsigned int &value)
{
    struct v4l2_control control;
    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_CONTRAST;

    if (ioctl (fd, VIDIOC_G_CTRL, &control) == 0) {
        value = (control.value - controls.mincontrast) * (int)((float)65535/(float)(controls.maxcontrast - controls.mincontrast));
        /* Ignore if V4L2_CID_CONTRAST is unsupported */
    } 
    else if (errno != EINVAL) {
        perror ("VIDIOC_G_CTRL");
        return false;
    }        
    return true;
}

bool VideoForLinux2::setGamma(unsigned int value)
{
    struct v4l2_control control;
    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_GAMMA;
    control.value = (int)(((float)(controls.maxgamma - controls.mingamma)/(float)65535) * (float)value + controls.mingamma);
    
    if (ioctl (fd, VIDIOC_S_CTRL, &control) == -1) {
        perror ("VIDIOC_S_CTRL");
        return false;
    }
    
    return true;    
}

bool VideoForLinux2::getGamma(unsigned int &value)
{
    struct v4l2_control control;
    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_GAMMA;

    if (ioctl (fd, VIDIOC_G_CTRL, &control) == 0) {
        value = (control.value - controls.mingamma) * (int)((float)65535/(float)(controls.maxgamma - controls.mingamma));
        /* Ignore if V4L2_CID_GAMMA is unsupported */
    } 
    else if (errno != EINVAL) {
        perror ("VIDIOC_G_CTRL");
        return false;
    }        
    return true;
}

bool VideoForLinux2::setSaturation(unsigned int value)
{
    struct v4l2_control control;
    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_SATURATION;
    control.value = (int)(((float)(controls.maxsaturation - controls.minsaturation)/(float)65535) * (float)value + controls.minsaturation);
    
    if (ioctl (fd, VIDIOC_S_CTRL, &control) == -1) {
        perror ("VIDIOC_S_CTRL");
        return false;
    }
    
    return true;    
}

bool VideoForLinux2::getSaturation(unsigned int &value)
{
    struct v4l2_control control;
    memset (&control, 0, sizeof (control));
    control.id = V4L2_CID_SATURATION;

    if (ioctl (fd, VIDIOC_G_CTRL, &control) == 0) {
        value = (control.value - controls.minsaturation) * (int)((float)65535/(float)(controls.maxsaturation - controls.minsaturation));
        /* Ignore if V4L2_CID_SATURATION is unsupported */
    } 
    else if (errno != EINVAL) {
        perror ("VIDIOC_G_CTRL");
        return false;
    }        
    return true;
}

void VideoForLinux2::setControls()
{
    struct v4l2_queryctrl queryctrl;

    controlsSetted = true;
    //enumerateControls();
    
    /*BRIGHTNESS*/
    memset (&queryctrl, 0, sizeof (queryctrl));
    queryctrl.id = V4L2_CID_BRIGHTNESS;

    if (ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl) ==-1) {
        if (errno != EINVAL) {
                perror ("VIDIOC_QUERYCTRL");
                //return false;
        } 
        else {
            printf ("V4L2_CID_BRIGHTNESS is not supported\n");
        }
    } 
    else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        printf ("V4L2_CID_BRIGHTNESS is not supported\n");
    }    
    
    controls.minbrightness = queryctrl.minimum;
    controls.maxbrightness = queryctrl.maximum;
    
    /*CONTRAST*/
    memset (&queryctrl, 0, sizeof (queryctrl));
    queryctrl.id = V4L2_CID_CONTRAST;

    if (ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl) ==-1) {
        if (errno != EINVAL) {
                perror ("VIDIOC_QUERYCTRL");
                //return false;
        } 
        else {
            printf ("V4L2_CID_CONTRAST is not supported\n");
        }
    } 
    else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        printf ("V4L2_CID_CONTRAST is not supported\n");
    } 
    
    controls.mincontrast = queryctrl.minimum;
    controls.maxcontrast = queryctrl.maximum;
    
    /*GAMMA*/
    memset (&queryctrl, 0, sizeof (queryctrl));
    queryctrl.id = V4L2_CID_GAMMA;

    if (ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl) ==-1) {
        if (errno != EINVAL) {
                perror ("VIDIOC_QUERYCTRL");
                //return false;
        } 
        else {
            printf ("V4L2_CID_GAMMA is not supported\n");
        }
    } 
    else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        printf ("V4L2_CID_GAMMA is not supported\n");
    } 
    
    controls.mingamma = queryctrl.minimum;
    controls.maxgamma = queryctrl.maximum;
    
    /*SATURATION*/
    memset (&queryctrl, 0, sizeof (queryctrl));
    queryctrl.id = V4L2_CID_SATURATION;

    if (ioctl (fd, VIDIOC_QUERYCTRL, &queryctrl) ==-1) {
        if (errno != EINVAL) {
                perror ("VIDIOC_QUERYCTRL");
                //return false;
        } 
        else {
            printf ("V4L2_CID_SATURATION is not supported\n");
        }
    } 
    else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
        printf ("V4L2_CID_SATURATION is not supported\n");
    } 
    
    controls.minsaturation = queryctrl.minimum;
    controls.maxsaturation = queryctrl.maximum;
    
}

void VideoForLinux2::enumerateMenu(struct v4l2_queryctrl &queryctrl, struct v4l2_querymenu querymenu)
{
    printf ("  Menu items:\n");

    memset (&querymenu, 0, sizeof (querymenu));
    querymenu.id = queryctrl.id;

    for (querymenu.index = queryctrl.minimum;
         querymenu.index <= queryctrl.maximum;
          querymenu.index++) {
            if (0 == ioctl (fd, VIDIOC_QUERYMENU, &querymenu)) {
                    printf ("  %s\n", querymenu.name);
            } else {
                    perror ("VIDIOC_QUERYMENU");
                    exit (EXIT_FAILURE);
            }
    }
}

void VideoForLinux2::enumerateControls()
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_querymenu querymenu;
    
    memset (&queryctrl, 0, sizeof (queryctrl));
    
    for (queryctrl.id = V4L2_CID_BASE;
    queryctrl.id < V4L2_CID_LASTP1;
    queryctrl.id++) {
        if (ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl) == 0) {
            if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
                printf("Control %s disabled\n", queryctrl.name);
                continue;
            }
            
            printf("Control %s\n", queryctrl.name);
            
            if (queryctrl.type == V4L2_CTRL_TYPE_MENU)
                enumerateMenu(queryctrl, querymenu);
        } 
        else {
            if (errno == EINVAL) {
                //perror("VIDIOC_QUERYCTRL");
                continue;
            }
            
            perror("VIDIOC_QUERYCTRL");
            exit(EXIT_FAILURE);
        }
    }
    
    for (queryctrl.id = V4L2_CID_PRIVATE_BASE;;
    queryctrl.id++) {
        if (0 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
            if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
                continue;
            
            printf("Control %s\n", queryctrl.name);
            
            if (queryctrl.type == V4L2_CTRL_TYPE_MENU)
                enumerateMenu(queryctrl, querymenu);
        } else {
            if (errno == EINVAL)
                break;
            
            perror("VIDIOC_QUERYCTRL");
            exit(EXIT_FAILURE);
        }
    }
}

