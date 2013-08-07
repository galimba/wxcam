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

#include "picture.h"
#include <iostream>

using namespace std;

BEGIN_EVENT_TABLE(Picture, wxDialog)
        EVT_SLIDER(ID_SLIDER_BRIGHTNESS, Picture::OnBrightnessChanged)
        EVT_SLIDER(ID_SLIDER_CONTRAST, Picture::OnContrastChanged)
        EVT_SLIDER(ID_SLIDER_GAMMA, Picture::OnGammaChanged)
        EVT_SLIDER(ID_SLIDER_SATURATION, Picture::OnSaturationChanged)
        EVT_SLIDER(ID_SLIDER_GAIN, Picture::OnGainChanged)
        EVT_CHECKBOX(ID_CHECKBOX_GAIN, Picture::OnSetAutoGainControl)
        EVT_SLIDER(ID_SLIDER_SHUTTER, Picture::OnShutterChanged)
        EVT_CHECKBOX(ID_CHECKBOX_SHUTTER, Picture::OnSetAutoShutterControl)
END_EVENT_TABLE()

Picture::Picture(Device *cam, wxWindow *parent, bool pwc)
: wxDialog(parent, wxID_ANY, wxString(_("Picture Functions")), wxPoint(0, 0))
{
    this->cam = cam;
        
    if(pwc)
        SetSize(250, 600);
    else
        SetSize(250, 370);
    
    wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(boxSizer);
        
    /*wxStaticText *text = new wxStaticText(this, -1, 
    "Set brightness, contrast, gamma, saturation:");
    boxSizer->Add(text, 0, wxALIGN_CENTER|wxALL, 10);*/
    
    /*brightness*/
    wxBoxSizer *brightnessSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(brightnessSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP, 15);
    
    brightness = new wxSlider(this, ID_SLIDER_BRIGHTNESS,
    GetBrightness(), 0, 100, wxDefaultPosition, wxSize(200, -1), 
    wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
    
    wxStaticText *textBrightness = new wxStaticText(this, -1, _("Brightness:"));
    brightnessSizer->Add(textBrightness, 0, wxALIGN_CENTER|wxALL, 5);
    brightnessSizer->Add(brightness, 0, wxALIGN_CENTER|wxALL, 0);
        
    /*contrast*/
    wxBoxSizer *contrastSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(contrastSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    
    contrast = new wxSlider(this, ID_SLIDER_CONTRAST,
    GetContrast(), 0, 100, wxDefaultPosition, wxSize(200, -1), 
    wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
    
    wxStaticText *textContrast = new wxStaticText(this, -1, _("Contrast:"));
    contrastSizer->Add(textContrast, 0,  wxALIGN_CENTER|wxALL, 5);
    contrastSizer->Add(contrast, 0, wxALIGN_CENTER|wxALL, 0);
    
    /*gamma*/
    wxBoxSizer *gammaSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(gammaSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    
    gamma = new wxSlider(this, ID_SLIDER_GAMMA,
    GetGamma(), 0, 100, wxDefaultPosition, wxSize(200, -1), 
    wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
    
    wxStaticText *textGamma = new wxStaticText(this, -1, _("Gamma:"));
    gammaSizer->Add(textGamma, 0,  wxALIGN_CENTER|wxALL, 5);
    gammaSizer->Add(gamma, 0, wxALIGN_CENTER|wxALL, 0);
    
    /*saturation*/
    wxBoxSizer *saturationSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(saturationSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    
    saturation = new wxSlider(this, ID_SLIDER_SATURATION,
    GetSaturation(), 0, 100, wxDefaultPosition, wxSize(200, -1), 
    wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
    
    wxStaticText *textSaturation = new wxStaticText(this, -1, _("Saturation:"));
    saturationSizer->Add(textSaturation, 0, wxALIGN_CENTER|wxALL, 5);
    saturationSizer->Add(saturation, 0, wxALIGN_CENTER|wxALL, 0);
    

    if(pwc) {   
        /*gain*/
        wxBoxSizer *gainSizer = new wxBoxSizer(wxVERTICAL);
        boxSizer->Add(gainSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP, 15);

        int gainValue = GetGain();
        int absGain;
        if(gainValue < 0)
            absGain = -gainValue;
        else {
            absGain = gainValue;
            absGain = (absGain * (float)100 / 65535 + 0.5);
        }
        gain = new wxSlider(this, ID_SLIDER_GAIN,
        absGain, 0, 100, wxDefaultPosition, wxSize(200, -1), 
        wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);

        wxStaticText *textGain = new wxStaticText(this, -1, _("Gain:"));
        gainSizer->Add(textGain, 0, wxALIGN_CENTER|wxALL, 5);
        gainSizer->Add(gain, 0, wxALIGN_CENTER|wxALL, 0);       

        checkboxGain = new wxCheckBox(this, ID_CHECKBOX_GAIN,
        _("Auto gain control"), wxDefaultPosition, wxDefaultSize);
        checkboxGain->SetValue( (gainValue < 0) ? true : false );    
        gainSizer->Add(checkboxGain, 0, wxALIGN_CENTER|wxALL, 10);    

        /*shutter speed*/
        wxBoxSizer *shutterSizer = new wxBoxSizer(wxVERTICAL);
        boxSizer->Add(shutterSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP, 5);

        int shutterValue = GetShutter();
        int absShutter;
        if(shutterValue < 0)
            absShutter = -shutterValue;
        else {
            absShutter = shutterValue;
            absShutter = (absShutter * (float)100 / 65535 + 0.5);
        }
        shutter = new wxSlider(this, ID_SLIDER_SHUTTER,
        absShutter, 0, 100, wxDefaultPosition, wxSize(200, -1), 
        wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS);
        if(shutterValue < 0)
            shutter->Enable(false);

        wxStaticText *textShutter = new wxStaticText(this, -1, _("Shutter speed:"));
        shutterSizer->Add(textShutter, 0, wxALIGN_CENTER|wxALL, 5);
        shutterSizer->Add(shutter, 0, wxALIGN_CENTER|wxALL, 0);       

        checkboxShutter = new wxCheckBox(this, ID_CHECKBOX_SHUTTER,
        _("Auto shutter speed control"), wxDefaultPosition, wxDefaultSize);
        checkboxShutter->SetValue( (shutterValue < 0) ? true : false );    
        shutterSizer->Add(checkboxShutter, 0, wxALIGN_CENTER|wxALL, 10);
        if(shutterValue < 0)
            checkboxShutter->Enable(false);
    }
          
    //button ok
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 15);
    
    wxButton *ok = new wxButton(this, wxID_OK, _("&OK"), 
        wxDefaultPosition, wxDefaultSize, 0);
    buttonSizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);   
    
}

Picture::~Picture()
{

}

void Picture::OnBrightnessChanged(wxCommandEvent& WXUNUSED( event ))
{         
    int val = (brightness->GetValue()*65535)/100;
    cam->setBrightness(val);    
}

void Picture::OnContrastChanged(wxCommandEvent& WXUNUSED( event ))
{       
    int val = (contrast->GetValue()*65535)/100;
    cam->setContrast(val); 
}

void Picture::OnGammaChanged(wxCommandEvent& WXUNUSED( event ))
{
    int val = (gamma->GetValue()*65535)/100;
    cam->setGamma(val);    
}

void Picture::OnSaturationChanged(wxCommandEvent& WXUNUSED( event ))
{
    int val = (saturation->GetValue()*65535)/100;
    cam->setSaturation(val); 
}

void Picture::OnGainChanged(wxCommandEvent& WXUNUSED( event ))
{
    //cout<<"OnGainChanged"<<endl<<flush;
    int agc;
    if( !checkboxGain->IsChecked() ) {
        agc = (gain->GetValue()*65535)/100; 
        cam->setGain(&agc);
    }    
}

void Picture::OnSetAutoGainControl(wxCommandEvent& event)
{
    bool b = event.IsChecked();
    int agc;
    if(b) {
        agc = -1;
        cam->setGain(&agc);
        checkboxShutter->Enable(false);
        shutter->Enable(false);
    }
    else {
       agc = (gain->GetValue()*65535)/100; 
       cam->setGain(&agc);
       checkboxShutter->Enable(true);
       shutter->Enable(true);
       shutter->SetValue(100);
    }
        
}

void Picture::OnShutterChanged(wxCommandEvent& WXUNUSED( event ))
{
    //cout<<"OnShutterChanged"<<endl<<flush;
    int ishutter;
    if( !checkboxShutter->IsChecked() ) {
        ishutter = (shutter->GetValue()*65535)/100;
        cam->setShutter(&ishutter);
    }    
}

void Picture::OnSetAutoShutterControl(wxCommandEvent& event)
{
    bool b = event.IsChecked();
    int ishutter;
    if(b) {
        int autoShutter = 65535;
        ishutter = -1;
        cam->setShutter(&autoShutter);
        cam->setShutter(&ishutter);
    }
    else {
       ishutter = (shutter->GetValue()*65535)/100; 
       cam->setShutter(&ishutter);
    }        
}

int Picture::GetBrightness()
{
    unsigned int ret;
    cam->getBrightness(ret);
    ret = (int)((ret/(float)65535*100) + 0.5);
    return ret;
}

int Picture::GetContrast()
{
    unsigned int ret;
    cam->getContrast(ret);
    ret = (int)((ret/(float)65535*100) + 0.5);
    return ret;
}

int Picture::GetGamma()
{
    unsigned int ret;
    cam->getGamma(ret);
    ret = (int)((ret/(float)65535*100) + 0.5);
    return ret;
}

int Picture::GetSaturation()
{
    unsigned int ret;
    cam->getSaturation(ret);
    ret = (int)((ret/(float)65535*100) + 0.5);
    return ret;
}

int Picture::GetGain()
{
    int agc;
    cam->getGain(&agc);
    return agc;
}

int Picture::GetShutter()
{
    int sht;
    cam->getShutter(&sht);
    return sht;
}

