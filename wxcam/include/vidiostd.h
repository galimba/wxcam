/***************************************************************************
 *   Copyright (C) 2008 by Marco Lorrai                                    *
 *   iking@aruba.it                                              *
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
#ifndef _VIDIOSTD_H
#define	_VIDIOSTD_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include "device.h"


class VidioStd : public wxDialog
{    
public:    
    VidioStd(wxWindow *parent=NULL);
    ~VidioStd();  
    
public:
    wxRadioBox *stdMode;
    
private:
     void loadValue();
    
private:
    wxButton *ok;
    wxButton *cancel;    
    
    //DECLARE_EVENT_TABLE()
};

#endif	/* _VIDIOSTD_H */

