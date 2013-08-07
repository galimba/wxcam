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

#include "resolution.h"
#include "setting.h"
#include <wx/sizer.h>

/*BEGIN_EVENT_TABLE(ResolutionDialog, wxDialog)
        EVT_CLOSE(ResolutionDialog::OnCloseOk)
END_EVENT_TABLE()*/


ResolutionDialog::ResolutionDialog(wxArrayString validValues, wxWindow *parent, bool pwc)
: wxDialog(parent, wxID_ANY, wxString(_("Frame settings")), wxPoint(0, 0))
{
    comboResolution = NULL;
    comboFrameRate = NULL;
    
    if(pwc)
        SetSize(260, 250);
    else
        SetSize(260, 180);
    
    wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer);
    
    wxStaticText *textResolution = new wxStaticText(this, -1, _("Choose resolution:"));
    boxSizer->Add(textResolution, 0, wxALIGN_CENTER|wxTOP, 20);
    
    comboResolution = new wxComboBox(this, ID_COMBOBOX_RESOLUTION, 
    wxT("- - -"), wxDefaultPosition, wxDefaultSize, validValues/*, wxCB_READONLY*/);
    comboResolution->SetValue(wxString(Setting::GetInstance()->GetResolution().c_str(), wxConvUTF8));
    boxSizer->Add(comboResolution, 0, wxALIGN_CENTER|wxALL, 10);
    
    if(pwc) {
        wxStaticText *textFrameRate = new wxStaticText(this, -1, _("Choose frame rate (fps):"));
        boxSizer->Add(textFrameRate, 0, wxALIGN_CENTER|wxTOP, 20);
        wxArrayString frameRateValues;
        fillFrameRate(frameRateValues);

        comboFrameRate = new wxComboBox(this, ID_COMBOBOX_FRAME_RATE, 
        wxT("- - -"), wxDefaultPosition, wxDefaultSize, frameRateValues, wxCB_READONLY);
        comboFrameRate->SetValue(wxString(Setting::GetInstance()->GetFrameRate().c_str(), wxConvUTF8));
        boxSizer->Add(comboFrameRate, 0, wxALIGN_CENTER|wxALL, 10);
    }
    
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 15);
    
    wxButton *ok = new wxButton(this, wxID_OK, _("&OK"), 
        wxDefaultPosition, wxDefaultSize, 0);
    buttonSizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    
    wxButton *cancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), 
        wxDefaultPosition, wxDefaultSize, 0);
    buttonSizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    
}

wxString ResolutionDialog::GetResolution()
{
    return comboResolution->GetValue();
}

wxString ResolutionDialog::GetFrameRate()
{
    if(comboFrameRate)
        return comboFrameRate->GetValue();
    else
        return wxEmptyString;
}

void ResolutionDialog::fillFrameRate(wxArrayString &frameRateValues) const
{
    frameRateValues.Add(wxString("5", wxConvUTF8));
    frameRateValues.Add(wxString("10", wxConvUTF8));
    frameRateValues.Add(wxString("15", wxConvUTF8));
    frameRateValues.Add(wxString("20", wxConvUTF8));
    frameRateValues.Add(wxString("25", wxConvUTF8));
    frameRateValues.Add(wxString("30", wxConvUTF8));
}

/*void ResolutionDialog::OnCloseOk(wxCloseEvent& event)                
{
     EndModal(wxID_OK);               
}*/
