//
//  Project: AWS-S3-Cpp-REST-API
//  Description: Example of how to use the AWS REST API with C++
//  Created:     03/01/2016
//  Author:      Cédric Verstraeten
//  Mail:        hello@cedric.ws
//  Website:     www.cedric.ws
//
//  The copyright to the computer program(s) herein
//  is the property of CédricVerstraeten, Belgium.
//  The program(s) may be used and/or copied.
//
/////////////////////////////////////////////////////

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "Guard.h"
#include "S3.h"
#include "Helper.h"

using namespace std;

// File directory
string fileDirectory = "/Users/cedricverst/test";
// Symbol directory 
string symbolDirectory = "/Users/cedricverst/symlink/";

// File watcher, will check if news files has been
// added to a specific directory, and create a symbolic link.
class Watcher
{
    public:
    static void addFile(const std::string & file)
    {
        ifstream f(file.c_str());
        if(f.good())
        {
            // Get filename
            vector<string> tokens;
            Helper::tokenize(file, tokens, "/");
            string fileName = tokens[tokens.size()-1];

            // create a symbol link
            string link = symbolDirectory + fileName;
            int beenCreated = symlink(file.c_str(), link.c_str());
            
            f.close();
        }
    }
    
    static void scan()
    {        
        FW::Guard * guard = new FW::Guard();
        guard->listenTo(fileDirectory);
        guard->onChange(&Watcher::addFile);
        guard->startLookingForNewFiles();
        
        while(true)
        {
            guard->look();
            usleep(1000*1000);
        }
    }
};

// This class will try to upload files to a S3 
// bucket.
class S3Uploader
{
    public:
    
    static void upload()
    {
        // Upload interval [0.5sec;4.2min]
        int min = 500;
        int max = 256000;
        int interval = min;
        
        // Initialize S3 credentials
        S3 s3;
        s3.setPublicKey("public");
        s3.setPrivateKey("private");
        
        while(true)
        {
            usleep(interval*1000);
            
            vector<string> storage;
            Helper::getFilesInDirectory(storage, symbolDirectory); // get all symbol links of directory
            
            vector<string>::iterator it = storage.begin();

            while(it != storage.end())
            { 
                string file = *it;
                bool hasBeenUploaded = s3.putObject(file);
                
                if(hasBeenUploaded)
                {
                    unlink(file.c_str()); // remove symbol link
                    interval = min; // reset interval
                }
                else
                {   
                    interval = (interval * 2 < max) ? interval * 2 : max; // update interval exponential.
                    break;
                }
                
                it++;
            }
        }
    }
};

class Sync
{
    public:
    static void start()
    {
        thread upload(S3Uploader::upload);
        thread watch(Watcher::scan);
        upload.join();
        watch.join();
    }
};

int main(int argc, char** argv)
{
    thread sync(Sync::start);
    sync.join();
    return 0;
}
