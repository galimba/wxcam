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

#ifndef _configure_H
#define	_configure_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/propdlg.h> 
#include <wx/generic/propdlg.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <wx/arrstr.h> 
#include <wx/checkbox.h>
#include <iostream>


class ConfigureDialog : public wxPropertySheetDialog
{
    DECLARE_CLASS(ConfigureDialog)
    public:
        ConfigureDialog(wxWindow* win);
        ~ConfigureDialog() {}
        
    private:
        wxArrayString fillValidValues(const wxString &s);
        
    private:
        wxPanel* CreateGlobalSettingsPage(wxWindow* parent);
        wxPanel* CreateSaveFileSettingsPage(wxWindow* parent); 
        wxPanel* CreateVideoSettingsPage(wxWindow* parent); 
        
        wxImageList* m_imageList;        
        wxComboBox* combo;
        wxComboBox* comboAudioDevice;
        wxRadioBox* rgbMode;
        wxRadioBox* v4lVersion;
        wxRadioBox* aviFormat;
        wxStaticBox* staticCompressionLevelBox;
        wxStaticBox* silenceBox;
        wxSlider *qualitySlider;
        wxSlider *hundredthsSlider;
        wxCheckBox *audio;
        wxStaticText *textUnit;
        wxStaticText *textAudioDevice;
        
    private:
        void OnSaveImage(wxCommandEvent&);
        void OnSaveVideo(wxCommandEvent&);
        void OnOk(wxCommandEvent&);
        void OnRadioBoxCompression(wxCommandEvent&);
        void OnAudioChanged(wxCommandEvent&);
        std::string getFileName(const std::string&);
        std::string getDir(const std::string&);

        DECLARE_EVENT_TABLE()        

};

enum {
    ID_COMBOBOX_DEVICE,
    ID_COMBOBOX_AUDIO_DEVICE,
    ID_RADIO_RGB,
    ID_AVI_FORMAT,
    ID_SLIDER,
    ID_SNAP_BUTTON,
    ID_AUDIO,
    ID_VIDEO_BUTTON
};

#endif	/* _configure_H */

