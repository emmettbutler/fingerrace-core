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

/*!
 * Sets the first run flag.
 * On Android: writes a file with hardcoded string.
 * On iOS: writes to NSUserDefaults.
 *
 * @param fname Name of file to be written.
 */
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

/*!
 * Checks for the presence of the first run flag
 * On Android: attempts to open the first run file.
 * On iOS: reads from NSUserDefaults.
 *
 * @param fname Name of file to be checked.
 * @return True if first run flag is present, false if not.
 */
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
    //printf("res: %d\n", res);
    return res;
#endif
}

/*!
 * @return Android app data directory as string.
 */
string FileManager::android_getFilePath(string fname) {
    string path("");
    path.append("/data/data/com.eperiod.fingerrace/");
    path.append(fname);

    return path;
}