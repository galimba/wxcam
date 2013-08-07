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

#ifndef _resolution_H
#define	_resolution_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/arrstr.h> 

class ResolutionDialog : public wxDialog
{
    public:
        ResolutionDialog(wxArrayString validValues, wxWindow *parent=NULL, bool pwc=false);
        
        wxString GetResolution();    
        wxString GetFrameRate();  
        
    private:
        void fillFrameRate(wxArrayString &frameRateValues) const;
       
    private:
        wxComboBox *comboResolution;
        wxComboBox *comboFrameRate;
    //DECLARE_EVENT_TABLE()
};

enum {
    ID_COMBOBOX_RESOLUTION,
    ID_COMBOBOX_FRAME_RATE
};


#endif	/* _capabilities_H */

