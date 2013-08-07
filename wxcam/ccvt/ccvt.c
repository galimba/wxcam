/*  CCVT: ColourConVerT: simple library for converting colourspaces
 * Copyright (C) 2002 Nemosoft Unv.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For questions, remarks, patches, etc. for this program, the author can be
 * reached at nemosoft@smcc.demon.nl.
 */

#include "ccvt.h"
#include <mjpegtools/jpegutils.h>
#include <mjpegtools/mjpeg_types.h>
#include <stdlib.h>
#include <string.h>

#define PUSH_RGB24	1
#define PUSH_BGR24	2
#define PUSH_RGB32	3
#define PUSH_BGR32	4

#define LIMIT(x)  ( (x) > 0xffff ? 0xff : ( (x) <= 0xff ? 0 : ( (x) >> 8 ) ) )

static int *LutYr = NULL;
static int *LutYg = NULL;
static int *LutYb = NULL;
static int *LutVr = NULL;
static int *LutVrY = NULL;
static int *LutUb = NULL;
static int *LutUbY = NULL;
static int *LutRv = NULL;
static int *LutGu = NULL;
static int *LutGv = NULL;
static int *LutBu = NULL;

unsigned char
RGB24_TO_Y(unsigned char r, unsigned char g, unsigned char b)
{
return (LutYr[(r)] + LutYg[(g)] + LutYb[(b)]);
}
unsigned char
YR_TO_V(unsigned char r, unsigned char y)
{
return (LutVr[(r)] + LutVrY[(y)]);
}
unsigned char
YB_TO_U(unsigned char b, unsigned char y)
{
return (LutUb[(b)] + LutUbY[(y)]);
}
unsigned char
R_FROMYV(unsigned char y, unsigned char v)
{
return CLIP((y) + LutRv[(v)]);
}
unsigned char
G_FROMYUV(unsigned char y, unsigned char u, unsigned char v)
{
return CLIP((y) + LutGu[(u)] + LutGv[(v)]);
}
unsigned char
B_FROMYU(unsigned char y, unsigned char u)
{
return CLIP((y) + LutBu[(u)]);
}



/* This is a simplistic approach. */
static void ccvt_420p(int width, int height, const unsigned char *src, unsigned char *dst, int push) {
    int line, col, linewidth;
    int y, u, v, yy, vr, ug, vg, ub;
    int r, g, b;
    const unsigned char *py, *pu, *pv;
    
    linewidth = width >> 1;
    py = src;
    pu = py + (width * height);
    pv = pu + (width * height) / 4;
    
    y = *py++;
    yy = y << 8;
    u = *pu - 128;
    ug =   88 * u;
    ub =  454 * u;
    v = *pv - 128;
    vg =  183 * v;
    vr =  359 * v;
    
    for (line = 0; line < height; line++) {
        for (col = 0; col < width; col++) {
            r = (yy +      vr) >> 8;
            g = (yy - ug - vg) >> 8;
            b = (yy + ub     ) >> 8;
            
            if (r < 0)   r = 0;
            if (r > 255) r = 255;
            if (g < 0)   g = 0;
            if (g > 255) g = 255;
            if (b < 0)   b = 0;
            if (b > 255) b = 255;
            
            switch(push) {
                case PUSH_RGB24:
                    *dst++ = r;
                    *dst++ = g;
                    *dst++ = b;
                    break;
                    
                case PUSH_BGR24:
                    *dst++ = b;
                    *dst++ = g;
                    *dst++ = r;
                    break;
                    
                case PUSH_RGB32:
                    *dst++ = r;
                    *dst++ = g;
                    *dst++ = b;
                    *dst++ = 0;
                    break;
                    
                case PUSH_BGR32:
                    *dst++ = b;
                    *dst++ = g;
                    *dst++ = r;
                    *dst++ = 0;
                    break;
            }
            
            y = *py++;
            yy = y << 8;
            if (col & 1) {
                pu++;
                pv++;
                
                u = *pu - 128;
                ug =   88 * u;
                ub =  454 * u;
                v = *pv - 128;
                vg =  183 * v;
                vr =  359 * v;
            }
        } /* ..for col */
        if ((line & 1) == 0) { // even line: rewind
            pu -= linewidth;
            pv -= linewidth;
        }
    } /* ..for line */
}

/* This is a really simplistic approach. Speedups are welcomed. */
static void ccvt_420i(int width, int height, unsigned char *src, unsigned char *dst, int push) {
    int line, col, linewidth;
    int y, u, v, yy, vr, ug, vg, ub;
    int r, g, b;
    unsigned char *py, *pu, *pv;
    
    linewidth = width + (width >> 1);
    py = src;
    pu = py + 4;
    pv = pu + linewidth;
    
    y = *py++;
    yy = y << 8;
    u = *pu - 128;
    ug =   88 * u;
    ub =  454 * u;
    v = *pv - 128;
    vg =  183 * v;
    vr =  359 * v;
    
    /* The biggest problem is the interlaced data, and the fact that odd
     * add even lines have V and U data, resp.
     */
    for (line = 0; line < height; line++) {
        for (col = 0; col < width; col++) {
            r = (yy +      vr) >> 8;
            g = (yy - ug - vg) >> 8;
            b = (yy + ub     ) >> 8;
            
            switch(push) {
                case PUSH_RGB24:
                    *dst++ = r;
                    *dst++ = g;
                    *dst++ = b;
                    break;
                    
                case PUSH_BGR24:
                    *dst++ = b;
                    *dst++ = g;
                    *dst++ = r;
                    break;
                    
                case PUSH_RGB32:
                    *dst++ = r;
                    *dst++ = g;
                    *dst++ = b;
                    *dst++ = 0;
                    break;
                    
                case PUSH_BGR32:
                    *dst++ = b;
                    *dst++ = g;
                    *dst++ = r;
                    *dst++ = 0;
                    break;
            }
            
            y = *py++;
            yy = y << 8;
            if ((col & 3) == 3)
                py += 2; // skip u/v
            if (col & 1) {
                if ((col & 3) == 3) {
                    pu += 4; // skip y
                    pv += 4;
                }
                else {
                    pu++;
                    pv++;
                }
                u = *pu - 128;
                ug =   88 * u;
                ub =  454 * u;
                v = *pv - 128;
                vg =  183 * v;
                vr =  359 * v;
            }
        } /* ..for col */
        if (line & 1) { // odd line: go to next band
            pu += linewidth;
            pv += linewidth;
        }
        else { // rewind u/v pointers
            pu -= linewidth;
            pv -= linewidth;
        }
    } /* ..for line */
}

void ccvt_uyvy(int width, int height, unsigned char *src, unsigned char *dest, int push) 
{    
    int R, G, B;
    int Y1, Y2;
    int cG, cR, cB;
    int i;
    for(i=height*width/2;i>0;i--) {
        cB = ((*src - 128) * 454) >> 8;
        cG = (*src++ - 128) * 88;
        Y1 = *src++;
        cR = ((*src - 128) * 359) >> 8;
        cG = (cG + (*src++ - 128) * 183) >> 8;
        Y2 = *src++;
        
        R = Y1 + cR;
        G = Y1 + cG;
        B = Y1 + cB;
        
        SAT(R);
        SAT(G);
        SAT(B);
        
        if(push == PUSH_RGB24) {               
            *dest++ = B;
            *dest++ = G;
            *dest++ = R;
        }
        else {
            *dest++ = R;
            *dest++ = G;
            *dest++ = B; 
        }
            
        
        R = Y2 + cR;
        G = Y2 + cG;
        B = Y2 + cB;
        
        SAT(R);
        SAT(G);
        SAT(B);
        
        if(push == PUSH_RGB24) {    
            *dest++ = B;
            *dest++ = G;
            *dest++ = R;
        }
        else {
            *dest++ = R;
            *dest++ = G;
            *dest++ = B;
        }
    }
    
}


/* This is a really simplistic approach. Speedups are welcomed. */
/* Derived this from the above 4:2:0 planar for yuyv */
/* Format: YUYV YUYV YUYV YUYV... */
static void ccvt_yuyv(int width, int height, const unsigned char *src, unsigned char *dst, int push) {
    int line, col, linewidth;
    int y, yy;
    int u, v;
    int vr, ug, vg, ub;
    int r, g, b;
    const unsigned char *py, *pu, *pv;
    
    linewidth = width - (width >> 1);
    py = src;
    pu = src + 1;
    pv = src + 3;
    
    y = *py;
    yy = y << 8;
    u = *pu - 128;
    ug = 88 * u;
    ub = 454 * u;
    v = *pv - 128;
    vg = 183 * v;
    vr = 359 * v;
    
    for (line = 0; line < height; line++) {
        for (col = 0; col < width; col++) {
            r = LIMIT(yy + vr);
            g = LIMIT(yy - ug - vg);
            b = LIMIT(yy + ub );
            switch(push) {
                case PUSH_RGB24:
                    *dst++ = r;
                    *dst++ = g;
                    *dst++ = b;
                    break;
                    
                case PUSH_BGR24:
                    *dst++ = b;
                    *dst++ = g;
                    *dst++ = r;
                    break;
                case PUSH_RGB32:
                    *dst++ = r;
                    *dst++ = g;
                    *dst++ = b;
                    *dst++ = 0;
                    break;
                    
                case PUSH_BGR32:
                    *dst++ = b;
                    *dst++ = g;
                    *dst++ = r;
                    *dst++ = 0;
                    break;
            }
            py += 2;
            y = *py;
            yy = y << 8;
            if ( (col & 1) == 1) {
                pu += 4; // skip yvy every second y
                pv += 4; // skip yuy every second y
            }
            u = *pu - 128;
            ug = 88 * u;
            ub = 454 * u;
            v = *pv - 128;
            vg = 183 * v;
            vr = 359 * v;
        } // ..for col
    } /* ..for line */
}

/*void ccvt_yuyv_rgb24_422(int width, int height, const void *src, void *dst) 
{
    const unsigned char *s;
    unsigned char *d;
//PIXTYPE_rgb24 *d;
    int l, c;
    int r, g, b, cr, cg, cb, y1, y2;
    
    l = height;
    s = src;
    d = dst;
    while (l--) {
        c = width >> 2;
        while (c--) {
            cb = ((*s - 128) * 454) >> 8;
            cg = (*s++ - 128) * 88;
            y1 = *s++;
            cr = ((*s - 128) * 359) >> 8;
            cg = (cg + (*s++ - 128) * 183) >> 8;
            y2 = *s++;
            r = y1 + cr;
            b = y1 + cb;
            g = y1 - cg;
            SAT(r);
            SAT(g);
            SAT(b);
            *d++ = r;
            *d++ = g;
            *d++ = b;
            r = y2 + cr;
            b = y2 + cb;
            g = y2 - cg;
            SAT(r);
            SAT(g);
            SAT(b);
            *d++ = r;
            *d++ = g;
            *d++ = b;
        }
    }
}*/

/*void ccvt_yuyv_bgr24_422(int width, int height, const void *src, void *dst) 
{
    const unsigned char *s;
    PIXTYPE_bgr24 *d;
    int l, c;
    int r, g, b, cr, cg, cb, y1, y2;
    
    l = height;
    s = src;
    d = dst;
    while (l--) {
        c = width >> 2;
        while (c--) {
            cb = ((*s - 128) * 454) >> 8;
            cg = (*s++ - 128) * 88;
            y1 = *s++;
            cr = ((*s - 128) * 359) >> 8;
            cg = (cg + (*s++ - 128) * 183) >> 8;
            y2 = *s++;
            r = y1 + cr;
            b = y1 + cb;
            g = y1 - cg;
            SAT(r);
            SAT(g);
            SAT(b);
            d->b = b;
            d->g = g;
            d->r = r;
            d++;
            r = y2 + cr;
            b = y2 + cb;
            g = y2 - cg;
            SAT(r);
            SAT(g);
            SAT(b);
            d->b = b;
            d->g = g;
            d->r = r;
            d++;
        }
    }
}*/

#define CLAMP(x)        ((x)<0?0:((x)>255)?255:(x))

typedef struct {
	int is_abs;
	int len;
	int val;
} code_table_t;

static void sonix_decompress_init(code_table_t * table)
{
	int i;
	int is_abs, val, len;

	for (i = 0; i < 256; i++) {
		is_abs = 0;
		val = 0;
		len = 0;
		if ((i & 0x80) == 0) {
			/* code 0 */
			val = 0;
			len = 1;
		} else if ((i & 0xE0) == 0x80) {
			/* code 100 */
			val = +4;
			len = 3;
		} else if ((i & 0xE0) == 0xA0) {
			/* code 101 */
			val = -4;
			len = 3;
		} else if ((i & 0xF0) == 0xD0) {
			/* code 1101 */
			val = +11;
			len = 4;
		} else if ((i & 0xF0) == 0xF0) {
			/* code 1111 */
			val = -11;
			len = 4;
		} else if ((i & 0xF8) == 0xC8) {
			/* code 11001 */
			val = +20;
			len = 5;
		} else if ((i & 0xFC) == 0xC0) {
			/* code 110000 */
			val = -20;
			len = 6;
		} else if ((i & 0xFC) == 0xC4) {
			/* code 110001xx: unknown */
			val = 0;
			len = 8;
		} else if ((i & 0xF0) == 0xE0) {
			/* code 1110xxxx */
			is_abs = 1;
			val = (i & 0x0F) << 4;
			len = 8;
		}
		table[i].is_abs = is_abs;
		table[i].val = val;
		table[i].len = len;
	}
}

/*
 *   sonix_decompress
 *   ================
 *   decompresses an image encoded by a SN9C101 camera controller chip.
 *
 *   IN    width
 *         height
 *         inp     pointer to compressed frame (with header already stripped)
 *   OUT   outp    pointer to decompressed frame
 *
 *         Returns 0 if the operation was successful.
 *         Returns <0 if operation failed.
 *
 */

int sonix_decompress(unsigned char *outp, unsigned char *inp, int width, int height) {
    int row, col;
    int val;
    int bitpos;
    unsigned char code;
    unsigned char *addr;
    
    /* local storage */
    static code_table_t table[256];
    static int init_done = 0;
    
    if (!init_done) {
        init_done = 1;
        sonix_decompress_init(table);
        /* do sonix_decompress_init first! */
        //return -1; // so it has been done and now fall through
    }
    
    bitpos = 0;
    for (row = 0; row < height; row++) {
        
        col = 0;
        
        /* first two pixels in first two rows are stored as raw 8-bit */
        if (row < 2) {
            addr = inp + (bitpos >> 3);
            code = (addr[0] << (bitpos & 7)) | (addr[1] >> (8 - (bitpos & 7)));
            bitpos += 8;
            *outp++ = code;
            
            addr = inp + (bitpos >> 3);
            code = (addr[0] << (bitpos & 7)) | (addr[1] >> (8 - (bitpos & 7)));
            bitpos += 8;
            *outp++ = code;
            
            col += 2;
        }
        
        while (col < width) {
            /* get bitcode from bitstream */
            addr = inp + (bitpos >> 3);
            code = (addr[0] << (bitpos & 7)) | (addr[1] >> (8 - (bitpos & 7)));
            
            /* update bit position */
            bitpos += table[code].len;
            
            /* calculate pixel value */
            val = table[code].val;
            if (!table[code].is_abs) {
                /* value is relative to top and left pixel */
                if (col < 2) {
                    /* left column: relative to top pixel */
                    val += outp[-2 * width];
                } else if (row < 2) {
                    /* top row: relative to left pixel */
                    val += outp[-2];
                } else {
                    /* main area: average of left pixel and top pixel */
                    val += (outp[-2] + outp[-2 * width]) / 2;
                }
            }
            
            /* store pixel */
            *outp++ = CLAMP(val);
            col++;
        }
    }
    
    return 0;
}

/*
 * BAYER2RGB24 ROUTINE TAKEN FROM:
 *
 * Sonix SN9C10x based webcam basic I/F routines
 * Takafumi Mizuno <taka-qce@ls-a.jp>
 *
 */

void bayer2rgb24(unsigned char *dst, unsigned char *src, long int width, long int height) {
    long int i;
    unsigned char *rawpt, *scanpt;
    long int size;
    
    rawpt = src;
    scanpt = dst;
    size = width * height;
    
    for (i = 0; i < size; i++) {
        if (((i / width) & 1) == 0) {	// %2 changed to & 1
            if ((i & 1) == 0) {
                /* B */
                if ((i > width) && ((i % width) > 0)) {
                    *scanpt++ = *rawpt;     /* B */
                    *scanpt++ = (*(rawpt - 1) + *(rawpt + 1) + *(rawpt + width) + *(rawpt - width)) / 4;	/* G */
                    *scanpt++ = (*(rawpt - width - 1) + *(rawpt - width + 1) + *(rawpt + width - 1) + *(rawpt + width + 1)) / 4;    /* R */
                } else {
                    /* first line or left column */
                    *scanpt++ = *rawpt;     /* B */
                    *scanpt++ = (*(rawpt + 1) + *(rawpt + width)) / 2;	/* G */
                    *scanpt++ = *(rawpt + width + 1);       /* R */
                }
            } else {
                /* (B)G */
                if ((i > width) && ((i % width) < (width - 1))) {
                    *scanpt++ = (*(rawpt - 1) + *(rawpt + 1)) / 2;  /* B */
                    *scanpt++ = *rawpt;	/* G */
                    *scanpt++ = (*(rawpt + width) + *(rawpt - width)) / 2;  /* R */
                } else {
                    /* first line or right column */
                    *scanpt++ = *(rawpt - 1);       /* B */
                    *scanpt++ = *rawpt;	/* G */
                    *scanpt++ = *(rawpt + width);   /* R */
                }
            }
        } else {
            if ((i & 1) == 0) {
                /* G(R) */
                if ((i < (width * (height - 1))) && ((i % width) > 0)) {
                    *scanpt++ = (*(rawpt + width) + *(rawpt - width)) / 2;  /* B */
                    *scanpt++ = *rawpt;	/* G */
                    *scanpt++ = (*(rawpt - 1) + *(rawpt + 1)) / 2;  /* R */
                } else {
                    /* bottom line or left column */
                    *scanpt++ = *(rawpt - width);   /* B */
                    *scanpt++ = *rawpt;	/* G */
                    *scanpt++ = *(rawpt + 1);       /* R */
                }
            } else {
                /* R */
                if (i < (width * (height - 1)) && ((i % width) < (width - 1))) {
                    *scanpt++ = (*(rawpt - width - 1) + *(rawpt - width + 1) + *(rawpt + width - 1) + *(rawpt + width + 1)) / 4;    /* B */
                    *scanpt++ = (*(rawpt - 1) + *(rawpt + 1) + *(rawpt - width) + *(rawpt + width)) / 4;	/* G */
                    *scanpt++ = *rawpt;     /* R */
                } else {
                    /* bottom line or right column */
                    *scanpt++ = *(rawpt - width - 1);       /* B */
                    *scanpt++ = (*(rawpt - 1) + *(rawpt - width)) / 2;	/* G */
                    *scanpt++ = *rawpt;     /* R */
                }
            }
        }
        rawpt++;
    }
    
}

void mjpegtoyuv420p(const unsigned char *src, unsigned char *dst,
        int width, int height, unsigned int size) {
    
    uint8_t *yuv[3];
    unsigned char *y, *u, *v;
    int loop;
    static int interlaced = 0;

    yuv[0] = malloc(width * height * sizeof(yuv[0][0]));
    yuv[1] = malloc(width * height / 4 * sizeof(yuv[1][0]));
    yuv[2] = malloc(width * height / 4 * sizeof(yuv[2][0]));
    
    int ret = decode_jpeg_raw((unsigned char*)src, size, interlaced, 420, width, height, yuv[0], yuv[1], yuv[2]);
    if(ret) {
        interlaced = 1;
        decode_jpeg_raw((unsigned char*)src, size, interlaced, 420, width, height, yuv[0], yuv[1], yuv[2]);
    }
    
    y=dst;
    u=y+width*height;
    v=u+(width*height)/4;
    memset(y, 0, width*height);
    memset(u, 0, width*height/4);
    memset(v, 0, width*height/4);
    
    for(loop=0; loop<width*height; loop++) {
        *dst++=yuv[0][loop];
    }
    
    for(loop=0; loop<width*height/4; loop++) {
        *dst++=yuv[1][loop];
    }
    
    for(loop=0; loop<width*height/4; loop++) {
        *dst++=yuv[2][loop];
    }
    
    free(yuv[0]);
    free(yuv[1]);
    free(yuv[2]);
}

void ccvt_yuyv_rgb24_422(int width, int height, const void *src, void *dst) {
    ccvt_uyvy(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_RGB24);
}

void ccvt_yuyv_bgr24_422(int width, int height, const void *src, void *dst) {
    ccvt_uyvy(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_BGR24);
}

void ccvt_uyvy_rgb24(int width, int height, const void *src, void *dst) {
    ccvt_uyvy(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_RGB24);
}

void ccvt_uyvy_bgr24(int width, int height, const void *src, void *dst) {
    ccvt_uyvy(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_BGR24);
}

void ccvt_yuyv_rgb24(int width, int height, const void *src, void *dst) {
    ccvt_yuyv(width, height, (const unsigned char *)src, (unsigned char *)dst, PUSH_RGB24);
}

void ccvt_yuyv_bgr24(int width, int height, const void *src, void *dst) {
    ccvt_yuyv(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_BGR24);
}

void ccvt_420p_rgb24(int width, int height, const void *src, void *dst) {
    ccvt_420p(width, height, (const unsigned char *)src, (unsigned char *)dst, PUSH_RGB24);
}

void ccvt_420p_bgr24(int width, int height, const void *src, void *dst) {
    ccvt_420p(width, height, (const unsigned char *)src, (unsigned char *)dst, PUSH_BGR24);
}

void ccvt_420p_rgb32(int width, int height, const void *src, void *dst) {
    ccvt_420p(width, height, (const unsigned char *)src, (unsigned char *)dst, PUSH_RGB32);
}

void ccvt_420p_bgr32(int width, int height, const void *src, void *dst) {
    ccvt_420p(width, height, (const unsigned char *)src, (unsigned char *)dst, PUSH_BGR32);
}

void ccvt_420i_rgb24(int width, int height, const void *src, void *dst) {
    ccvt_420i(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_RGB24);
}

void ccvt_420i_bgr24(int width, int height, const void *src, void *dst) {
    ccvt_420i(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_BGR24);
}

void ccvt_420i_rgb32(int width, int height, const void *src, void *dst) {
    ccvt_420i(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_RGB32);
}

void ccvt_420i_bgr32(int width, int height, const void *src, void *dst) {
    ccvt_420i(width, height, (unsigned char *)src, (unsigned char *)dst, PUSH_BGR32);
}
