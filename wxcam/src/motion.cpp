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

#include "motion.h"

using namespace std;

MovementDetector::MovementDetector() 
{
    initialized = false;
    movementDetectionActivated = false;
    verboseMode = false;
    createNewArea = false;
    showAreasInitialized = false;
    slowMovementDetection = true;
    
    recordPauseTimerDelay = 16;
    stopRecordingTimer = recordPauseTimerDelay;
    sampleRGBTimerInterval = 48;
    sampleRGBTimer = 0;
    
    recording = true;
    paused = true;
    
    minRedArea = 100.0;
    minGreenArea = 100.0;
    minBlueArea = 100.0;
    maxRedArea = 0.0;
    maxGreenArea = 0.0;
    maxBlueArea = 0.0;
    meanRedArea = 0.0;
    meanGreenArea = 0.0;
    meanBlueArea = 0.0;
    minRed = 100.0;
    minGreen = 100.0;
    minBlue = 100.0;
    maxRed = 0.0;
    maxGreen = 0.0;
    maxBlue = 0.0;
    meanRed = 0.0;
    meanGreen = 0.0;
    meanBlue = 0.0;
    nrOfRedSamples = 0;
    nrOfGreenSamples = 0;
    nrOfBlueSamples = 0;
    nrOfRedAreaSamples = 0;
    nrOfGreenAreaSamples = 0;
    nrOfBlueAreaSamples = 0;
    lightGrey.Set(200, 200, 200);
    green.Set(0, 250, 0);
    
    selectedAreaDialog = NULL;
    parentWindow = NULL;
    
}

MovementDetector::~MovementDetector() 
{
    if(selectedAreaDialog)
        selectedAreaDialog->Destroy();
}

void MovementDetector::SetImageSize(wxImage& image) 
{
    
    if (!initialized) {
        imgWidth = image.GetWidth();
        imgHeight = image.GetHeight();
        
        if (verboseMode) {
            cout << "Width: " << imgWidth << "\tHeight: " << imgHeight << endl;
        }
        
    }
    initialized = true;
    
    
    if ((image.GetWidth() != imgWidth) || (image.GetHeight() != imgHeight)) {
//    Recalculate areas
        
        double widthscale = image.GetWidth() / (imgWidth * 1.0);
        double heightscale = image.GetHeight() / (imgHeight * 1.0);
        
        for ( std::vector<AreaStruct>::iterator areaRect=areaList.begin();
        areaRect!=areaList.end();
        ++areaRect ) {
            areaRect->w_lower = (int)(areaRect->w_lower * widthscale);
            areaRect->w_upper = (int)(areaRect->w_upper * widthscale);
            areaRect->h_lower = (int)(areaRect->h_lower * heightscale);
            areaRect->h_upper = (int)(areaRect->h_upper * heightscale);
        }
        
        imgWidth = image.GetWidth();
        imgHeight = image.GetHeight();
    }
    
}

void MovementDetector::ActivateMovementDetection() 
{
    movementDetectionActivated = true;
}

void MovementDetector::DeactivateMovementDetection() 
{
    movementDetectionActivated = false;
}

void MovementDetector::ActivateSlowMovementDetection() 
{
    slowMovementDetection = true;
}

void MovementDetector::DeactivateSlowMovementDetection() 
{
    slowMovementDetection = false;
}

void MovementDetector::ActivateVerboseMode() 
{
    verboseMode = true;
}

void MovementDetector::DeactivateVerboseMode() 
{
    verboseMode = false;
}


bool MovementDetector::MovementCheck(wxImage& wximg, std::vector<AreaStruct>::iterator currentArea, int status) 
{
    bool activateRecording;
    
    activateRecording = false;
    if (currentArea->selected && (!createNewArea) || (status != REC_NONE && status != REC_STOPPING) ) {
        redArea = 0;
        greenArea = 0;
        blueArea = 0;
        
        gWidth = currentArea->w_upper - currentArea->w_lower;
        gHeight = currentArea->h_upper - currentArea->h_lower;
        
        long iDataElement;
        long iPosition;
        
        iDataElement = -1;
        
        
        for (int iHeight = currentArea->h_lower ; iHeight < currentArea->h_upper ; iHeight++) {
            for (int iWidth = currentArea->w_lower ; iWidth < currentArea->w_upper ; iWidth++) {
                iDataElement++;
                iPosition = iHeight * imgWidth + iWidth;
                if ( ((abs(*(wximg.GetData()+3*iPosition) - *(currentArea->data_lp+3*iDataElement))/2.55) >
                        (100.0 - currentArea->redSensitivity)) ) {
                    redArea++;
                }
                if ( ((abs(*(wximg.GetData()+3*iPosition+1) - *(currentArea->data_lp+3*iDataElement+1))/2.55) >
                        (100.0 - currentArea->greenSensitivity)) ) {
                    greenArea++;
                }
                if ( ((abs(*(wximg.GetData()+3*iPosition+2) - *(currentArea->data_lp+3*iDataElement+2))/2.55) >
                        (100.0 - currentArea->blueSensitivity)) ) {
                    blueArea++;
                }
                
                if (currentArea->selected && (status == REC_NONE || status == REC_STOPPING)) {
                    maxRed = max(maxRed, (abs(*(wximg.GetData()+3*iPosition) - *(currentArea->data_lp+3*iDataElement))/2.55));
                    minRed = min(minRed, (abs(*(wximg.GetData()+3*iPosition) - *(currentArea->data_lp+3*iDataElement))/2.55));
                    meanRed = mean(nrOfRedSamples, meanRed,
                            (abs(*(wximg.GetData()+3*iPosition) - *(currentArea->data_lp+3*iDataElement))/2.55));
                    maxGreen = max(maxGreen,
                            (abs(*(wximg.GetData()+3*iPosition+1) - *(currentArea->data_lp+3*iDataElement+1))/2.55));
                    minGreen = min(minGreen,
                            (abs(*(wximg.GetData()+3*iPosition+1) - *(currentArea->data_lp+3*iDataElement+1))/2.55));
                    meanGreen = mean(nrOfGreenSamples, meanGreen,
                            (abs(*(wximg.GetData()+3*iPosition+1) - *(currentArea->data_lp+3*iDataElement+1))/2.55));
                    maxBlue = max(maxBlue, (abs(*(wximg.GetData()+3*iPosition+2) - *(currentArea->data_lp+3*iDataElement+2))/2.55));
                    minBlue = min(minBlue, (abs(*(wximg.GetData()+3*iPosition+2) - *(currentArea->data_lp+3*iDataElement+2))/2.55));
                    meanBlue = mean(nrOfBlueSamples, meanBlue,
                            (abs(*(wximg.GetData()+3*iPosition+2) - *(currentArea->data_lp+3*iDataElement+2))/2.55));
                }
                
                if ((status == REC_CONTINUING) && slowMovementDetection) {
                    *(currentArea->data_lp+3*iDataElement) = *(wximg.GetData()+3*iPosition);
                    *(currentArea->data_lp+3*iDataElement+1) = *(wximg.GetData()+3*iPosition+1);
                    *(currentArea->data_lp+3*iDataElement+2) = *(wximg.GetData()+3*iPosition+2);
                }
                
            } // for width
        }  // for height
        
        
        if (currentArea->selected && (status == REC_NONE || status == REC_STOPPING)) {
            if (showAreasInitialized && selectedAreaDialog) {
                if (selectedAreaDialog->UpdatedSensitivities()) {
                    cout << "Getting Sensitivities" << endl;
                    selectedAreaDialog->GetAreaSensitivity(currentArea->redSensitivity,
                            currentArea->greenSensitivity,
                            currentArea->blueSensitivity,
                            currentArea->redMaxArea,
                            currentArea->greenMaxArea,
                            currentArea->blueMaxArea);
                    cout << "Sensitivities read from Dialog" << endl;
                }
                if (selectedAreaDialog->SendSensitivities()) {
                    selectedAreaDialog->SetAreaSensitivity(currentArea->redSensitivity,
                            currentArea->greenSensitivity,
                            currentArea->blueSensitivity,
                            currentArea->redMaxArea,
                            currentArea->greenMaxArea,
                            currentArea->blueMaxArea);
                }
            }
            if (sampleRGBTimer < sampleRGBTimerInterval) {
                maxRedArea = max(maxRedArea, (redArea * 100.0 / (gWidth*gHeight)));
                maxGreenArea = max(maxGreenArea, (greenArea * 100.0 / (gWidth*gHeight)));
                maxBlueArea = max(maxBlueArea, (blueArea * 100.0 / (gWidth*gHeight)));
                minRedArea = min(minRedArea, (redArea * 100.0 / (gWidth*gHeight)));
                minGreenArea = min(minGreenArea, (greenArea * 100.0 / (gWidth*gHeight)));
                minBlueArea = min(minBlueArea, (blueArea * 100.0 / (gWidth*gHeight)));
                meanRedArea = mean(nrOfRedAreaSamples, meanRedArea, (redArea * 100.0 / (gWidth*gHeight)));
                meanGreenArea = mean(nrOfGreenAreaSamples, meanGreenArea, (greenArea * 100.0 / (gWidth*gHeight)));
                meanBlueArea = mean(nrOfBlueAreaSamples, meanBlueArea, (blueArea * 100.0 / (gWidth*gHeight)));
                sampleRGBTimer++;
            }
            else {
                if (selectedAreaDialog) {
                    selectedAreaDialog->UpdateStatistics(minRed,
                            meanRed,
                            maxRed,
                            minGreen,
                            meanGreen,
                            maxGreen,
                            minBlue,
                            meanBlue,
                            maxBlue,
                            minRedArea,
                            meanRedArea,
                            maxRedArea,
                            minGreenArea,
                            meanGreenArea,
                            maxGreenArea,
                            minBlueArea,
                            meanBlueArea,
                            maxBlueArea);
                }
                
                if (verboseMode) {
                    GetStatisticsStdOutput();
                }
                sampleRGBTimer = 0;
                minRedArea = 100.0;
                minGreenArea = 100.0;
                minBlueArea = 100.0;
                maxRedArea = 0.0;
                maxGreenArea = 0.0;
                maxBlueArea = 0.0;
                meanRedArea = 0.0;
                meanGreenArea = 0.0;
                meanBlueArea = 0.0;
                minRed = 100.0;
                minGreen = 100.0;
                minBlue = 100.0;
                maxRed = 0.0;
                maxGreen = 0.0;
                maxBlue = 0.0;
                meanRed = 0.0;
                meanGreen = 0.0;
                meanBlue = 0.0;
                nrOfRedSamples = 0;
                nrOfGreenSamples = 0;
                nrOfBlueSamples = 0;
                nrOfRedAreaSamples = 0;
                nrOfGreenAreaSamples = 0;
                nrOfBlueAreaSamples = 0;
            }
        }
        else if (currentArea->selected) {
            sampleRGBTimer = sampleRGBTimerInterval;
        }
        
        if ( (status != REC_NONE && status != REC_STOPPING) &&
                (((redArea * 100.0 / (gWidth*gHeight)) > currentArea->redMaxArea) ||
                ((greenArea * 100.0 / (gWidth*gHeight)) > currentArea->greenMaxArea) ||
                ((blueArea * 100.0 / (gWidth*gHeight)) > currentArea->blueMaxArea)) ) {
            activateRecording = true;
        }
        
    }
    
    return activateRecording;
    
}

void MovementDetector::MovementDetection(wxImage& image, int& status) 
{  
    if(status != REC_NONE || showAreasInitialized) {
        if (movementDetectionActivated || showAreasInitialized) {
            imgWidth = image.GetWidth();
            imgHeight = image.GetHeight();
            
            if (showAreasInitialized && !movementDetectionActivated) {
                SelectedAreaMovementCheck(image);
            }
            else {
                record = false;
                if (areaList.begin() == areaList.end()) {
                    cout << "No area available. Creating default area equal to the image size." << endl;
                    AddArea(0, 0, imgWidth, imgHeight);
                }
                
                int counter;
                counter = 0;
                for ( std::vector<AreaStruct>::iterator areaRect=areaList.begin(); areaRect!=areaList.end(); ++areaRect ) {
                    counter++;
                    record = record || MovementCheck(image, areaRect, status);
                }
                
                if (record) {
                    if (recording && verboseMode) {
                        cout << "Recording! " << endl;
                        recording = false;
                        paused = true;
                    }
                    status = REC_CONTINUING;
                    stopRecordingTimer = recordPauseTimerDelay;
                } else if (status == REC_CONTINUING && stopRecordingTimer > 0) {
                    stopRecordingTimer--;
                } else if (status != REC_STOPPING || status != REC_NONE) {
                    if (paused && verboseMode) {
                        cout << "Paused! " << endl;
                        paused = false;
                        recording = true;
                    }
                    status = REC_PAUSE;
                }
            }
        }
    }
}

void MovementDetector::HideDialog() 
{
    if (selectedAreaDialog) {
        selectedAreaDialog->HideDialog();
    }
}

void MovementDetector::ShowDialog() 
{
    if (selectedAreaDialog) {
        selectedAreaDialog->ShowDialog();
        UpdateDialogSensitivities();
    }
}

bool MovementDetector::GetDialogStatus()
{
    if (selectedAreaDialog) {
        return !selectedAreaDialog->GetDialogStatus();
    }
    return false;
}

void MovementDetector::SelectedAreaMovementCheck(wxImage& image) 
{
    if (!movementDetectionActivated) {
        imgWidth = image.GetWidth();
        imgHeight = image.GetHeight();
        if (area != areaList.end()) {
            int dummystatus = REC_NONE;
            
            MovementCheck(image, area, dummystatus);
            if (verboseMode) {
                if ((dummystatus == REC_CONTINUING)&&recording) {
                    recording = false;
                    paused = true;
                    cout << "Sensitivity settings of this area would trigg recording.";
                }
                else if ((dummystatus == REC_PAUSE)&&paused) {
                    recording = true;
                    paused = false;
                    cout << "Sensitivity settings of this area would set the cam to pause.";
                }
            }
        }
    }
}

void MovementDetector::SetSensitivitySampleInterval(int sampleInterval) 
{
    sampleRGBTimerInterval = sampleInterval;
}

void MovementDetector::GetStatisticsStdOutput() 
{
    cout << endl
            << "maxRed: " << maxRed
            << "\tmaxGreen: " << maxGreen
            << "\tmaxBlue: " << maxBlue << endl;
    
    cout << "minRed: " << minRed
            << "\tminGreen: " << minGreen
            << "\tminBlue: " << minBlue << endl;
    
    cout << "meanRed: " << meanRed
            << "\tmeanGreen: " << meanGreen
            << "\tmeanBlue: " << meanBlue << endl;
    
    cout << "nrOfRedSamples: " << nrOfRedSamples
            << "\tnrOfGreenSamples: " << nrOfGreenSamples
            << "\tnrOfBlueSamples: " << nrOfBlueSamples << endl;
    
    cout << "maxRedArea: " << maxRedArea
            << "\tmaxGreenArea: " << maxGreenArea
            << "\tmaxBlueArea: " << maxBlueArea << endl;
    
    cout << "minRedArea: " << minRedArea
            << "\tminGreenArea: " << minGreenArea
            << "\tminBlueArea: " << minBlueArea << endl;
    
    cout << "meanRedArea: " << meanRedArea
            << "\tmeanGreenArea: " << meanGreenArea
            << "\tmeanBlueArea: " << meanBlueArea << endl;
}

void MovementDetector::AddArea(int w_lower, int h_lower, int w_upper, int h_upper) 
{
    createNewArea = true;
    AreaStruct areaStruct;
    
    areaStruct.w_lower = w_lower;
    areaStruct.w_upper = w_upper;
    areaStruct.h_lower = h_lower;
    areaStruct.h_upper = h_upper;
    areaStruct.selected = true;
    
    areaStruct.redSensitivity = 80.0;
    areaStruct.greenSensitivity = 80.0;
    areaStruct.blueSensitivity = 80.0;
    areaStruct.redMaxArea = 0.5;
    areaStruct.greenMaxArea = 0.5;
    areaStruct.blueMaxArea = 0.5;
    
    areaStruct.data_lp = new unsigned char[(3 * (w_upper - w_lower) *
            (h_upper - h_lower))];
    
    if (verboseMode) {
        cout << "Creating area of size: " << (w_upper - w_lower) << "x" << (h_upper - h_lower) << endl;
        cout << "Width spans from: " << w_lower << " to " << w_upper << endl;
        cout << "Height spans from: " << h_lower << " to " << h_upper << endl;
    }
    
    for (int iDataInitialization = 0;
    iDataInitialization < ((w_upper - w_lower) * (h_upper - h_lower));
    iDataInitialization++) {
        *(areaStruct.data_lp + (3*iDataInitialization)) = 0;      // red init
        *(areaStruct.data_lp + (3*iDataInitialization + 1)) = 0;  // green init
        *(areaStruct.data_lp + (3*iDataInitialization + 2)) = 0;  // blue init
    }
    
    if (verboseMode) {
        cout << "Area initialized." << endl;
    }
    
    areaList.push_back( areaStruct );
    
    area = areaList.end() - 1;
    createNewArea = false;
    UpdateDialogSensitivities();
    
}

void MovementDetector::RemoveSelectedArea() 
{
    if (area != areaList.end()) {
        delete[] area->data_lp;
        area->data_lp = NULL;
        if ((area + 1) == areaList.end()) {
            areaList.erase(area);
            area = areaList.begin();
        }
        else {
            areaList.erase((area));
        }
        
        if (area != areaList.end()) {
            area->selected = true;
        }
        UpdateDialogSensitivities();
    }
}

void MovementDetector::UpdateDialogSensitivities() 
{
    if (area != areaList.end()) {
        if (selectedAreaDialog) {
            selectedAreaDialog->SetAreaSensitivity(area->redSensitivity,
                    area->greenSensitivity,
                    area->blueSensitivity,
                    area->redMaxArea,
                    area->greenMaxArea,
                    area->blueMaxArea);
        }
    }
}

void MovementDetector::RemoveAllAreas() 
{
    for ( std::vector<AreaStruct>::iterator areaRect=areaList.begin();
    areaRect!=areaList.end();
    ++areaRect ) {
        areaList.erase(area);
    }
}

void MovementDetector::SelectNextArea() 
{
    if (area != areaList.end()) {
        area->selected = false;
        if ((area + 1) == areaList.end()) {
            area=areaList.begin();
        }
        else {
            area++;
        }
        
        if (area != areaList.end()) {
            area->selected = true;
        }
        
        UpdateDialogSensitivities();
    }
}

void MovementDetector::SelectPreviousArea() 
{
    if (area != areaList.end()) {
        area->selected = false;
        if (area == areaList.begin()) {
            area=areaList.end() - 1;
        }
        else {
            area--;
        }
        
        if (area != areaList.end()) {
            area->selected = true;
        }
        
        UpdateDialogSensitivities();
    }
}

void MovementDetector::UnselectAllAreas() 
{
    for ( std::vector<AreaStruct>::iterator areaRect=areaList.begin();
    areaRect!=areaList.end();
    ++areaRect ) {
        areaRect->selected = false;
    }
    
}

void MovementDetector::UnselectSelectedArea() 
{
    if (area != areaList.end()) {
        area->selected = false;
    }
}

void MovementDetector::SetAreaSensitivity(double percentageRedSensitivity,
        double percentageGreenSensitivity,
        double percentageBlueSensitivity,
        double percentageRedArea,
        double percentageGreenArea,
        double percentageBlueArea) 
{    
    if (area != areaList.end()) {
        //Set Colour sensitivity
        area->redSensitivity = percentageRedSensitivity;
        area->greenSensitivity = percentageGreenSensitivity;
        area->blueSensitivity = percentageBlueSensitivity;
        //Set Area sensitivity
        area->redMaxArea = percentageRedArea;
        area->greenMaxArea = percentageGreenArea;
        area->blueMaxArea = percentageBlueArea;
    }
    
    UpdateDialogSensitivities();
    
}

void MovementDetector::GetAreaSensitivity(double& percentageRedSensitivity,
        double& percentageGreenSensitivity,
        double& percentageBlueSensitivity,
        double& percentageRedArea,
        double& percentageGreenArea,
        double& percentageBlueArea) 
{    
    if (area != areaList.end()) {
        //Set Colour sensitivity
        percentageRedSensitivity = area->redSensitivity;
        percentageGreenSensitivity = area->greenSensitivity;
        percentageBlueSensitivity = area->blueSensitivity;
        //Set Area sensitivity
        percentageRedArea = area->redMaxArea;
        percentageGreenArea = area->greenMaxArea;
        percentageBlueArea = area->blueMaxArea;
    }
    else {
        //Reset Colour sensitivity
        percentageRedSensitivity = 0.0;
        percentageGreenSensitivity = 0.0;
        percentageBlueSensitivity = 0.0;
        //Reset Area sensitivity
        percentageRedArea = 0.0;
        percentageGreenArea = 0.0;
        percentageBlueArea = 0.0;
    }
    
}

void MovementDetector::SetRedSensitivity(double percentageSensitivity) 
{
    if (area != areaList.end()) {
        area->redSensitivity = percentageSensitivity;
    }
    
    UpdateDialogSensitivities();
}

void MovementDetector::SetGreenSensitivity(double percentageSensitivity) 
{
    if (area != areaList.end()) {
        area->greenSensitivity = percentageSensitivity;
    }
    
    UpdateDialogSensitivities();
}

void MovementDetector::SetBlueSensitivity(double percentageSensitivity) 
{
    if (area != areaList.end()) {
        area->blueSensitivity = percentageSensitivity;
    }
    
    UpdateDialogSensitivities();
}

void MovementDetector::SetRedMaxArea(double percentageArea) 
{
    if (area != areaList.end()) {
        area->redMaxArea = percentageArea;
    }
    
    UpdateDialogSensitivities();
}

void MovementDetector::SetGreenMaxArea(double percentageArea) {
    if (area != areaList.end()) {
        area->greenMaxArea = percentageArea;
    }
    
    UpdateDialogSensitivities();
}

void MovementDetector::SetBlueMaxArea(double percentageArea) 
{
    if (area != areaList.end()) {
        area->blueMaxArea = percentageArea;
    }
    
    UpdateDialogSensitivities();
}

double MovementDetector::GetRedSensitivity() 
{
    if (area != areaList.end()) {
        return area->redSensitivity;
    }
    else {
        return 0.0;
    }
}

double MovementDetector::GetGreenSensitivity() 
{
    if (area != areaList.end()) {
        return area->greenSensitivity;
    }
    else {
        return 0.0;
    }
}

double MovementDetector::GetBlueSensitivity() 
{
    if (area != areaList.end()) {
        return area->blueSensitivity;
    }
    else {
        return 0.0;
    }
}

double MovementDetector::GetRedMaxArea() 
{
    if (area != areaList.end()) {
        return area->redMaxArea;
    }
    else {
        return 0.0;
    }
}

double MovementDetector::GetGreenMaxArea() {
    if (area != areaList.end()) {
        return area->greenMaxArea;
    }
    else {
        return 0.0;
    }
}

double MovementDetector::GetBlueMaxArea() 
{
    if (area != areaList.end()) {
        return area->blueMaxArea;
    }
    else {
        return 0.0;
    }
}

double MovementDetector::max(double cur_max, double value) {
    if (value > cur_max) return value;
    else return cur_max;
}

double MovementDetector::min(double cur_min, double value) 
{
    if (value < cur_min) return value;
    else return cur_min;
}

double MovementDetector::mean(long &nrOfSamples, double cur_mean, double value) 
{
    cur_mean = (cur_mean * nrOfSamples + value) / (nrOfSamples + 1);
    nrOfSamples++;
    return cur_mean;
}

void MovementDetector::DrawAreas(bool showMDAreasStatus,
        wxDC& dc,
        int offsetx,
        int offsety,
        double scalex,
        double scaley,
        bool withdialog)
{
    areaOffsetX = offsetx;
    areaOffsetY = offsety;
    areaScaleX = scalex;
    areaScaleY = scaley;
    
    if (showMDAreasStatus) {
        if (withdialog && (!previouswithdialog)) {
            ShowDialog();
        }
        else if ((!withdialog) && previouswithdialog) {
            HideDialog();
        }
        previouswithdialog = withdialog;
        
        int offx, endx, offy, endy;
        wxPoint rectangle[5];
        for ( std::vector<AreaStruct>::iterator areaRect=areaList.begin();
        areaRect!=areaList.end();
        ++areaRect ) {
            if (areaRect->selected) {
                dc.SetPen( wxPen( green, 5, wxSOLID ) );
            }
            else {
                dc.SetPen( wxPen( lightGrey, 5, wxSOLID ) );
            }
            offx = (int)(areaRect->w_lower * scalex) + offsetx;
            endx = (int)(areaRect->w_upper * scalex) + offsetx;
            offy = (int)(areaRect->h_lower * scaley) + offsety;
            endy = (int)(areaRect->h_upper * scaley) + offsety;
            rectangle[0] = wxPoint(offx, offy);
            rectangle[1] = wxPoint(endx, offy);
            rectangle[2] = wxPoint(endx, endy);
            rectangle[3] = wxPoint(offx, endy);
            rectangle[4] = wxPoint(offx, offy);
            dc.DrawLines( 5, rectangle);
        }
        
        if ((!showAreasInitialized) && withdialog) {
            if(!selectedAreaDialog && parentWindow) {
                selectedAreaDialog = new MDAreaDialog( parentWindow );
                ShowDialog();
            }
            else {
                selectedAreaDialog->ShowDialog();
            }
        }
    }
    else if (showAreasInitialized) {
        if(selectedAreaDialog)
            selectedAreaDialog->HideDialog();
        previouswithdialog = false;
    }
    showAreasInitialized = showMDAreasStatus;
}

void MovementDetector::MouseEvent(wxMouseEvent& event) 
{    
    if (showAreasInitialized) {
        if (event.LeftDown() && (!createNewArea)) {
            UnselectSelectedArea();
            int mousePosX;
            int mousePosY;
            
            mousePosX = (int)((event.GetPosition().x - areaOffsetX) / areaScaleX);
            if (mousePosX < 0) mousePosX = 0;
            else if (mousePosX > imgWidth) mousePosX = imgWidth;
            
            mousePosY = (int)((event.GetPosition().y - areaOffsetY) / areaScaleY);
            if (mousePosY < 0) mousePosY = 0;
            else if (mousePosY > imgHeight) mousePosY = imgHeight;
            
            AreaStruct areaStruct;
            
            areaStruct.w_lower = mousePosX;
            areaStruct.w_upper = mousePosX;
            areaStruct.h_lower = mousePosY;
            areaStruct.h_upper = mousePosY;
            areaStruct.selected = true;
            
            areaStruct.redSensitivity = 80.0;
            areaStruct.greenSensitivity = 80.0;
            areaStruct.blueSensitivity = 80.0;
            areaStruct.redMaxArea = 0.5;
            areaStruct.greenMaxArea = 0.5;
            areaStruct.blueMaxArea = 0.5;
            
            areaList.push_back( areaStruct );
            area = areaList.end() - 1;
            createNewArea = true;
            
        } // event.LeftDown
        else if (event.Dragging() && createNewArea) {
            int mousePosX;
            int mousePosY;
            
            mousePosX = (int)((event.GetPosition().x - areaOffsetX) / areaScaleX);
            if (mousePosX < 0) mousePosX = 0;
            else if (mousePosX > imgWidth) mousePosX = imgWidth;
            
            mousePosY = (int)((event.GetPosition().y - areaOffsetY) / areaScaleY);
            if (mousePosY < 0) mousePosY = 0;
            else if (mousePosY > imgHeight) mousePosY = imgHeight;
            
            if (mousePosX > area->w_lower) {
                area->w_upper = mousePosX;
            }
            else if (mousePosX < area->w_lower) {
                area->w_lower = mousePosX;
            }
            
            if (mousePosY > area->h_lower) {
                area->h_upper = mousePosY;
            }
            else if (mousePosY < area->h_lower) {
                area->h_lower = mousePosY;
            }
        } // event.Dragging
        else if (event.LeftUp() && createNewArea) {
            
            area->data_lp = new unsigned char[(3*(area->w_upper - area->w_lower) *
                    (area->h_upper - area->h_lower))];
            
            for (int iDataInitialization = 0;
            iDataInitialization < ((area->w_upper - area->w_lower) *
                    (area->h_upper - area->h_lower));
            iDataInitialization++) {
                *(area->data_lp + (3*iDataInitialization)) = 0;      // red init
                *(area->data_lp + (3*iDataInitialization + 1)) = 0;  // green init
                *(area->data_lp + (3*iDataInitialization + 2)) = 0;  // blue init
            }
            
            if (verboseMode) {
                cout << "Area initialized." << endl;
            }
            if (verboseMode) {
                cout << "Created area of size: " << (area->w_upper - area->w_lower) << "x"
                        << (area->h_upper - area->h_lower) << endl;
                cout << "Width spans from: " << area->w_lower << " to " << area->w_upper << endl;
                cout << "Height spans from: " << area->h_lower << " to " << area->h_upper << endl;
            }
            
            createNewArea = false;
            UpdateDialogSensitivities();
        } // event.LeftUp
    }
}

void MovementDetector::KeyDown(wxKeyEvent& event) 
{    
    if (showAreasInitialized) {
        long key_pressed = event.GetKeyCode();
        if ((key_pressed == WXK_TAB)||(key_pressed == WXK_RIGHT)) {
            SelectNextArea();
        }
        else if (key_pressed == WXK_LEFT) {
            SelectPreviousArea();
        }
        else if (key_pressed == WXK_DELETE) {
            RemoveSelectedArea();
        }
    }
}

void MovementDetector::SetParentWindow(wxWindow* windowPtr) {
    parentWindow = windowPtr;
}
