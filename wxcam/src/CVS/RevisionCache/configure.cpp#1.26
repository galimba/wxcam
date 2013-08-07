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

#include "configure.h"
#include "setting.h"
#include <wx/bookctrl.h>
#include <wx/spinctrl.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <iostream>
#include <string>
#include <sstream>

//images
#include "device.xpm"
#include "save.xpm"
#include "film.xpm"

IMPLEMENT_CLASS(ConfigureDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(ConfigureDialog, wxPropertySheetDialog)
EVT_BUTTON (ID_SNAP_BUTTON, ConfigureDialog::OnSaveImage)
EVT_BUTTON (ID_VIDEO_BUTTON, ConfigureDialog::OnSaveVideo)
EVT_BUTTON (wxID_OK, ConfigureDialog::OnOk)
EVT_RADIOBOX (ID_AVI_FORMAT, ConfigureDialog::OnRadioBoxCompression)
EVT_CHECKBOX (ID_AUDIO, ConfigureDialog::OnAudioChanged)
END_EVENT_TABLE()

ConfigureDialog::ConfigureDialog(wxWindow* win) 
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);
    
    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    resizeBorder = 0;
    tabImage1 = 0;
    tabImage2 = 1;
    tabImage3 = 2;
    int sheetStyle = wxPROPSHEET_SHRINKTOFIT;
   
    sheetStyle |= wxPROPSHEET_TOOLBOOK;

    SetSheetStyle(sheetStyle);
    SetSheetInnerBorder(0);
    SetSheetOuterBorder(0);

    // create the image list

    m_imageList = new wxImageList(32, 32);
    m_imageList->Add(wxIcon(device_xpm));
    m_imageList->Add(wxIcon(save_xpm));
    m_imageList->Add(wxIcon(film_xpm));
   
    Create(win, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE);
    
    CreateButtons(wxOK | wxHELP| wxCANCEL);
    
    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* generalSettings = CreateGlobalSettingsPage(notebook);
    wxPanel* saveSettings = CreateSaveFileSettingsPage(notebook);
    wxPanel* videoSettings = CreateVideoSettingsPage(notebook);

    notebook->AddPage(generalSettings, _("  Global  "), true, tabImage1);
    notebook->AddPage(saveSettings, _("  Files  "), false, tabImage2);
    notebook->AddPage(videoSettings, _("  Video  "), false, tabImage3);

    LayoutDialog();
}

wxPanel* ConfigureDialog::CreateGlobalSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);  
    
    wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(boxSizer);
    
    wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(vSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP, 0);    
    boxSizer->Add(hSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxTOP, 0);    
        
    wxArrayString validValues = fillValidValues(wxString("video*", wxConvUTF8));
    wxStaticText *textDevice = new wxStaticText(panel, -1, _("Webcam device:"));
    combo = new wxComboBox(panel, ID_COMBOBOX_DEVICE, 
    wxT("- - -"), wxDefaultPosition, wxDefaultSize, validValues);
    combo->SetValue(wxString(Setting::GetInstance()->GetDeviceFile().c_str(), wxConvUTF8));
    
    vSizer->Add(textDevice, 0, wxALIGN_CENTER|wxALL, 5);
    vSizer->Add(combo, 0, wxALIGN_CENTER|wxBOTTOM, 15);
    
    wxString choices[8];
    choices[0] = _("&YUV420P");
    choices[1] = _("&YUV420");
    choices[2] = _("&YUYV");
    choices[3] = _("&RGB24");
    choices[4] = _("&RGB32");
    choices[5] = _("&JPEG");
    choices[6] = _("&MPEG");
    choices[7] = _("&AUTO");
    rgbMode = new wxRadioBox(panel, ID_RADIO_RGB, _("&Frame format:"),
        wxDefaultPosition, wxDefaultSize, 8, choices);
    rgbMode->SetStringSelection(wxString(Setting::GetInstance()->GetFrameFormat().c_str(), wxConvUTF8));
    hSizer->Add(rgbMode, 0, wxALIGN_CENTER|wxALL, 10);
    
    choices[0] = _("&Video4Linux1");
    choices[1] = _("&Video4Linux2");
    choices[2] = _("&AUTO");

    v4lVersion = new wxRadioBox(panel, ID_RADIO_RGB, _("&Driver:"),
            wxDefaultPosition, wxDefaultSize, 3, choices);
    v4lVersion->SetStringSelection(wxString(Setting::GetInstance()->GetDriverAPI().c_str(), wxConvUTF8));
    hSizer->Add(v4lVersion, 0, wxALIGN_CENTER|wxALL, 10);
    
    return panel;
}

wxPanel* ConfigureDialog::CreateSaveFileSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);
    wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(boxSizer);
    
    wxStaticBox* staticSnapBox = new wxStaticBox(panel, wxID_ANY, _("Choose snapshot file"));
    wxBoxSizer *snapSizer = new wxStaticBoxSizer(staticSnapBox, wxVERTICAL);
    boxSizer->Add(snapSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 20);
    wxButton *snapButton = new wxButton(panel, ID_SNAP_BUTTON, _("&Choose"), wxDefaultPosition);
    wxStaticText *textSnap = new wxStaticText(panel, -1, _("Snapshot file:"));
    snapSizer->Add(textSnap, 0, wxALIGN_CENTRE|wxALL, 5);
    snapSizer->Add(snapButton, 0, wxALIGN_CENTRE|wxALL, 5);
    
    wxStaticBox* staticVideoBox = new wxStaticBox(panel, wxID_ANY, _("Choose video registration file"));
    wxBoxSizer *videoSizer = new wxStaticBoxSizer(staticVideoBox, wxVERTICAL);
    boxSizer->Add(videoSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT, 20);
    wxButton *videoButton = new wxButton(panel, ID_VIDEO_BUTTON, _("&Choose"), wxDefaultPosition);
    wxStaticText *textVideo = new wxStaticText(panel, -1, _("Video file:"));
    videoSizer->Add(textVideo, 0, wxALIGN_CENTRE|wxALL, 5);
    videoSizer->Add(videoButton, 0, wxALIGN_CENTRE|wxALL, 5);
    
    return panel;
}

wxPanel* ConfigureDialog::CreateVideoSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);
    wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
         
    panel->SetSizer(boxSizer);
    
    wxBoxSizer *hSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
    
    wxString strAviFormat[2];
    strAviFormat[0] = _("&none (uncompressed)");
    strAviFormat[1] = _("&xvid");
    
    aviFormat = new wxRadioBox(panel, ID_AVI_FORMAT, _("&Choose compression:"),
        wxDefaultPosition, wxSize(220, 80), 2, strAviFormat);    
    hSizer->Add(aviFormat, 0, wxGROW|wxALL, 10);   
    
    staticCompressionLevelBox = new wxStaticBox(panel, wxID_ANY, _("Choose xvid quality:"), wxDefaultPosition, wxSize(220, 80));
    wxSizer *bSizer = new wxStaticBoxSizer(staticCompressionLevelBox, wxVERTICAL);
    hSizer->Add(bSizer, 0, wxGROW|wxALL, 10);
    qualitySlider = new wxSlider(panel, ID_SLIDER, 50, 0, 100, wxDefaultPosition, wxSize(200, 60), wxSL_HORIZONTAL|wxSL_LABELS);
    bSizer->Add(qualitySlider);       
        
    vSizer->Add(hSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT, 10);
        
    /*audio = new wxCheckBox(panel, ID_AUDIO, _("Enable audio recording (xvid only)") );
    vSizer->Add(audio, wxGROW|wxALL, 15);
    
    wxArrayString validValues = fillValidValues(wxString("dsp*", wxConvUTF8));
    textAudioDevice = new wxStaticText(panel, -1, _("Audio device:"));
    comboAudioDevice = new wxComboBox(panel, ID_COMBOBOX_AUDIO_DEVICE, 
    wxT("- - -"), wxDefaultPosition, wxDefaultSize, validValues);
    comboAudioDevice->SetValue(wxString(Setting::GetInstance()->GetAudioDeviceFile().c_str(), wxConvUTF8));
    
    vSizer->Add(textAudioDevice, 0, wxALIGN_CENTER|wxALL, 5);
    vSizer->Add(comboAudioDevice, 0, wxALIGN_CENTER|wxBOTTOM, 15);
    */
    silenceBox = new wxStaticBox(panel, wxID_ANY, _("Add silence for synchronization reason:"), wxDefaultPosition, wxSize(220, 80));
    wxSizer *sSizer = new wxStaticBoxSizer(silenceBox, wxVERTICAL);
    vSizer->Add(sSizer, 0, wxGROW|wxALL, 10);
    hundredthsSlider = new wxSlider(panel, ID_SLIDER, 20, 0, 100, wxDefaultPosition, wxSize(200, 40), wxSL_HORIZONTAL|wxSL_LABELS);
    hundredthsSlider->SetValue(Setting::GetInstance()->GetSilence());    
    sSizer->Add(hundredthsSlider, 0, wxGROW|wxALL, 5);
    textUnit = new wxStaticText(panel, -1, _("(hundredths of a second)"));
    vSizer->Add(textUnit, wxGROW|wxALL, 15);
    
    boxSizer->Add(vSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT, 10);     
      
    if((Setting::GetInstance()->GetCompression() == "none")) {
        aviFormat->SetSelection(0);
        staticCompressionLevelBox->Enable(false);        
        qualitySlider->Enable(false);
        //comboAudioDevice->Enable(false);
        //audio->Enable(false);
        silenceBox->Enable(false);
        textUnit->Enable(false);
        //textAudioDevice->Enable(false);
        hundredthsSlider->Enable(false);
    }
    else
        aviFormat->SetSelection(1);
    qualitySlider->SetValue(Setting::GetInstance()->GetCompressionLevel());
    
    return panel;
}

wxArrayString ConfigureDialog::fillValidValues(const wxString &s)
{    
    wxDir d;
    wxArrayString validValues;
    d.GetAllFiles(wxString("/dev", wxConvUTF8), &validValues, s, wxDIR_FILES);    
    return validValues;
}

void ConfigureDialog::OnSaveImage(wxCommandEvent& WXUNUSED( event ))
{
    std::string supported;
    std::string filename = Setting::GetInstance()->GetSnapshotFile();
    supported = std::string("Tif files (*.tif)|*.tif|Jpeg files (*.jpg)|*.jpg") +
    std::string("|Bmp files (*.bmp)|*.bmp|Png files (*.png)|*.png") +
    std::string("|Pcx files (*.pcx)|*.pcx|Xpm files (*.xpm)|*.xpm");
    wxFileDialog dialog(this,
                        _("Save snapshot"),
                        wxEmptyString,
                        wxString(getFileName(filename).c_str(), wxConvUTF8),
                        wxString(supported.c_str(), wxConvUTF8),
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(1);
    dialog.SetDirectory(wxString(getDir(filename).c_str(), wxConvUTF8));
    
    if (dialog.ShowModal() == wxID_OK) {
        Setting::GetInstance()->SetSnapshotFile((const char*)dialog.GetPath().mb_str(wxConvUTF8));                     
    }
}

void ConfigureDialog::OnSaveVideo(wxCommandEvent& WXUNUSED( event ))
{
    std::string filename = Setting::GetInstance()->GetVideoFile();
    wxFileDialog dialog(this,
                        _("Save video"),
                        wxEmptyString,
                        wxString(getFileName(filename).c_str(), wxConvUTF8),
                        _("AVI files (*.avi)|*.avi"),
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(0);
    dialog.SetDirectory(wxString(getDir(filename).c_str(), wxConvUTF8));

    if (dialog.ShowModal() == wxID_OK) {
        Setting::GetInstance()->SetVideoFile((const char *)dialog.GetPath().mb_str(wxConvUTF8));
    }
}

void ConfigureDialog::OnOk(wxCommandEvent& WXUNUSED( event ))
{
    std::string oldDevice = Setting::GetInstance()->GetDeviceFile();
    std::string oldV4LAPI = Setting::GetInstance()->GetDriverAPI();    
    std::string oldFrameFormat = Setting::GetInstance()->GetFrameFormat();
    Setting::GetInstance()->SetDeviceFile((const char*)combo->GetValue().mb_str(wxConvUTF8));    
    Setting::GetInstance()->SetFrameFormat((const char*)rgbMode->GetStringSelection().mb_str(wxConvUTF8));
    Setting::GetInstance()->SetDriverAPI((const char*)v4lVersion->GetStringSelection().mb_str(wxConvUTF8));
    if(aviFormat->GetStringSelection() == wxString("xvid", wxConvUTF8))
        Setting::GetInstance()->SetCompression((const char*)wxString("xvid", wxConvUTF8).mb_str(wxConvUTF8));
    else
        Setting::GetInstance()->SetCompression((const char*)wxString("none", wxConvUTF8).mb_str(wxConvUTF8));

    Setting::GetInstance()->SetAudioEnabled(true);
    Setting::GetInstance()->SetCompressionLevel(qualitySlider->GetValue());
    Setting::GetInstance()->SetSilence(hundredthsSlider->GetValue());            
    if((oldV4LAPI != (const char*)v4lVersion->GetStringSelection().mb_str(wxConvUTF8))
        || (oldDevice != (const char*)combo->GetValue().mb_str(wxConvUTF8))
        || (oldFrameFormat != (const char*)rgbMode->GetStringSelection().mb_str(wxConvUTF8))) {
       wxMessageBox(_("You have modified device file or frame format or device API settings.\nPlease restart the program for changes to take effect..."), _("Device setting"), wxICON_INFORMATION); 
    }    
    Close();
}

void ConfigureDialog::OnRadioBoxCompression(wxCommandEvent& WXUNUSED(event))
{
    if(aviFormat->GetSelection()) {
        staticCompressionLevelBox->Enable(true);
        qualitySlider->Enable(true);       
        silenceBox->Enable(true);
        textUnit->Enable(true);
        hundredthsSlider->Enable(true);
        
    }
    else {
        staticCompressionLevelBox->Enable(false);
        qualitySlider->Enable(false);       
        silenceBox->Enable(false);        
        textUnit->Enable(false);
        hundredthsSlider->Enable(false);
    }
}

void ConfigureDialog::OnAudioChanged(wxCommandEvent& WXUNUSED(event))
{
    if(audio->IsChecked()) {
        silenceBox->Enable(true);
        textUnit->Enable(true);
        comboAudioDevice->Enable(true);
        textAudioDevice->Enable(true);
        hundredthsSlider->Enable(true);
    }
    else {
        silenceBox->Enable(false);
        textUnit->Enable(false);
        comboAudioDevice->Enable(false);
        textAudioDevice->Enable(false);
        hundredthsSlider->Enable(false);        
    }        
}

std::string ConfigureDialog::getFileName(const std::string& str)
{
    std::string tmp = str;
    std::string::size_type pos = tmp.rfind('/');
    return tmp.erase(0, pos);
}

std::string ConfigureDialog::getDir(const std::string& str)
{
    std::string tmp = str;
    std::string::size_type pos = tmp.rfind('/');
    return tmp.erase(pos);
}

