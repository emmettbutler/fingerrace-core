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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <Foundation/Foundation.h>
#endif

using namespace std;

void FileManager::saveFile(string fname) {
    printf("setting first run flag\n");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string path = android_getFilePath(fname);
    FILE *fp = fopen(path.c_str(), "w");

    if (!fp) {
        return;
    }

    // wtf??
    fputs("to enable turbo mode:\n1) turn your phone upside down\n2) tap the tutorial button 49 times\n3) don't miss, or you'll have to start over!", fp);
    fclose(fp);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[NSUserDefaults standardUserDefaults] setBool:true forKey:@"firstrun_done"];
    [[NSUserDefaults standardUserDefaults] synchronize];
#endif
}

bool FileManager::readFile(string fname) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string path = android_getFilePath(fname);
    FILE *fp = fopen(path.c_str(), "r");
    char buf[50] = {0};

    if (!fp) {
        return false;
    } else {
        fclose(fp);
        return true;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool res = [[NSUserDefaults standardUserDefaults] boolForKey:@"firstrun_done"];
    printf("res: %d\n", res);
    return res;
#endif
}

string FileManager::android_getFilePath(string fname) {
    string path("");
    // In android, every programe has a director under /data/data.
    // The path is /data/data/ + start activity package name.
    // You can save application specific data here.
    path.append("/data/data/com.eperiod.fingerrace/");
    path.append(fname);

    return path;
}