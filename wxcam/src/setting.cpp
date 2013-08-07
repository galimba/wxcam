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

#include "setting.h"
#include <iostream>
#include <sstream>
#include <wx/filename.h>
#include <wx/dir.h>

using namespace std;

Setting* Setting::setting = NULL;
wxMutex Setting::mutex;
CFile* Setting::cfile = NULL;

Setting* Setting::GetInstance() 
{
    wxMutexLocker mLocker( mutex );
    if ( !setting )
        setting = new Setting();
    return setting;
}

Setting::Setting() 
{
    wxString confFile = wxFileName::GetHomeDir();
    confFile += wxString("/.wxcam", wxConvUTF8);
    cfile = new CFile( (const char*)confFile.mb_str(wxConvUTF8) );
}

Setting::~Setting() 
{
    wxMutexLocker mLocker( mutex );
    if ( setting ) {
        delete setting;
        setting = NULL;
    }
    if ( cfile ) {
        delete cfile;
        cfile = NULL;
    }
}

bool Setting::GetAlreadyOpen() 
{    
    if(cfile->readValue( "Opened:" ) == "Yes")
        return true;
    else
        return false;   
}

bool Setting::isFirstExecution() 
{
    wxMutexLocker mLocker( mutex );
    if(cfile->fileOpened())
        return !GetAlreadyOpen();
    else
        return true;
            
}

std::string Setting::GetDeviceFile() 
{
    wxMutexLocker mLocker( mutex );
    return cfile->readValue( "Device:" );
}

std::string Setting::GetAudioDeviceFile() 
{
    wxMutexLocker mLocker( mutex );
    return cfile->readValue( "AudioDevice:" );
}

std::string Setting::GetDriverAPI()
{
    wxMutexLocker mLocker( mutex );
    return cfile->readValue( "DriverAPI:" );
}

std::string Setting::GetFrameFormat() 
{
    wxMutexLocker mLocker( mutex );
    return cfile->readValue( "Format:" );
}

std::string Setting::GetSnapshotFile( bool orig ) 
{
    wxMutexLocker mLocker( mutex );
    wxString path;
    std::string fileName = cfile->readValue( "Snapshot:" );
    if ( orig )
        return fileName.c_str();
    else
        return (const char*)getNonOverwrittenFile(wxString(fileName.c_str(), wxConvUTF8)).mb_str(wxConvUTF8);
    
}

std::string Setting::GetVideoFile( bool orig ) 
{
    wxMutexLocker mLocker( mutex );
    wxString fileName = wxString(cfile->readValue( "Video:" ).c_str(), wxConvUTF8);
    if ( orig )       
        return (const char*)fileName.mb_str(wxConvUTF8);    
    else
        return (const char*)getNonOverwrittenFile(fileName).mb_str(wxConvUTF8);
}

std::string Setting::GetResolution() 
{
    wxMutexLocker mLocker( mutex );
    return cfile->readValue( "Resolution:" );
}

std::string Setting::GetFrameRate() 
{
    wxMutexLocker mLocker( mutex );
    return cfile->readValue( "Frame_Rate:" );
}

std::string Setting::GetCompression() 
{
    wxMutexLocker mLocker( mutex );
    return cfile->readValue( "Compression:" );
}

int Setting::GetCompressionLevel() 
{
    wxMutexLocker mLocker( mutex );
    return atoi(cfile->readValue( "Compression_Level:" ).c_str());
}

bool Setting::GetAudioEnabled() 
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Audio:" ) == "Yes")
        return true;
    return false;
}

int Setting::GetSilence() 
{
    wxMutexLocker mLocker( mutex );
    return atoi(cfile->readValue( "Silence:" ).c_str());        
}

bool Setting::GetAdjustColors()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "AdjustColors:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetNegative()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Negative:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetUpturned()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Upturned:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetMonochrome()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Monochrome:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetMirror()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Mirror:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetVerticalMirror()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "VerticalMirror:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetInstagram()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Instagram:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetStretch()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Stretch:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetEdgeAsm()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "EdgeAsm:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetBonaAsm()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "BonaAsm:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetPixelate()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Pixelate:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetMedian()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Median:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetMedianAsm()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "MedianAsm:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetBlueOnlyAsm()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "BlueOnlyAsm:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetMonoAsm()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "MonoAsm:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetNegativeAsm()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "NegativeAsm:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetBlur()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Blur:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetDeriche()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Deriche:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetNoise()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Noise:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetSharpen()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Sharpen:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetEdge()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Edge:" ) == "Yes")
        return true;
    return false; 
}

bool Setting::GetLaplacian()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "Laplacian:" ) == "Yes")
        return true;
    return false;
}

bool Setting::GetDateTime()
{
    wxMutexLocker mLocker( mutex );
    if (cfile->readValue( "DateTimeLabel:" ) == "Yes")
        return true;
    return false;
}

video_std Setting::GetVideoStd()
{
    wxMutexLocker mLocker( mutex );
    video_std std;
    if (cfile->readValue( "VideoStd:" ) == "PAL")
        std = MODE_PAL;
    else if (cfile->readValue( "VideoStd:" ) == "NTSC")
        std = MODE_NTSC;
    else if (cfile->readValue( "VideoStd:" ) == "SECAM")
        std = MODE_SECAM; 
    else
        std = NONE; 
    return std;   
}

void Setting::SetAlreadyOpen()
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "Opened:", "Yes" );    
}

void Setting::SetDeviceFile( const std::string &device ) 
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "Device:", device );
}

void Setting::SetAudioDeviceFile( const std::string &device ) 
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "AudioDevice:", device );
}

void Setting::SetDriverAPI(const std::string & api)
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "DriverAPI:", api );
}

void Setting::SetFrameFormat( const std::string &format ) 
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "Format:", format );    
}

void Setting::SetSnapshotFile( const std::string &snapshot ) 
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "Snapshot:", snapshot );
}

void Setting::SetVideoFile( const std::string &video ) 
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "Video:", video );
}

void Setting::SetResolution( const std::string &resolution ) 
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "Resolution:", resolution );
}

void Setting::SetFrameRate( const std::string &frameRate ) 
{
    wxMutexLocker mLocker( mutex );  
    cfile->writeValueReplace( "Frame_Rate:", frameRate );         
}

void Setting::SetCompression( const std::string &compression ) 
{
    wxMutexLocker mLocker( mutex );
    cfile->writeValueReplace( "Compression:", compression );
}

void Setting::SetCompressionLevel( int compressionLevel ) 
{
    wxMutexLocker mLocker( mutex );
    ostringstream s;
    s<<compressionLevel;
    cfile->writeValueReplace( "Compression_Level:", s.str() );   
}

void Setting::SetAudioEnabled( bool b ) 
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Audio:", "Yes" );
    else
        cfile->writeValueReplace( "Audio:", "No" );
}

void Setting::SetSilence( int silence ) 
{
    wxMutexLocker mLocker( mutex );
    ostringstream s;
    s<<silence;
    cfile->writeValueReplace( "Silence:", s.str() );
}

void Setting::SetAdjustColors(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "AdjustColors:", "Yes" );
    else
        cfile->writeValueReplace( "AdjustColors:", "No" );
}

void Setting::SetNegative(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Negative:", "Yes" );
    else
        cfile->writeValueReplace( "Negative:", "No" );
}

void Setting::SetUpturned(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Upturned:", "Yes" );
    else
        cfile->writeValueReplace( "Upturned:", "No" );
}

void Setting::SetMonochrome(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Monochrome:", "Yes" );
    else
        cfile->writeValueReplace( "Monochrome:", "No" );
}

void Setting::SetMirror(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Mirror:", "Yes" );
    else
        cfile->writeValueReplace( "Mirror:", "No" );
}

void Setting::SetVerticalMirror(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "VerticalMirror:", "Yes" );
    else
        cfile->writeValueReplace( "VerticalMirror:", "No" );
}

void Setting::SetInstagram(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Instagram:", "Yes" );
    else
        cfile->writeValueReplace( "Instagram:", "No" );
}

void Setting::SetStretch(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Stretch:", "Yes" );
    else
        cfile->writeValueReplace( "Stretch:", "No" );
}

void Setting::SetEdgeAsm(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "EdgeAsm:", "Yes" );
    else
        cfile->writeValueReplace( "EdgeAsm:", "No" );
}

void Setting::SetBonaAsm(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "BonaAsm:", "Yes" );
    else
        cfile->writeValueReplace( "BonaAsm:", "No" );
}

void Setting::SetPixelate(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Pixelate:", "Yes" );
    else
        cfile->writeValueReplace( "Pixelate:", "No" );
}

void Setting::SetMedian(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Median:", "Yes" );
    else
        cfile->writeValueReplace( "Median:", "No" );
}

void Setting::SetMedianAsm(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "MedianAsm:", "Yes" );
    else
        cfile->writeValueReplace( "MedianAsm:", "No" );
}

void Setting::SetNegativeAsm(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "NegativeAsm:", "Yes" );
    else
        cfile->writeValueReplace( "NegativeAsm:", "No" );
}

void Setting::SetBlueOnlyAsm(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "BlueOnlyAsm:", "Yes" );
    else
        cfile->writeValueReplace( "BlueOnlyAsm:", "No" );
}

void Setting::SetMonoAsm(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "MonoAsm:", "Yes" );
    else
        cfile->writeValueReplace( "MonoAsm:", "No" );
}

void Setting::SetBlur(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Blur:", "Yes" );
    else
        cfile->writeValueReplace( "Blur:", "No" );
}

void Setting::SetDeriche(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Deriche:", "Yes" );
    else
        cfile->writeValueReplace( "Deriche:", "No" );
}

void Setting::SetNoise(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Noise:", "Yes" );
    else
        cfile->writeValueReplace( "Noise:", "No" );
}

void Setting::SetSharpen(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Sharpen:", "Yes" );
    else
        cfile->writeValueReplace( "Sharpen:", "No" );
}

void Setting::SetEdge(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Edge:", "Yes" );
    else
        cfile->writeValueReplace( "Edge:", "No" );
}

void Setting::SetLaplacian(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "Laplacian:", "Yes" );
    else
        cfile->writeValueReplace( "Laplacian:", "No" );
}

void Setting::SetDateTime(bool b)
{
    wxMutexLocker mLocker( mutex );
    if(b)
        cfile->writeValueReplace( "DateTimeLabel:", "Yes" );
    else
        cfile->writeValueReplace( "DateTimeLabel:", "No" );
}

void Setting::SetVideoStd(video_std std)
{
    wxMutexLocker mLocker( mutex );
    if (std == MODE_PAL) 
        cfile->writeValueReplace( "VideoStd:", "PAL" );
    else if (std == MODE_NTSC) 
        cfile->writeValueReplace( "VideoStd:", "NTSC" );
    else if (std == MODE_SECAM) 
        cfile->writeValueReplace( "VideoStd:", "SECAM" );
    else //NONE
        cfile->writeValueReplace( "VideoStd:", "None" );    
}

void Setting::WriteDefaults() 
{
    wxMutexLocker mLocker( mutex );
    wxString s;
    cfile->writeValueReplace( "Device:", "/dev/video0" );
    cfile->writeValueReplace( "AudioDevice:", "/dev/dsp" );
    cfile->writeValueReplace( "DriverAPI:", "AUTO" );
    cfile->writeValueReplace( "Format:", "AUTO" );
    s = wxFileName::GetHomeDir() + wxString("/Desktop/image.tif", wxConvUTF8);
    cfile->writeValueReplace( "Snapshot:", (const char*)s.mb_str(wxConvUTF8) );
    s = wxFileName::GetHomeDir() + wxString("/Desktop/video.avi", wxConvUTF8);
    cfile->writeValueReplace( "Video:", (const char*)s.mb_str(wxConvUTF8) );
    cfile->writeValueReplace( "Resolution:", "160x120" );
    cfile->writeValueReplace( "Frame_Rate:", "15" );
    cfile->writeValueReplace( "Compression:", "none" );
    cfile->writeValueReplace( "Compression_Level:", "50" );
    cfile->writeValueReplace( "Audio:", "Yes" );
    cfile->writeValueReplace( "Silence:", "20" );
    cfile->writeValueReplace( "Opened:", "No" );
    cfile->writeValueReplace( "AdjustColors:", "No" );
    cfile->writeValueReplace( "Negative:", "No" );
    cfile->writeValueReplace( "Upturned:", "No" );
    cfile->writeValueReplace( "Monochrome:", "No" );
    cfile->writeValueReplace( "Mirror:", "No" );
    cfile->writeValueReplace( "VerticalMirror:", "No" );
    cfile->writeValueReplace( "Instagram:", "No" );
    cfile->writeValueReplace( "Stretch:", "No" );
    cfile->writeValueReplace( "EdgeAsm:", "No" );
    cfile->writeValueReplace( "Blur:", "No" );
    cfile->writeValueReplace( "Deriche:", "No" );
    cfile->writeValueReplace( "Sharpen:", "No" );
    cfile->writeValueReplace( "Noise:", "No" );
    cfile->writeValueReplace( "Edge:", "No" );
    cfile->writeValueReplace( "Laplacian:", "No" );
    cfile->writeValueReplace( "DateTimeLabel:", "No" );
    cfile->writeValueReplace( "VideoStd:", "None" );
}

wxString Setting::getDirByFile(const wxString &fileName) 
{
    string tmp = (const char*)fileName.mb_str(wxConvUTF8);
    size_t pos;
    if( ( pos = tmp.rfind('/') ) != std::string::npos ) {
        tmp.erase( pos );
        return wxString(tmp.c_str(), wxConvUTF8);
    }
    else
        return wxGetCwd();
}

wxString Setting::getFileByPath(const wxString &fileName) 
{
    string tmp = (const char*)fileName.mb_str(wxConvUTF8);
    size_t pos;
    if( ( pos = tmp.rfind('/') ) != std::string::npos ) {
        tmp.erase( 0, pos + 1 );
        return wxString(tmp.c_str(), wxConvUTF8);
    }
    else
        return fileName;
}

wxString Setting::getNonOverwrittenFile(const wxString &fileName) 
{
    int counter = 0;
    char ret[80];
    wxString exten = getFileByPath( fileName );
    wxString nameWoExten = getFileByPath( fileName );
    size_t dim = getFileByPath( fileName ).size() - 4;
    exten.erase( 0,  dim );
    nameWoExten.erase( dim );
    wxString found, search;
    wxDir dir( getDirByFile( fileName ) );
    search = getFileByPath( fileName );
    while ( dir.GetFirst( &found, search, wxDIR_FILES ) ) {
        counter++;
        snprintf( ret, 80, "%s%s%03d%s", (const char*)nameWoExten.mb_str(wxConvUTF8),
                "-", counter, (const char*)exten.mb_str(wxConvUTF8) );
        search = wxString(ret, wxConvUTF8);
    }
    snprintf( ret, 80, "%s%s%s", (const char*)getDirByFile( fileName ).mb_str(wxConvUTF8), "/",
            (const char*)search.mb_str(wxConvUTF8) );
    return wxString(ret, wxConvUTF8);
}

