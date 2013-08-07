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

#ifndef _avi_H
#define	_avi_H

#include <iostream>
#include <string>
#include <wx/thread.h>
#include "cfile.h"

class wxWindow;

class Avi : public wxThread
{
public:
    Avi() {dimFreeMem = 0;}
    virtual ~Avi() {}
    void setParameters(wxWindow *parent, const std::string &filename, int fps, int width, int height)
    {
        this->fps = fps;
        this->width = width;
        this->height = height;
        this->filename = filename;
        this->parent = parent;
        frameSize = width*height*3;
    }
    virtual int  setup() = 0;
    virtual void addFrame(const char*) = 0;
    virtual void record() = 0;
    int calculateFreeMem() {
        try {
            CFile memInfo("/proc/meminfo");
            std::string value = memInfo.readValue("MemTotal:");
            sscanf( value.c_str(), "%ld", &dimFreeMem );
            //dimFreeMem /= 2;
            return dimFreeMem;
        }
        catch(...) {
            return 0;
        }
        return dimFreeMem;
    }
    
protected:
    unsigned long dimFreeMem;
    wxWindow *parent;
    std::string filename;
    int fps, width, height;
    int frameSize;
};


#endif	/* _avi_H */

