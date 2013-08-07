/***************************************************************************
 *   Copyright (C) 2008 by Örjan Medhage                                   *
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

#ifndef _mddialog_h_
#define	_mddialog_h_

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <string>
#include <vector>
#include "wx/numdlg.h"

class MDAreaDialog : public wxDialog
{
    public:

       MDAreaDialog();

       MDAreaDialog(wxWindow *parent=NULL);

       ~MDAreaDialog();    
       
       void OnButtonOk(wxCommandEvent& WXUNUSED(event));

       void OnButtonCancel(wxCommandEvent& WXUNUSED(event));
       
       void OnButtonHide(wxCommandEvent& WXUNUSED(event));

       bool UpdatedSensitivities();

       bool SendSensitivities();
       
       bool GetDialogStatus();

       void ShowDialog();

       void HideDialog();

       void GetAreaSensitivity(double &red, 
                               double &green, 
                               double &blue, 
                               double &redArea, 
                               double &greenArea, 
                               double &blueArea);

       void SetAreaSensitivity(double &red, 
                               double &green, 
                               double &blue, 
                               double &redArea, 
                               double &greenArea, 
                               double &blueArea);

       void UpdateStatistics(double &redMin, 
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
                             double &blueMaxArea);
       
    private:

       wxWindow *parent;
       wxStaticBox* areaSensitivityBox;
       wxSizer* areaSensitivityColSizer;
       wxFlexGridSizer* areaSensitivityRowSizer;
       wxFlexGridSizer* areaSensitivityRowSizer2;

       wxStaticText* redSensText;
       wxStaticText* greenSensText;
       wxStaticText* blueSensText;
       wxTextCtrl* redSensInputField;
       wxTextCtrl* greenSensInputField;
       wxTextCtrl* blueSensInputField;

       wxStaticText* redAreaText;
       wxStaticText* greenAreaText;
       wxStaticText* blueAreaText;
       wxTextCtrl* redAreaInputField;
       wxTextCtrl* greenAreaInputField;
       wxTextCtrl* blueAreaInputField;

       wxBoxSizer *buttonSizer;
       wxButton *cancelButton;
       wxButton *okButton;
       wxButton *hideButton;
       wxBoxSizer *okCancelButtonSizer;

       wxStaticText *tableHeadColourText, *tableHeadSensitivityText, 
                    *tableHeadMinText, *tableHeadMeanText, *tableHeadMaxText,
                    *tableHeadAreaColourText, *tableHeadAreaText,
                    *tableHeadAreaMinText, *tableHeadAreaMeanText, 
                    *tableHeadAreaMaxText;
       wxStaticText *redMinText, *redMeanText, *redMaxText;
       wxStaticText *greenMinText, *greenMeanText, *greenMaxText;
       wxStaticText *blueMinText, *blueMeanText, *blueMaxText;

       wxStaticText *redMinAreaText, *redMeanAreaText, *redMaxAreaText;
       wxStaticText *greenMinAreaText, *greenMeanAreaText, *greenMaxAreaText;
       wxStaticText *blueMinAreaText, *blueMeanAreaText, *blueMaxAreaText;

       double *redStatus, *greenStatus, *blueStatus; 
       double *redAreaStatus, *greenAreaStatus, *blueAreaStatus;

       bool sensitivitiesUpdated;
       bool gettingSensitivities;
       bool dialog_hidden;

       enum
       {
          rSTxt,
          gSTxt,
          bSTxt,
          rSInput,
          gSInput,
          bSInput,
          rATxt,
          gATxt,
          bATxt,
          rAInput,
          gAInput,
          bAInput,
          wxID_HIDE
       };

       DECLARE_EVENT_TABLE()

};

#endif	/* _mddialog_h_ */


