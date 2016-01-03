#ifndef __Helper_H_INCLUDED__   // if Helper.h hasn't been included yet...
#define __Helper_H_INCLUDED__   // #define this so the compiler knows it has been 

#include <iomanip>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sstream>
#include <cctype>

class Helper
{ 
    public:
        
        static void getFilesInDirectory(std::vector<std::string> &out, const std::string &directory);
        static void tokenize(const std::string & str, std::vector<std::string> & tokens, const std::string & delimiters);
        static std::string urlencode(const std::string &value);
};

#endif