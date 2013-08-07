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

#ifndef _progressdlg_H
#define	_progressdlg_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/gauge.h>

class ProgressDlg : public wxDialog
{

    public:
        ProgressDlg(wxWindow *parent=NULL);
        ~ProgressDlg();
        
        void Update(int n);         
        
    private:
	wxGauge *gauge;
        wxStaticText *textProgress;
        wxString progressString;
    
};

enum {
	ID_GAUGE
};

#endif	/* _progressdlg_H */

