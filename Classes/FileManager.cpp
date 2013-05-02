//
//  FileManager.cpp
//  FingerRace
//
//  Created by Evan Moore on 5/1/13.
//
//

#include "cocos2d.h"
#include "FileManager.h"
#include <stdio.h>

using namespace std;

void FileManager::saveFile(string fname) {
    string path = getFilePath(fname);
    FILE *fp = fopen(path.c_str(), "w");

    if (!fp) {
        return;
    }

    fputs("to enable turbo mode:\n1) turn your phone upside down\n2) tap the tutorial button 49 times\n3) don't miss, or you'll have to start over!", fp);
    fclose(fp);
}

bool FileManager::readFile(string fname) {
    string path = getFilePath(fname);
    FILE *fp = fopen(path.c_str(), "r");
    char buf[50] = {0};

    if (!fp) {
        return false;
    } else {
        fclose(fp);
        return true;
    }
}

string FileManager::getFilePath(string fname) {
    string path("");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // In android, every programe has a director under /data/data.
    // The path is /data/data/ + start activity package name.
    // You can save application specific data here.
    path.append("/data/data/com.eperiod.fingerrace/");
    path.append(fname);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path = cocos2d::CCApplication::sharedApplication().getAppDataPath();
    path.append(fname);
#endif

    return path;
}