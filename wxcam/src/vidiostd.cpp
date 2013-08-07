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

#include "vidiostd.h"
#include "setting.h"
#include "tv.xpm"

VidioStd::VidioStd(wxWindow *parent)
        : wxDialog(parent, wxID_ANY, wxString(_("Choose video standard")), wxPoint(0, 0))
{
    SetSize(250, 220);
    CenterOnScreen();
    SetIcon(tv_xpm);
    
    wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(boxSizer);
    
   /* wxStaticText *text = new wxStaticText(this, -1, 
    _("Set a video standard:"));
    boxSizer->Add(text, 0, wxALIGN_CENTER|wxALL, 10);*/
    
    wxString choices[4];
    choices[0] = _T("&PAL");
    choices[1] = _T("&NTSC");
    choices[2] = _T("&SECAM");
    choices[3] = _("&NONE");
    
    stdMode = new wxRadioBox(this, -1, _("&Standard:"),
        wxDefaultPosition, wxSize(230, 120), 4, choices);
    stdMode->SetStringSelection(wxString(Setting::GetInstance()->GetFrameFormat().c_str(), wxConvUTF8));
    boxSizer->Add(stdMode, 0, wxALIGN_CENTER|wxALL, 10);
    
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 15);
    
    wxButton *ok = new wxButton(this, wxID_OK, _("&OK"), 
        wxDefaultPosition, wxDefaultSize, 0);
    buttonSizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    
    wxButton *cancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), 
        wxDefaultPosition, wxDefaultSize, 0);
    buttonSizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    
    loadValue();
}

VidioStd::~VidioStd()
{
    
}

void VidioStd::loadValue()
{
    video_std vs = Setting::GetInstance()->GetVideoStd();
    stdMode->SetSelection((int)vs);
}


