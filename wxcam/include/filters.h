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

void convert2bgr(unsigned char* frame, int size);

void negative(unsigned char* frame, int size);

void upturned(unsigned char* frame, int size);

void mirror(unsigned char* frame, int size, int width);

void monochrome(unsigned char* frame, int size);

void edge(unsigned char* frame, int size, int width);

void rgb32_rgb24(unsigned int width, unsigned int height, const unsigned char* src, unsigned char* dest);

void rgb32_bgr24(unsigned int width, unsigned int height, const unsigned char* src, unsigned char* dest);

void median(unsigned char* image, int size, int width);

void verticalmirror(unsigned char* frame, int size, int width);

extern "C" {void stretch_asm(unsigned char* frame, unsigned char* dst, int height, int width, int size);}
void stretch_wrp(unsigned char* src, int size, int color);

extern "C" {void negative_asm(unsigned char* src, int size);}
void negative_wrp(unsigned char* src, int size);

extern "C" {void blueonly_asm(unsigned char* src, int size, int color);}
void blueonly_wrp(unsigned char* src, int size, int color);

extern "C" {void verticalMirror_asm(unsigned char* src, int size, int width, int height);}
void verticalMirror_wrp(unsigned char* src, int size, int width);

extern "C" {unsigned long long int mono_asm(unsigned char* src, int size);}        // cambie el return value a int para devolver el tiempo
void mono_wrp(unsigned char* src, int size);

extern "C" {void blur_asm(unsigned char* src, unsigned char* dst, int h, int w, int src_row_size, int dst_row_size);}
void blur_wrp(unsigned char* src, int size, int width);

extern "C" {void edge_asm(unsigned char* src, unsigned char* dst, int height, int width);}
void edge_wrp(unsigned char* src, int size, int height, int width);

extern "C" {void bona_asm(unsigned char* src, int size, int threshold);}
void bona_wrp(unsigned char* src, int size);

extern "C" {void pixelate_asm(unsigned char* src, int grade, int height, int width);}
void pixelate_wrp(unsigned char* src, int size, int height, int width);

extern "C" {void median_asm(unsigned char* frame, unsigned char* dst, int height, int width, int size);}
void median_wrp(unsigned char* src, int size, int height, int width);

void instagram_wrp(unsigned char* src, int size, int width);
extern "C" {void blushing_asm(unsigned char* src, int size, int threshold);}
extern "C" {void brightness_asm(unsigned char* src, int size, int value);}
extern "C" {void contrast_asm(unsigned char* src, int size);}

static __inline__ unsigned long rdtsc(void);