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

#include "progressdlg.h"
#include <sstream>

ProgressDlg::ProgressDlg(wxWindow *parent) 
: wxDialog(parent, wxID_ANY, wxString(_("Saving file...")), wxDefaultPosition)
{
    SetSize(200, 120);
    progressString = wxString("0%", wxConvUTF8);
    
    wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(boxSizer);
        
    gauge = new wxGauge(this, ID_GAUGE, 100, wxDefaultPosition, 
    wxSize(150, 30), wxGA_HORIZONTAL);    
    wxBoxSizer *gaugeSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(gaugeSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP, 5);    
    gaugeSizer->Add(gauge, 0, wxALIGN_CENTER|wxTOP, 30);
    
    textProgress = new wxStaticText(this, -1, progressString);
    gaugeSizer->Add(textProgress, 0, wxALIGN_CENTER|wxALL, 5);
    
    gauge->SetRange(100);
    gauge->SetValue(0);

}

ProgressDlg::~ProgressDlg()
{
    gauge->Destroy();
}

void ProgressDlg::Update(int n) 
{    
    gauge->SetValue(n);
    std::ostringstream ostr;
    ostr<<n<<"%";
    progressString = wxString(ostr.str().c_str(), wxConvUTF8);
    textProgress->SetLabel(progressString);
}

