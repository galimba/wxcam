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

#ifndef _uncompressed_H
#define	_uncompressed_H

#include "avi.h"
#include <string>
#include <list>
#include <wx/thread.h>

#define AVIF_HASINDEX           0x00000010      
#define AVIF_MUSTUSEINDEX       0x00000020
#define AVIF_ISINTERLEAVED      0x00000100
#define AVIF_WASCAPTUREFILE     0x00010000
#define AVIF_COPYRIGHTED        0x00020000

typedef char byte;
class wxWindow;

class AviUncompressed : public Avi
{
    public:
        AviUncompressed();
        ~AviUncompressed();
        
        void addFrame(const char* frame);
        int getBufferSize();
        void record();
        int setup() { return 0; };
        
    private:
        enum endianness {
            LITTLE_ENDIAN_ARCH,
            BIG_ENDIAN_ARCH
        };
        
    private:
        void *Entry();
        void writeQueuedFrame(const char* frame);
        void writeFile();
        void endianTest();
        int writeString(const char* bytes);
        int writeIntegerSafe(int val);
        int writeShortIntegerSafe(short int val);
        void writeAVIH();
        void writeSTRH();
        void writeBMIH();
        int writeFrame(byte *input);
        
    private:
        int max;
        std::list<std::string> queue;
        bool endRecord;
        wxMutex mutex;
        wxCondition cond;
        bool firstFrame;
        bool fileOpened;
        int totalFrames;
        std::string tmpfile;
        endianness arch;
        int fd;
    
};

#endif	/* _uncompressed_H */

