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

#include "recording.h"
#include <iostream>
#include <wx/button.h>

#include "record_image.xpm"

using namespace std;

BEGIN_EVENT_TABLE(Recording, wxDialog)
    EVT_BUTTON(ID_STOP_RECORDING, Recording::OnStopRecording)
END_EVENT_TABLE()  

Recording::Recording(wxWindow *parent)
: wxDialog(parent, wxID_ANY, wxString(_("Recording")), wxPoint(300, 20))
{
    SetSize(280, 145);
        
    wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(boxSizer);
    
    wxBoxSizer *image = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(image, 0, wxALIGN_LEFT|wxEXPAND|wxALL, 20);
    
    wxStaticBitmap *rec = new wxStaticBitmap(this, -1, record_image_xpm, wxDefaultPosition);
    image->Add(rec, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxLEFT, 15);
    
    wxBoxSizer *text = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(text, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxLEFT, 40);
    
    textRecording = new wxStaticText(this, -1, _("Recording..."));
    
    SetForegroundColour( wxColour(wxT("RED")) );
    timeRecording = new wxStaticText(this, -1, wxT("00:00.0"));
    wxFont font = timeRecording->GetFont();
    font.SetPointSize(16);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    timeRecording->SetFont(font);
    
    wxButton *stop = new wxButton(this, ID_STOP_RECORDING, _("&Stop"), 
        wxDefaultPosition, wxDefaultSize, 0);
    
    text->Add(textRecording, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP, 15);
    text->Add(timeRecording, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP, 15);
    text->Add(stop, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP, 15);
    strec = RECORDING;
    
}

Recording::~Recording()
{
    
}

void Recording::setRecordingTime(const wxTimeSpan &ts)
{
    char label[20];    
    wxTimeSpan tmp = ts;
    tmp -= secPause;
    long tenths_sec = (tmp.GetMilliseconds().ToLong()/100);
    unsigned int minutes = tenths_sec/600;
    unsigned int seconds = tenths_sec/10 - minutes*60;
    unsigned int tenths =  tenths_sec - seconds*10 - minutes*600;
    snprintf(label, 20, "%02d:%02d.%d", minutes, seconds, tenths);
    timeRecording->SetLabel(wxString(label, wxConvUTF8));
}

void Recording::SetRecordingStatus(bool recording) 
{
    if (recording && strec==PAUSED) {
        textRecording->SetLabel(_("&Recording..."));
        strec = RECORDING;
        if(timeRecording->GetLabel() != wxT("00:00.0")) {
            wxDateTime now = wxDateTime::UNow();
            secPause += now.Subtract(pauseStart);
        }
    }
    else if (!recording && strec==RECORDING) {
        strec = PAUSED;
        textRecording->SetLabel(_("&Paused"));
        pauseStart = wxDateTime::UNow();
    }
}

void Recording::OnStopRecording(wxCommandEvent& WXUNUSED( event ))
{
    Close();
}
