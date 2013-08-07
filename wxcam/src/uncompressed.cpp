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

#include "uncompressed.h"
#include "wxcam.h"
#include "frame.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/window.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

AviUncompressed::AviUncompressed() : cond(mutex) 
{
    endianTest();
    totalFrames=0;
    endRecord=false;
    firstFrame=true;
    fileOpened=false;
    max=0;
}
    
AviUncompressed::~AviUncompressed() 
{
    //cout<<"~AviUncompressed()"<<endl;
}
    
//main thread
void AviUncompressed::addFrame(const char* frame) {
    static bool messageShown = false;
    if(firstFrame) {
        messageShown = false;
        calculateFreeMem();
        firstFrame = false;
        fd = open(filename.c_str(), O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
        if(fd == -1) {
            perror("Error opening avi file");
            wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, STOP_REC );
            event.SetInt(1);
            wxPostEvent( parent, event );
            return;
        }
        else
            fileOpened=true;
        byte zero[224];
        memset(zero, 0, 224);
        int res = write(fd, zero, 224);
    }
    if(fileOpened) {
        if(dimFreeMem > 0) {
            mutex.Lock();
            int totalByte = (queue.size() + 1) * frameSize;
            mutex.Unlock();
            if( (dimFreeMem * 1024) < totalByte) {
                if( !messageShown ) {
                    cout<<"Buffer full"<<endl;
                    wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, STOP_REC );
                    event.SetInt(2);
                    wxPostEvent( parent, event );
                    messageShown = true;
                }
                return;
            }
        }
        mutex.Lock();
        queue.push_back(std::string(frame, frameSize));
        cond.Signal();
        mutex.Unlock();
    }
}

//Recording thread
void* AviUncompressed::Entry() {
    while(true) {
        mutex.Lock();
        while(!queue.size() && !endRecord)
            cond.Wait();
        //cout<<"Queue size: "<<queue_size<<endl<<flush;
        if(endRecord && !queue.size()) { //registration terminated
            mutex.Unlock();
            if(fileOpened)
                writeFile();
            break;
        }

        string strFrame = queue.front();
        queue.pop_front();

        //max not yet initialize, first execution after calling record()
        if(endRecord && !max) {
            max = queue.size();
            if(max > 10) {
                wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, PROGRESS_DIALOG_START );
                wxPostEvent( parent, event );
            }
            else
                max=-1;
        }

        //max already initialized
        if(endRecord && max > 0) {
            wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, PROGRESS_DIALOG_EVENT );
            int n = (int)(((float)max - (float)queue.size()) / (float)max * 100);
            event.SetInt( n );

            // send in a thread-safe way
            wxPostEvent( parent, event );
        }

        mutex.Unlock();

        const char* frame = strFrame.c_str();
        writeQueuedFrame(frame);
    }
    if(max > 0) {
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, PROGRESS_DIALOG_EVENT );

        event.SetInt(-1); // that's all
        wxPostEvent( parent, event );
    }
    wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, STOP_REC );
    event.SetInt(0);
    wxPostEvent( parent, event );
    return NULL;
}

//main thread
void AviUncompressed::record() {
    mutex.Lock();
    endRecord=true;
    cond.Signal();
    mutex.Unlock();
}

void AviUncompressed::endianTest() {
    byte test[2];
    short int *t_int;
    test[0] = 0;
    test[1] = 4;
    t_int = (short int*)&test;
    if(*t_int == 1024) {
        //LITTLE ENDIAN!!
        arch = LITTLE_ENDIAN_ARCH;
//        cout<<"Found little endian architecture"<<endl;
    }
    else { //*t_int == 4
        arch = BIG_ENDIAN_ARCH;
//        cout<<"Found big endian architecture"<<endl;
    }
}

int AviUncompressed::writeString(const char* bytes) {
    int size = strlen(bytes);
    int written = write(fd, bytes, size);
    if(written == -1) {
        perror("Error writing");
    }
    return written;
}

int AviUncompressed::writeIntegerSafe(int val)  //size: 4 byte
{
    byte *s_int;
    if(arch == BIG_ENDIAN_ARCH) { //BIG_ENDIAN
        s_int = new byte[4];
        byte *tmp_val = (byte*)&val;
        for(int i=0, j=3; i<4; j--, i++)
            s_int[j] = tmp_val[i];
        //s_int[3] = val/16777216;
        //s_int[2] = (val - b_int[3])/65536;
        //s_int[1] = (val - b_int[2])/256;
        //s_int[0] = val - b_int[1];
    }
    else {
        s_int = (byte*)&val;
    }
    int written = write(fd, s_int, 4);
    if(arch == BIG_ENDIAN_ARCH)
        delete [] s_int;
    if(written == -1) {
        perror("Error writing");
    }
    return written;
}

int AviUncompressed::writeShortIntegerSafe(short int val)  //size: 2 byte
{
    byte *s_int;
    if(arch == BIG_ENDIAN_ARCH) { //BIG_ENDIAN
        s_int = new byte[2];
        byte *tmp_val = (byte*)&val;
        s_int[0] = tmp_val[1];
        s_int[1] = tmp_val[0];
    }
    else {
        s_int = (byte*)&val;
    }
    int written = write(fd, s_int, 2);
    if(arch == BIG_ENDIAN_ARCH)
        delete [] s_int;
    if(written == -1) {
        perror("Error writing");
    }
    return written;
}

void AviUncompressed::writeAVIH() 
{
    writeString("avih");
    writeIntegerSafe(56);
    writeIntegerSafe((int)(1000000.0 / fps)); //dwMicroSecPerFrame
    writeIntegerSafe(0); //dwMaxBytesPerSec
    writeIntegerSafe(0); //dwPaddingGranularity
    writeIntegerSafe(AVIF_HASINDEX | AVIF_WASCAPTUREFILE); //dwFlags
    writeIntegerSafe(totalFrames); //dwTotalFrames
    writeIntegerSafe(0); //dwInitialFrames
    writeIntegerSafe(1); //dwStreams, video only
    writeIntegerSafe(frameSize); //dwSuggestedBufferSize
    writeIntegerSafe(width); //dwWidth
    writeIntegerSafe(height); //dwHeight
    writeIntegerSafe(0);  //dwReserved
    writeIntegerSafe(0); //dwReserved
    writeIntegerSafe(0); //dwReserved
    writeIntegerSafe(0); //dwReserved
}

void AviUncompressed::writeSTRH() 
{
    writeString("vids");
    writeString("DIB "); //fccHandler
    writeIntegerSafe(0);  //dwFlags
    writeIntegerSafe(0);  //dwPriority, dwLanguage
    writeIntegerSafe(0);  //dwInitialFrame
    int dwMicroSecPerFrame = (int)(1000000.0 / fps);
    writeIntegerSafe(dwMicroSecPerFrame); //dwScale
    writeIntegerSafe(1000000); //dwRate
    writeIntegerSafe(0); //dwStart
    writeIntegerSafe(totalFrames); // dwLength
    writeIntegerSafe(frameSize); //dwSuggestedBufferSize
    writeIntegerSafe(0); //dwQuality
    writeIntegerSafe(0); //dwSampleSize
    writeIntegerSafe(0); //rcFrame
    writeShortIntegerSafe(width); //rcFrame
    writeShortIntegerSafe(height); //rcFrame
}

void AviUncompressed::writeBMIH() 
{
    writeIntegerSafe(40); //biSize
    writeIntegerSafe(width); //biWidth
    writeIntegerSafe(height); //biHeight
    writeShortIntegerSafe(1); //biPlanes
    writeShortIntegerSafe(24); //biBitCount
    writeIntegerSafe(0); //biCompression
    writeIntegerSafe(0); //biSizeImage
    writeIntegerSafe(0); //biXPelsPerMeter
    writeIntegerSafe(0); //biYPelsPerMeter
    writeIntegerSafe(0); //biClrUsed
    writeIntegerSafe(0); //biClrImportant
}

int AviUncompressed::writeFrame(byte *input) 
{
    writeString("00db");
    writeIntegerSafe(frameSize);

    //starting from the last line
    //and inverting RGB with GBR

    for(int i = height-1; i>=0; i--) {
        byte line[width*3];
        for( int j = 0; j<(width*3); j = j + 3 ) {
            int row_pos = (i * width * 3) + j + 2;
            line[j] = input[row_pos];
            row_pos = (i * width * 3) + j + 1;
            line[j + 1] = input[row_pos];
            row_pos = (i * width * 3) + j;
            line[j + 2] = input[row_pos];
        }
        int ret = write(fd, line, width*3);
        if(ret == -1) {
            perror("Error writing");
            return ret;
        }
    }

    return frameSize;
}

void AviUncompressed::writeQueuedFrame(const char* frame) 
{
    int ret = writeFrame((byte*)frame);
    if(ret == -1) {
        perror("Error writing");
        return;
    }
    totalFrames++;
}

void AviUncompressed::writeFile() 
{
    int indexSize = totalFrames * 16;
    int moviSize = 4 + totalFrames * (frameSize + 8);
    int secondListSize = 4 + 8 + 56 + 48;
    int firstListSize =  4 + 64 + (8 + secondListSize);
    int fileSize = (8 + firstListSize) + 8 + (4 + moviSize) + (8 + indexSize);

    lseek(fd, 0, SEEK_SET);

    writeString("RIFF");
    writeIntegerSafe(fileSize);
    writeString("AVI ");
    writeString("LIST");
    writeIntegerSafe(firstListSize);
    writeString("hdrl");
    writeAVIH();  //64 byte
    //88 byte written

    writeString("LIST");
    writeIntegerSafe(secondListSize);
    writeString("strl");
    writeString("strh");
    writeIntegerSafe(56);
    writeSTRH(); //56 byte
    writeString("strf");
    writeIntegerSafe(40);
    writeBMIH();  //40 byte
    //212 byte written

    writeString("LIST");
    writeIntegerSafe(moviSize);
    writeString("movi");
    //224 total byte

    //Replaced first 224 zero!

    lseek(fd, 0, SEEK_END);
    //I go to end of file and write index

    writeString("idx1");
    writeIntegerSafe(indexSize);
    int chunkOffset = 4;

    int frameNumber;
    for( frameNumber = 1; frameNumber <= totalFrames; frameNumber++ ) {
        writeString("00db");
        writeIntegerSafe(16);
        writeIntegerSafe(chunkOffset);
        chunkOffset += frameSize + 8;
        writeIntegerSafe(frameSize);
    }

    close(fd);
}

