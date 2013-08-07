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

#ifndef _picture_H
#define	_picture_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include "device.h"

class Picture : public wxDialog
{
    public:
        Picture(Device *cam, wxWindow *parent=NULL, bool pwc=false);
        ~Picture();
        
    private:
        void OnBrightnessChanged(wxCommandEvent&);
        void OnContrastChanged(wxCommandEvent&);
        void OnGammaChanged(wxCommandEvent&);
        void OnSaturationChanged(wxCommandEvent&); 
        void OnGainChanged(wxCommandEvent&); 
        void OnSetAutoGainControl(wxCommandEvent&);
        void OnShutterChanged(wxCommandEvent&); 
        void OnSetAutoShutterControl(wxCommandEvent&);
        int GetBrightness();
        int GetContrast();
        int GetGamma();
        int GetSaturation();
        int GetGain();
        int GetShutter();
        
    private:
        Device *cam;
        
    private:
        wxSlider *brightness;
        wxSlider *contrast;
        wxSlider *gamma;
        wxSlider *saturation;
        wxSlider *gain;
        wxCheckBox *checkboxGain;
        wxSlider *shutter;
        wxCheckBox *checkboxShutter;
        DECLARE_EVENT_TABLE()
    
};

enum {
    ID_SLIDER_BRIGHTNESS,
    ID_SLIDER_CONTRAST,
    ID_SLIDER_GAMMA,
    ID_SLIDER_SATURATION,
    ID_SLIDER_GAIN,
    ID_CHECKBOX_GAIN,
    ID_SLIDER_SHUTTER,
    ID_CHECKBOX_SHUTTER
};


#endif	/* _picture_H */

