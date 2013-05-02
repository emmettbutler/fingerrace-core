//
//  FileManager.h
//  FingerRace
//
//  Created by Evan Moore on 5/1/13.
//
//

#ifndef __FingerRace__FileManager__
#define __FingerRace__FileManager__

#include <string>

class FileManager
{
public:
    static void saveFile(std::string fname);
    static bool readFile(std::string fname);
    static std::string android_getFilePath(std::string fname);
};

#endif