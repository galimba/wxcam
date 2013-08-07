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

#ifndef _frame_H
#define	_frame_H

#include <wx/wx.h>
#include <wx/datetime.h>
#include "device.h"
#include "uncompressed.h"
#include "xvidcodec.h"
#include "motion.h"
#include <string>
#include <list>
#include <CImg.h>

using namespace cimg_library;

extern Device cam; 
class wxCam;
class ProgressDlg;

class Frame : public wxWindow
{
    public:
        Frame(const std::string &resolution, wxCam* main, wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);
        ~Frame();    
        
        void OpenDevice();
        //void SetFormat();
        bool isPWC();
        bool isOpen();
        bool SetResolution(const std::string &resolution, const std::string &frameRate);
        void TakeSnapshot();
        void OnPaint( wxPaintEvent& event );
        void OnMouseEvent( wxMouseEvent& event );
        void OnKeyDown( wxKeyEvent& event );
        void OnIdle( wxIdleEvent &event );        
        void OnEraseBackground( wxEraseEvent &event );
        void Started();
        void startRecording();
        void stopRecording();
        void setFullscreen(bool enable);
        void OnAudioError( wxCommandEvent& event );
        void OnProgressEvent(wxCommandEvent& event);
        void OnUpdateProgress(wxUpdateUIEvent& event);
        void OnStartProgress(wxCommandEvent& event);
        void OnRecThreadTerminated(wxCommandEvent& event);
        void ActivateMovementDetection();  // New method to activate movement detection
        void DeactivateMovementDetection();  // New method to deactivate movement detection
        bool GetMovementDetectionStatus();  // Show whether movement detection is active or not
        void ShowMDAreas();  // New method to activate Show movement detection areas
        void HideMDAreas();  // New method to deactivate Show movement detection areas
        bool GetMDAreasShowStatus(); // Get movement detection areas show status
        void GetResolution(const std::string &resolution, int &w, int &h);     
        CImg<unsigned char> cimg_from_rgb(unsigned char const* rgb, const unsigned int dimw, const unsigned int dimh);
        void rgb_from_cimg(CImg<unsigned char> const & cimg, unsigned char *buffer);
       
    private:        
        void updateFrequency(float freq);
        bool isBusy();
        void applyFilters();
        void setBusy(bool);
        
        enum rec_status
        {
            REC_STARTING,
            REC_CONTINUING,
            REC_STOPPING,
            REC_NONE,
            REC_PAUSE
        };
        
    private:
        wxCam* mainFrame;
        bool deviceOpen;
        bool snapshot;
        bool started;
        bool fullScreen;
        bool movementDetection;
        bool restoreMDialog;
        MovementDetector mdet;
        bool showMDAreasStatus;
        int assessFrequency;
        unsigned int width, height; 
        wxDateTime rec_start;
        bool busy;
        std::list<float> freqList;
        std::string compression;              
        wxBitmap img;
        char *buffer;  
        rec_status rstatus;
        ProgressDlg *dialog;
        Avi *rec;
        DECLARE_EVENT_TABLE()
};

enum {
    PROGRESS_DIALOG_START,
    PROGRESS_DIALOG_EVENT,
    STOP_REC,
    AUDIO_ERR
};

#endif	/* _myframe_H */

