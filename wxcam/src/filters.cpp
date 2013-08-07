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

#include "filters.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

void convert2bgr(unsigned char* frame, int size)
{
    for(int i=0; i < size; i = i + 3)
    {
        unsigned char tmp;
        tmp = frame[i];
        frame[i] = frame[i + 2];
        frame[i + 2] = tmp;
    }
}

void negative(unsigned char* frame, int size)
{
    unsigned long t0 = rdtsc();
    for(int i=0; i < size; i++)
        frame[i] = ~frame[i];
    
    unsigned long t1 = rdtsc();
    //printf("ciclos negative c++: %ld \n", t1-t0);
}

void upturned(unsigned char* frame, int size)
{    
    int j = size - 1;
    for(int i=0; i < size/2; i = i + 3)
    {
        unsigned char tmp;
        tmp = frame[i];
        frame[i] = frame[j - 2];
        frame[j - 2] = tmp;
        
        tmp = frame[i + 1];
        frame[i + 1] = frame[j - 1];
        frame[j - 1] = tmp;
        
        tmp = frame[i + 2];
        frame[i + 2] = frame[j];
        frame[j] = tmp;
        j = j - 3;
    }
}

void mirror(unsigned char* frame, int size, int width)
{
    int height = size/(width*3);
    unsigned char pixel[3];
    for (int w=0; w < width/2; w++) {
        for(int h=w*3; h < height*width* 3; h = h + width*3) {
            memcpy(pixel, frame + h, 3);
            int last = h + (width - 1 - 2*w)*3;
            memcpy(frame + h, frame + last, 3);
            memcpy(frame + last, pixel, 3);
        }        
    }      
}

void monochrome(unsigned char* frame, int size) {
    
    //volatile int dont_remove __attribute__((unused)); // volatile to stop optimizing - AL PEDO
    unsigned long t0 = rdtsc();
    for(int i=0; i < size; i = i + 3) {
        int mean = (frame[i] + frame[i+1] + frame[i+2])/3;
        frame[i]=frame[i+1]=frame[i+2]=mean;
        
    }
    unsigned long t1 = rdtsc();
    //printf("ciclos mono c++: %ld \n", t1-t0);
}

// based from http://www.pages.drexel.edu/~weg22/edge.html
// Author: Bill Green
void edge(unsigned char* frame, int size, int width)
{
    unsigned long t0 = rdtsc();
    
    int height = size/(width*3);
    unsigned int x, y;
    int i, j;
    long sumX, sumY;
    int	nColors, sum;
    unsigned char edge[size];
    unsigned char bnimage[size/3];
    
    int c=0;
    for(int i=0; i < size; i = i + 3) {
        int mean = (frame[i] + frame[i+1] + frame[i+2])/3;
        bnimage[c] = mean;
        c++;    
    }

    char GX[3][3];
    char GY[3][3];

    /* 3x3 GX Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html */
    GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
    GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
    GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;

    /* 3x3 GY Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html */
    GY[0][0] =  1; GY[0][1] =  2; GY[0][2] =  1;
    GY[1][0] =  0; GY[1][1] =  0; GY[1][2] =  0;
    GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;
        
    for(y=0; y<=(height-1); y++)  {
        for(x=0; x<=(width-1); x++)  {
            sumX = 0;
            sumY = 0;

            /* image boundaries */
            if(y==0 || y==height-1)
                sum = 0;
            else if(x==0 || x==width-1)
                sum = 0;

            /* Convolution starts here */
            else   {

                /*-------x GRADIENT APPROXIMATION------*/
                for(i=-1; i<=1; i++)  {
                    for(j=-1; j<=1; j++)  {
                        sumX = sumX + (int)( (*(bnimage + x + i +
                                (y + j)*width)) * GX[i+1][j+1]);
                    }
                }

                
                /*-------y GRADIENT APPROXIMATION-------*/
                for(i=-1; i<=1; i++)  {
                    for(j=-1; j<=1; j++)  {
                        sumY = sumY + (int)( (*(bnimage + x + i +
                                (y + j)*width)) * GY[i+1][j+1]);
                    }
                }

                /*---GRADIENT MAGNITUDE APPROXIMATION (Myler p.218)----*/
                sum = abs(sumX) + abs(sumY);
            }

            if(sum>255) sum=255;

            unsigned char res = ~(unsigned char)(sum);
            /*if(res>100)
                res=255;
            else
                res=0;*/
            *(edge + 3*(x + y*width)) = res;
            *(edge + 3*(x + y*width) + 1) = res; 
            *(edge + 3*(x + y*width) + 2) = res;             
        }
    }
    memcpy(frame, edge, size);
    
    unsigned long t1 = rdtsc();
    //printf("ciclos edge c++: %ld \n", t1-t0);
    
}

void rgb32_rgb24(unsigned int width, unsigned int height, const unsigned char* src, unsigned char* dest)
{
    int size = width*height*4;
    int j = 0;
    for(int i=0; i < size; i = i + 4)
    {
        memcpy(dest + j, src + i, 3);       
        j = j + 3;
    }
}

void rgb32_bgr24(unsigned int width, unsigned int height, const unsigned char* src, unsigned char* dest)
{
    int size = width*height*4;
    int j = 0;
    for(int i=0; i < size; i = i + 4)
    {
        dest[j] = src[i + 2];
        dest[j + 1] = src[i + 1];
        dest[j + 2] = src[i];        
        j = j + 3;
    }
}

void verticalMirror_wrp(unsigned char* frame, int size, int width)
{
    int height = size/(width*3);
    verticalMirror_asm(frame, size, 3*width, height);
}

void instagram_wrp(unsigned char* src, int size, int width)
{
    //Toasted effect ingredients
    brightness_asm(src, size, 40);
    blushing_asm(src, size, 26500);
    contrast_asm(src, size);
    contrast_asm(src, size);

    return;
}

void edge_wrp(unsigned char* frame, int size, int height, int width)
{
    unsigned long t0 = rdtsc();
    
    unsigned char edge[size];
    edge_asm(frame, edge, height, width);
    memcpy(frame, edge, size);
    
    unsigned long t1 = rdtsc();
    //printf("ciclos edge asm: %ld \n", t1-t0);
}

void bona_wrp(unsigned char* src, int size)
{
    static int modelR;
    static int modelG;
    static int modelB;

    int deltaR;
    int deltaG;
    int deltaB;

    int r;
    int g;
    int b;

    int average;

    //float distance;
    int distance;

    //float constant = sqrt(255*255*3);
    //float threshold = 0.35 * constant;
    //float threshold = 0.1225 * 255 * 255 * 3;

    int threshold = 27000;
    bona_asm(src, size, threshold);
    return;

    modelR = 255;
    modelG = 0;
    modelB = 0;

    for(int i = 0; i < size; i = i + 3)
    {
        r = src[i+0];
        g = src[i+1];
        b = src[i+2];

        deltaR = pow((modelR - r), 2);
        deltaG = pow((modelG - g), 2);
        deltaB = pow((modelB - b), 2);

        //distance = sqrt(deltaR + deltaG + deltaB);
        distance = deltaR + deltaG + deltaB;

        if(distance > threshold)
        {
            average = (r+g+b)/3;
            src[i+0] = average;
            src[i+1] = average;
            src[i+2] = average;
        } else {
            //printf("%d %d \n", distance, (int)threshold);
        }

    }
}

void pixelate_wrp(unsigned char* frame, int size, int height, int width)
{
    unsigned long t0 = rdtsc();
    
    static int grade = 0;
    static int goingup = 1;

    if (goingup)
    {
        grade++;
        if (grade > 1)
        {
            goingup = 0;
        }
    }
    else
    {
        grade--;
        if (grade < 1)
        {
            goingup = 1;
        }
    }

    //el grado queda en 2 para la demo
    pixelate_asm(frame, grade, height, width);
    
    unsigned long t1 = rdtsc();
    //printf("ciclos pixelate asm: %ld \n", t1-t0);
}

void negative_wrp(unsigned char* frame, int size)
{
    unsigned long t0 = rdtsc();
    negative_asm(frame, size);
    unsigned long t1 = rdtsc();
    //printf("ciclos negative asm: %ld \n", t1-t0);
}

void blueonly_wrp(unsigned char* frame, int size, int color)
{
    static int grade = 0;
    static int goingup = 1;

    if (goingup)
    {
        grade+=2;
        if (grade > 2)
        {
            goingup = 0;
        }
    }
    else
    {
        grade-=2;
        if (grade < 2)
        {
            goingup = 1;
        }
    }
    printf("%d \n", grade);
    //grade=2;
    // 0=red, 2=green, 4=blue
    blueonly_asm(frame, size, grade);
}

void mono_wrp(unsigned char* frame, int size)
{
    unsigned long t0= rdtsc();
    mono_asm(frame, size);
    unsigned long t1= rdtsc();
    //printf("ciclos mono asm: %ld \n", t1-t0);
    
}

void blur_wrp(unsigned char* frame, int size, int width)
{
    unsigned char edge[size];
    width = width * 3;
    int height = size / width;
    //unsigned char* src, unsigned char* dst, int h en pixels, int w, int src_row_size, int dst_row_siz
    blur_asm(frame, edge, height, width/3, width/3, width/3);
    memcpy(frame, edge, size);
}

int inversaEfecto(int x, int y)
{
    float a= 0.1;
    int b= 1;

    float angle = a*exp(-(x*x+y*y)/(b*b));
    //float angle = a*(x*x+y*y);
    float u = cos(angle)*x + sin(angle)*y;
    float v = -sin(angle)*x + cos(angle)*y;
    if(u+v<x+y)return (int)(u+v);
    else return x+y;
    //return x+y; // descomentar para devolver la identidad
}

void median(unsigned char* image, int size, int width)
{
    /* para medicion de tiempo*/
    unsigned long t0= rdtsc();
    
    //Move window through all elements of the image
    int ancho = width*3;
    int alto = size / ancho;
    unsigned char result[size];

    for (int m = 2; m < alto-2; ++m)
    {
       for (int n = 2; n < ancho-2; n+=3)
       {
          //Pick up window elements
          int k = 0;
          int l = 0;
          int o = 0;
          char windowR[25];
          char windowG[25];
          char windowB[25];
          for (int j = m - 2; j < m + 3; j++)
          {
             for (int i = n - 6; i < n + 9; i+=3)
             {
                windowR[++k] = image[j * ancho + i];
                windowG[++l] = image[j * ancho + i +1];
                windowB[++o] = image[j * ancho + i +2];
             }
          }
          //Order elements (only half of them)
          for (int j = 0; j < 13; ++j)
          {
             //Find position of minimum element
             int min = j;
             for (int l = j + 1; l < 25; ++l)
             {
                if (sqrt(windowR[l]*windowR[l]+windowG[l]*windowG[l]+windowB[l]*windowB[l])
                    < sqrt(windowR[min]*windowR[min]+windowG[min]*windowG[min]+windowB[min]*windowB[min])) min = l;
             }
             //Put found minimum element in its place
             unsigned char tempR = windowR[j];
             unsigned char tempG = windowG[j];
             unsigned char tempB = windowB[j];
             windowR[j] = windowR[min];
             windowG[j] = windowG[min];
             windowB[j] = windowB[min];
             windowR[min] = tempR;
             windowG[min] = tempG;
             windowB[min] = tempB;
          }

          //Get result - the middle element
          result[m * ancho + n] = windowR[13];
          result[m * ancho + n+1] = windowG[13];
          result[m * ancho + n+2] = windowB[13];

      }
    }

    memcpy(image, result, size);
    /* para medicion de tiempos*/
    unsigned long t1= rdtsc();
    //printf("ciclos median c++: %ld \n", t1-t0);
}

void median_wrp(unsigned char* src, int size, int height, int width)
{
    /*para medicion de tiempos*/
    unsigned long t0= rdtsc();
    ////////////////////
    
    unsigned char dst[size];
    median_asm(src, dst, height, width, size);
    // si lo quiero aplicar mas de una vez...
//    unsigned char dst2[size];
//    median_asm(dst, dst2, height, width);
    // aplicandolo 3 veces
//    unsigned char dst3[size];
//    median_asm(dst2, dst3, height, width);
    // copio el dst3, dst2 o dst1 dependiendo de lo que quiero
    // Esto se puede hacer parametrico!
    memcpy(src, dst, size);
    
    /*para medicion de tiempos*/
    unsigned long t1= rdtsc();
    //printf("ciclos median asm: %ld \n", t1-t0);
}

void stretch_wrp(unsigned char* frame, int size, int width)
{
    unsigned char dst[size];
    int height= size/(width*3);
    stretch_asm(frame, dst, height, width, size);
    memcpy(frame, dst, size);
}

/**** RDTSC ROUTINE *******/
static __inline__ unsigned long rdtsc(void)
{
   //unsigned long long int x;
   unsigned a, d;

   __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

   return (((unsigned long)a) | (unsigned long)d >> 32);
}
/**************************/