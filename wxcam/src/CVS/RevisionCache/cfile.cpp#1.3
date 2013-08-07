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

#include "cfile.h"

using namespace std;

CFile::CFile(const char* fileName)
: fileName(fileName) 
{
    opened=false;
    readBuffer();
}

CFile::~CFile()
{}

bool CFile::writeValueReplace(const string& key, const string& value)
{
    string row;
    for(vector<string>::iterator it=buffer.begin(); it!=buffer.end(); ++it) {
        row = *it;
        if(key.size() <= row.size() ) {
            row.erase(key.size());
            if(key == row) {
                buffer.erase(it); 
                row = key + " " + value;
                buffer.push_back(row);
                return writeBuffer();
            }
        }
    } 
    //key not found in buffer - adding
    row = key + " " + value;
    buffer.push_back(row);
    return writeBuffer();
}

bool CFile::addValueNoReplace(const string& key, const string& value)
{
    string row = key + " " + value;
    buffer.push_back(row);
    return writeBuffer();
}

bool CFile::removeKey(const string& key) 
{
    string row;
    vector<string> tmp;
    for(vector<string>::iterator it=buffer.begin(); it!=buffer.end(); ++it) {
        row = *it;
        if( key.size() <= row.size()) {
            row.erase(key.size());
            if(key != row) 
                tmp.push_back(*it);        
        }
    }
    buffer=tmp;
    return writeBuffer();
}

bool CFile::removeElement(const string& key, const string& value) 
{
    vector<string> tmp;
    for(vector<string>::iterator it=buffer.begin(); it!=buffer.end(); ++it) {
        string row = key + " " + value;
        if(*it != row) 
            tmp.push_back(*it);        
    }
    buffer=tmp;
    return writeBuffer();
}

string CFile::readValue(const string& key) 
{
    string row;
    for(vector<string>::iterator it=buffer.begin(); it!=buffer.end(); ++it) {
        row = *it;
	string::size_type pos = row.find(" ");
        if(pos != string::npos)
            row.erase(pos);
        else
            continue;
        if(key == row) {
            row = *it;
            row.erase(0, key.size() + 1);
            return row;
        }
    }
    return string("");
    /*throw HeaderNotFoundException();*/
}

vector<string> CFile::readArrayValue(const string& key) 
{
    vector<string> ret_val;
    string row;
    for(vector<string>::iterator it=buffer.begin(); it!=buffer.end(); ++it) {
        row = *it;
	string::size_type pos = row.find(" ");
        if(pos != string::npos)
            row.erase(pos);
        else
            continue;
        if(key == row) {
            row = *it;
            row.erase(0, key.size() + 1);
            ret_val.push_back(row);
        }
    }
    return ret_val;
}

void CFile::readBuffer() 
{
    buffer.clear();
    string line;
    ifstream in(fileName.c_str());
    if(!in) {
        perror("Cannot open input file");
        opened=false;
	return;
    }
    opened=true;
    while(getline(in, line)) {
        if(line.size())
            buffer.push_back(line);
    }
    in.close();
}

bool CFile::writeBuffer() 
{
    ofstream out(fileName.c_str());
    if(!out) {
        perror("Cannot open file for writing");
        return false;
    }
    for(vector<string>::iterator it = buffer.begin(); it!=buffer.end(); ++it) {
        string s = *it;
        out<<s.c_str()<<endl;
    }
    out.close();
    return true;
}
