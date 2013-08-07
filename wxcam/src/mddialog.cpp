/***************************************************************************
 *   Copyright (C) 2007 by Örjan Medhage                                   *
 *   orjan.medhage@gmail.com                                               *
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

#include "mddialog.h"
#include "wxcam.h"
#include <iostream>
#include <wx/button.h>

using namespace std;

BEGIN_EVENT_TABLE(MDAreaDialog, wxDialog)
    EVT_BUTTON(wxID_OK, MDAreaDialog::OnButtonOk)
    EVT_BUTTON(wxID_CANCEL, MDAreaDialog::OnButtonCancel)
    EVT_BUTTON(wxID_HIDE, MDAreaDialog::OnButtonHide)
END_EVENT_TABLE()  

MDAreaDialog::MDAreaDialog()
{
    parent=NULL;
}

MDAreaDialog::MDAreaDialog(wxWindow *parent)
: wxDialog(parent, wxID_ANY, wxString(_("Area Sensitivity Setting")), 
           wxPoint(700, 20), wxSize(380, 530), wxCAPTION)
{

// Initialize status
    this->parent=parent;
    sensitivitiesUpdated = false;
    gettingSensitivities = false;
    dialog_hidden = true;    

// Create wxStaticBox
    areaSensitivityBox = new wxStaticBox(this, 
                                         wxID_ANY, 
                                         _("Colour Sensitivity"));

// Create main Sizer
    areaSensitivityColSizer = new wxStaticBoxSizer(areaSensitivityBox, 
                                                   wxVERTICAL);

// Setup sensitivity input fields

    wxString initRGB;
    initRGB << 0.0;

    redSensText = new wxStaticText(this, wxID_ANY, _("Red: "));
    redSensInputField = new wxTextCtrl(this,
                                       rSTxt,
                                       initRGB,
                                       wxDefaultPosition, 
                                       wxSize(60,30), 
                                       wxTE_PROCESS_ENTER);

    greenSensText = new wxStaticText(this, wxID_ANY, _("Green: "));
    greenSensInputField = new wxTextCtrl(this,
                                         gSTxt,
                                         initRGB,
                                         wxDefaultPosition, 
                                         wxSize(60,30), 
                                         wxTE_PROCESS_ENTER);

    blueSensText = new wxStaticText(this, wxID_ANY, _("Blue: "));
    blueSensInputField = new wxTextCtrl(this,
                                        bSTxt,
                                        initRGB,
                                        wxDefaultPosition, 
                                        wxSize(60,30), 
                                        wxTE_PROCESS_ENTER);

// Setup sensitivity texts

    tableHeadColourText = new wxStaticText(this, wxID_ANY, _("Colour"));
    tableHeadSensitivityText = new wxStaticText(this,
                                                wxID_ANY, 
                                                _("Sensitivity (%)"));
    tableHeadMinText = new wxStaticText(this, wxID_ANY, _("Min"));
    tableHeadMeanText = new wxStaticText(this, wxID_ANY, _("Mean"));
    tableHeadMaxText = new wxStaticText(this, wxID_ANY, _("Max"));

    redMinText = new wxStaticText(this, wxID_ANY, _T(""));
    redMeanText = new wxStaticText(this, wxID_ANY, _T(""));
    redMaxText = new wxStaticText(this, wxID_ANY, _T(""));
    greenMinText = new wxStaticText(this, wxID_ANY, _T(""));
    greenMeanText = new wxStaticText(this, wxID_ANY, _T(""));
    greenMaxText = new wxStaticText(this, wxID_ANY, _T(""));
    blueMinText = new wxStaticText(this, wxID_ANY, _T(""));
    blueMeanText = new wxStaticText(this, wxID_ANY, _T(""));
    blueMaxText = new wxStaticText(this, wxID_ANY, _T(""));

// Setup area sensitivity input fields

    redAreaText = new wxStaticText(this, wxID_ANY, _("Red: "));
    redAreaInputField = new wxTextCtrl(this,
                                       rATxt,
                                       initRGB,
                                       wxDefaultPosition, 
                                       wxSize(60,30), 
                                       wxTE_PROCESS_ENTER);

    greenAreaText = new wxStaticText(this, wxID_ANY, _("Green: "));
    greenAreaInputField = new wxTextCtrl(this,
                                         gATxt,
                                         initRGB,
                                         wxDefaultPosition, 
                                         wxSize(60,30), 
                                         wxTE_PROCESS_ENTER);

    blueAreaText = new wxStaticText(this, wxID_ANY, _("Blue: "));
    blueAreaInputField = new wxTextCtrl(this,
                                        bATxt,
                                        initRGB,
                                        wxDefaultPosition, 
                                        wxSize(60,30), 
                                        wxTE_PROCESS_ENTER);


// Setup area sensitivity texts

    tableHeadAreaColourText = new wxStaticText(this, wxID_ANY, _("Colour"));
    tableHeadAreaText = new wxStaticText(this, wxID_ANY, _("Area (%)"));
    tableHeadAreaMinText = new wxStaticText(this, wxID_ANY, _("Min"));
    tableHeadAreaMeanText = new wxStaticText(this, wxID_ANY, _("Mean"));
    tableHeadAreaMaxText = new wxStaticText(this, wxID_ANY, _("Max"));

    redMinAreaText = new wxStaticText(this, wxID_ANY, _T(""));
    redMeanAreaText = new wxStaticText(this, wxID_ANY, _T(""));
    redMaxAreaText = new wxStaticText(this, wxID_ANY, _T(""));
    greenMinAreaText = new wxStaticText(this, wxID_ANY, _T(""));
    greenMeanAreaText = new wxStaticText(this, wxID_ANY, _T(""));
    greenMaxAreaText = new wxStaticText(this, wxID_ANY, _T(""));
    blueMinAreaText = new wxStaticText(this, wxID_ANY, _T(""));
    blueMeanAreaText = new wxStaticText(this, wxID_ANY, _T(""));
    blueMaxAreaText = new wxStaticText(this, wxID_ANY, _T(""));

// Sensitivity statistics sizer

    areaSensitivityRowSizer = new wxFlexGridSizer(4,5,5,5);
    areaSensitivityRowSizer->SetVGap(10);
    areaSensitivityRowSizer->SetHGap(15);

// Sensitivity statistics - Table head

    areaSensitivityRowSizer->Add(tableHeadColourText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL, 5); 
    areaSensitivityRowSizer->Add(tableHeadSensitivityText,
                                 0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer->Add(tableHeadMinText,
                                 0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer->SetHGap(25);
    areaSensitivityRowSizer->Add(tableHeadMeanText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer->SetHGap(15);
    areaSensitivityRowSizer->Add(tableHeadMaxText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);

// Red - Sensitivity statistics

    areaSensitivityRowSizer->Add(redSensText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer->Add(redSensInputField,
                                 0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer->Add(redMinText,
                                 0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer->SetHGap(25);
    areaSensitivityRowSizer->Add(redMeanText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,11);
    areaSensitivityRowSizer->SetHGap(15);
    areaSensitivityRowSizer->Add(redMaxText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,7);

// Green - Sensitivity statistics

    areaSensitivityRowSizer->Add(greenSensText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer->Add(greenSensInputField,
                                 0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer->Add(greenMinText,
                                 0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer->SetHGap(25);
    areaSensitivityRowSizer->Add(greenMeanText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,11);
    areaSensitivityRowSizer->SetHGap(15);
    areaSensitivityRowSizer->Add(greenMaxText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,7);

// Blue - Sensitivity statistics

    areaSensitivityRowSizer->Add(blueSensText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer->Add(blueSensInputField,
                                 0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer->Add(blueMinText,
                                 0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer->SetHGap(25);
    areaSensitivityRowSizer->Add(blueMeanText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,11);
    areaSensitivityRowSizer->SetHGap(15);
    areaSensitivityRowSizer->Add(blueMaxText,
                                 0, wxLEFT | wxALIGN_CENTRE_VERTICAL,7);
    areaSensitivityRowSizer->SetVGap(20);

// Area statistics Sizer

    areaSensitivityRowSizer2 = new wxFlexGridSizer(4,5,5,5);
    areaSensitivityRowSizer2->SetVGap(10);
    areaSensitivityRowSizer2->SetHGap(15);

// Area statistics - Table head

    areaSensitivityRowSizer2->Add(tableHeadAreaColourText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer2->Add(tableHeadAreaText,
                                  0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer2->Add(tableHeadAreaMinText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,32);
    areaSensitivityRowSizer2->SetHGap(25);
    areaSensitivityRowSizer2->Add(tableHeadAreaMeanText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer2->SetHGap(15);
    areaSensitivityRowSizer2->Add(tableHeadAreaMaxText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);

// Red - Area statistics

    areaSensitivityRowSizer2->Add(redAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer2->Add(redAreaInputField,
                                  0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer2->Add(redMinAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,31);
    areaSensitivityRowSizer2->SetHGap(25);
    areaSensitivityRowSizer2->Add(redMeanAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,11);
    areaSensitivityRowSizer2->SetHGap(15);
    areaSensitivityRowSizer2->Add(redMaxAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,7);

// Green - Area statistics

    areaSensitivityRowSizer2->Add(greenAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer2->Add(greenAreaInputField,
                                  0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer2->Add(greenMinAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,31);
    areaSensitivityRowSizer2->SetHGap(25);
    areaSensitivityRowSizer2->Add(greenMeanAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,11);
    areaSensitivityRowSizer2->SetHGap(15);
    areaSensitivityRowSizer2->Add(greenMaxAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,7);

// Blue - Area statistics

    areaSensitivityRowSizer2->Add(blueAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,5);
    areaSensitivityRowSizer2->Add(blueAreaInputField,
                                  0, wxALIGN_CENTRE_VERTICAL,15);
    areaSensitivityRowSizer2->Add(blueMinAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,32);
    areaSensitivityRowSizer2->SetHGap(25);
    areaSensitivityRowSizer2->Add(blueMeanAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,11);
    areaSensitivityRowSizer2->SetHGap(15);
    areaSensitivityRowSizer2->Add(blueMaxAreaText,
                                  0, wxLEFT | wxALIGN_CENTRE_VERTICAL,7);
    areaSensitivityRowSizer2->SetVGap(20);

// Create button

    cancelButton = new wxButton( this, wxID_CANCEL, _("Reload") );
    okButton = new wxButton( this, wxID_OK, _("Modify") );
    hideButton = new wxButton( this, wxID_HIDE, _("Hide") );

// Create button Sizer

    okCancelButtonSizer = new wxBoxSizer( wxHORIZONTAL );
    okCancelButtonSizer->Add(okButton, 0, wxALL, 10);
    okCancelButtonSizer->Add(cancelButton, 0, wxALL, 10);
    okCancelButtonSizer->Add(hideButton, 0, wxALL, 10);

// Create the main Sizer

    areaSensitivityColSizer->Add(areaSensitivityRowSizer,
                                 0, wxTOP, 45);
    areaSensitivityColSizer->Add(areaSensitivityRowSizer2,
                                 0, wxTOP, 35);
    areaSensitivityColSizer->Add(okCancelButtonSizer,
                                 0, wxTOP | wxALIGN_CENTER, 30);
    SetSizer(areaSensitivityColSizer);

}

MDAreaDialog::~MDAreaDialog()
{
}

void MDAreaDialog::OnButtonOk(wxCommandEvent& WXUNUSED(event))
{
// Set MovementDetection Data of the Selected Area
    if (!(redSensInputField->GetValue()).ToDouble(redStatus)) {
        cout << "Error: Red Semsitivity: Not a valid input!" << endl;
    }
    
    if (!(greenSensInputField->GetValue()).ToDouble(greenStatus)) {
        cout << "Error: Green Semsitivity: Not a valid input!" << endl;
    }
    
    if (!(blueSensInputField->GetValue()).ToDouble(blueStatus)) {
        cout << "Error: Blue Semsitivity: Not a valid input!" << endl;
    }
    
    if (!(redAreaInputField->GetValue()).ToDouble(redAreaStatus)) {
        cout << "Error: Red Area Sensitivity: Not a valid input!" << endl;
    }
    
    if (!(greenAreaInputField->GetValue()).ToDouble(greenAreaStatus)) {
        cout << "Error: Green Area Sensitivity: Not a valid input!" << endl;
    }
    
    if (!(blueAreaInputField->GetValue()).ToDouble(blueAreaStatus)) {
        cout << "Error: Blue Area Sensitivity: Not a valid input!" << endl;
    }
    if ( (redSensInputField->GetValue()).ToDouble(redStatus) &&
            (greenSensInputField->GetValue()).ToDouble(greenStatus) &&
            (blueSensInputField->GetValue()).ToDouble(blueStatus) &&
            (redAreaInputField->GetValue()).ToDouble(redAreaStatus) &&
            (greenAreaInputField->GetValue()).ToDouble(greenAreaStatus) &&
            (blueAreaInputField->GetValue()).ToDouble(blueAreaStatus) ) {
        sensitivitiesUpdated = true;
        gettingSensitivities = false;
    }

}

void MDAreaDialog::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
//     Get MovementDetection Data of the Selected Area
   sensitivitiesUpdated = false;
   gettingSensitivities = true;
}

void MDAreaDialog::OnButtonHide(wxCommandEvent& WXUNUSED(event))
{
    if(parent)
        ((wxCam*)parent)->OnHideMDAreas();
}

bool MDAreaDialog::UpdatedSensitivities()
{
    return sensitivitiesUpdated;
}

bool MDAreaDialog::SendSensitivities()
{
    return gettingSensitivities;
}

bool MDAreaDialog::GetDialogStatus()
{
    return dialog_hidden;
}

void MDAreaDialog::ShowDialog()
{
    if(dialog_hidden) {
        Move(wxPoint(700, 20));
        Show(true);
        dialog_hidden = false;
    }
}

void MDAreaDialog::HideDialog()
{
    if(!dialog_hidden) {
        Show(false);
        dialog_hidden = true;
    }
}

void MDAreaDialog::GetAreaSensitivity(double &red, 
                                      double &green, 
                                      double &blue, 
                                      double &redArea, 
                                      double &greenArea, 
                                      double &blueArea)
{
    red = *redStatus;
    green = *greenStatus;
    blue = *blueStatus;
    redArea = *redAreaStatus;
    greenArea = *greenAreaStatus;
    blueArea = *blueAreaStatus;

    sensitivitiesUpdated = false;
}

void MDAreaDialog::SetAreaSensitivity(double &red, 
                                      double &green, 
                                      double &blue, 
                                      double &redArea, 
                                      double &greenArea, 
                                      double &blueArea)
{
    redStatus = new double(red);
    greenStatus = new double(green);
    blueStatus = new double(blue);
    redAreaStatus = new double(redArea);
    greenAreaStatus = new double(greenArea);
    blueAreaStatus = new double(blueArea);

    wxString redInput(_T(""));
    wxString greenInput(_T(""));
    wxString blueInput(_T(""));
    wxString redAreaInput(_T(""));
    wxString greenAreaInput(_T(""));
    wxString blueAreaInput(_T(""));

    redInput << red;
    greenInput << green;
    blueInput << blue;
    redAreaInput << redArea;
    greenAreaInput << greenArea;
    blueAreaInput << blueArea;

    redSensInputField->SetValue(redInput);
    greenSensInputField->SetValue(greenInput);
    blueSensInputField->SetValue(blueInput);
    redAreaInputField->SetValue(redAreaInput);
    greenAreaInputField->SetValue(greenAreaInput);
    blueAreaInputField->SetValue(blueAreaInput);
    gettingSensitivities = false;
}

void MDAreaDialog::UpdateStatistics(double &redMin, 
                                    double &redMean, 
                                    double &redMax, 
                                    double &greenMin, 
                                    double &greenMean, 
                                    double &greenMax, 
                                    double &blueMin, 
                                    double &blueMean, 
                                    double &blueMax,
                                    double &redMinArea, 
                                    double &redMeanArea, 
                                    double &redMaxArea, 
                                    double &greenMinArea, 
                                    double &greenMeanArea, 
                                    double &greenMaxArea, 
                                    double &blueMinArea, 
                                    double &blueMeanArea, 
                                    double &blueMaxArea)
{
   wxString redDataMin;
   wxString redDataMean;
   wxString redDataMax;
   wxString greenDataMin;
   wxString greenDataMean;
   wxString greenDataMax;
   wxString blueDataMin;
   wxString blueDataMean;
   wxString blueDataMax;

   wxString redDataMinArea;
   wxString redDataMeanArea;
   wxString redDataMaxArea;
   wxString greenDataMinArea;
   wxString greenDataMeanArea;
   wxString greenDataMaxArea;
   wxString blueDataMinArea;
   wxString blueDataMeanArea;
   wxString blueDataMaxArea;

   redMinText->SetLabel(redDataMin.Format(wxT("%.1f"), redMin));
   redMeanText->SetLabel(redDataMean.Format(wxT("%3.1f"), redMean));
   redMaxText->SetLabel(redDataMax.Format(wxT("%3.1f"), redMax));
   greenMinText->SetLabel(greenDataMin.Format(wxT("%3.1f"), greenMin));
   greenMeanText->SetLabel(greenDataMean.Format(wxT("%3.1f"), greenMean));
   greenMaxText->SetLabel(greenDataMax.Format(wxT("%3.1f"), greenMax));
   blueMinText->SetLabel(blueDataMin.Format(wxT("%3.1f"), blueMin));
   blueMeanText->SetLabel(blueDataMean.Format(wxT("%3.1f"), blueMean));
   blueMaxText->SetLabel(blueDataMax.Format(wxT("%3.1f"), blueMax));

   redMinAreaText->SetLabel(redDataMinArea.Format(wxT("%3.1f"), redMinArea));
   redMeanAreaText->SetLabel(redDataMeanArea.Format(wxT("%3.1f"), redMeanArea));
   redMaxAreaText->SetLabel(redDataMaxArea.Format(wxT("%3.1f"), redMaxArea));
   greenMinAreaText->SetLabel(greenDataMinArea.Format(wxT("%3.1f"), greenMinArea));
   greenMeanAreaText->SetLabel(greenDataMeanArea.Format(wxT("%3.1f"), greenMeanArea));
   greenMaxAreaText->SetLabel(greenDataMaxArea.Format(wxT("%3.1f"), greenMaxArea));
   blueMinAreaText->SetLabel(blueDataMinArea.Format(wxT("%3.1f"), blueMinArea));
   blueMeanAreaText->SetLabel(blueDataMeanArea.Format(wxT("%3.1f"), blueMeanArea));
   blueMaxAreaText->SetLabel(blueDataMaxArea.Format(wxT("%3.1f"), blueMaxArea));
}
