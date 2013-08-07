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

#ifndef _motion_h_
#define	_motion_h_

#include <wx/wx.h>
#include <string>
#include <vector>
#include "mddialog.h"

struct AreaStruct
{
    int w_lower;
    int h_lower;
    int w_upper;
    int h_upper;
    bool selected;
    double redMaxArea;
    double greenMaxArea;
    double blueMaxArea;
    double redSensitivity;
    double greenSensitivity;
    double blueSensitivity;
    unsigned char* data_lp;    
};

class MovementDetector 
{        
    private:        
       std::vector<AreaStruct> areaList;
       std::vector<AreaStruct>::iterator area;
        
       enum md_status
       {
          REC_STARTING,
          REC_CONTINUING,
          REC_STOPPING,
          REC_NONE,
          REC_PAUSE
       };
       
       unsigned int imgWidth, imgHeight; 

       md_status status;

       bool initialized;
       bool movementDetectionActivated;
       long redArea;
       long greenArea;
       long blueArea;
       long recordPauseTimerDelay;
       long sampleRGBTimerInterval;
       int gWidth;
       int gHeight;
       bool paused;
       bool recording;
       bool verboseMode;
       bool slowMovementDetection;
       long stopRecordingTimer;
       long sampleRGBTimer;
       double maxRed;
       double minRed;
       double meanRed;
       long nrOfRedSamples;
       long nrOfRedAreaSamples;
       double maxGreen;
       double minGreen;
       double meanGreen;
       long nrOfGreenSamples;
       long nrOfGreenAreaSamples;
       double maxBlue;
       double minBlue;
       double meanBlue;
       long nrOfBlueSamples;
       long nrOfBlueAreaSamples;
       double maxRedArea;
       double minRedArea;
       double maxGreenArea;
       double minGreenArea;
       double maxBlueArea;
       double minBlueArea;
       double meanRedArea;
       double meanGreenArea;
       double meanBlueArea;
       bool record;
       int areaOffsetX;
       int areaOffsetY;
       double areaScaleX;
       double areaScaleY;
       bool showAreasInitialized;
       bool createNewArea;
       wxColour lightGrey;
       wxColour green;
       wxWindow* parentWindow;
       MDAreaDialog* selectedAreaDialog;
       bool previouswithdialog;

    public:

       MovementDetector();

       ~MovementDetector(); 

       void SetImageSize(wxImage& image);

       void ActivateMovementDetection();

       void DeactivateMovementDetection();

       void ActivateSlowMovementDetection();

       void DeactivateSlowMovementDetection();

       void ActivateVerboseMode();

       void DeactivateVerboseMode();

       bool MovementCheck(wxImage& wximg, 
                          std::vector<AreaStruct>::iterator currentArea, 
                          int status);

       void MovementDetection(wxImage& image, int& status);

       void ShowDialog();

       void HideDialog();
       
       bool GetDialogStatus();

       void SelectedAreaMovementCheck(wxImage& image);

       void SetSensitivitySampleInterval(int sampleInterval);

       void GetStatisticsStdOutput();

       void AddArea(int w_lower, int h_lower, int w_upper, int h_upper);

       void RemoveSelectedArea();

       void UpdateDialogSensitivities();

       void RemoveAllAreas();

       void SelectNextArea();

       void SelectPreviousArea();

       void UnselectAllAreas();

       void UnselectSelectedArea();

       void SetAreaSensitivity(double percentageRedSensitivity,
                               double percentageGreenSensitivity,
                               double percentageBlueSensitivity,
                               double percentageRedArea,
                               double percentageGreenArea,
                               double percentageBlueArea);

       void GetAreaSensitivity(double& percentageRedSensitivity,
                               double& percentageGreenSensitivity,
                               double& percentageBlueSensitivity,
                               double& percentageRedArea,
                               double& percentageGreenArea,
                               double& percentageBlueArea);


       void SetRedSensitivity(double percentageSensitivity);

       void SetGreenSensitivity(double percentageSensitivity);

       void SetBlueSensitivity(double percentageSensitivity);

       void SetRedMaxArea(double percentageArea);

       void SetGreenMaxArea(double percentageArea);

       void SetBlueMaxArea(double percentageArea);

       double GetRedSensitivity();

       double GetGreenSensitivity();

       double GetBlueSensitivity();

       double GetRedMaxArea();

       double GetGreenMaxArea();

       double GetBlueMaxArea();

       double max(double cur_max, double value);

       double min(double cur_min, double value);

       double mean(long &nrOfSamples, double cur_mean, double value);

       void DrawAreas(bool showMDAreasStatus, 
                      wxDC& dc, 
                      int offsetx, 
                      int offsety, 
                      double scalex = 1.0, 
                      double scaley = 1.0, 
                      bool withdialog = true);

       void MouseEvent(wxMouseEvent& event);

       void KeyDown(wxKeyEvent& event);

       void SetParentWindow(wxWindow* windowPtr);

};


#endif	/* _motion_h_ */

