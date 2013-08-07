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

#ifndef CFILE_H
#define CFILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class HeaderNotFoundException {
    public:
        HeaderNotFoundException() {
            printf("Header not found\n");
        }
};

class FileNotFoundException {
    public:
        FileNotFoundException() {
            printf("File not found\n");
        }
};

class CFile {
    public:
        CFile(const char* fileName);
        ~CFile();
        bool writeValueReplace(const std::string& key, const std::string& value);
        bool addValueNoReplace(const std::string& key, const std::string& value);
        std::string readValue(const std::string& key);
        std::vector<std::string> readArrayValue(const std::string& key);
        bool removeKey(const std::string& key);
        bool removeElement(const std::string& key, const std::string& value);
        bool fileOpened() { return opened; }  //true if I can open the file for reading
        
    private:
        bool opened;
        void readBuffer();
        bool writeBuffer();
        std::vector<std::string> buffer;
        std::string fileName;
};

#endif //CFILE_H
