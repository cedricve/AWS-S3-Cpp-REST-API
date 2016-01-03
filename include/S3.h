#ifndef __S3_H_INCLUDED__   // if S3.h hasn't been included yet...
#define __S3_H_INCLUDED__   // #define this so the compiler knows it has been included

#include <vector>
#include <curl/curl.h>
#include <time.h>
#include "HMAC_SHA1.h"
#include "base64.h"
#include "Helper.h"

using namespace std;

class S3
{
    private:
    
        string m_publicKey;
        string m_privateKey;
    
    public:
    
        void setPublicKey(string key);
        void setPrivateKey(string key);
    
        string getDate();
        string authorize(const string request);
    
        // Resource methods
        string getBuckets(); // returns a list of all buckets.
        string getObjects(const string & bucket); // returns all objects in bucket.
        bool putObject(const string & file); // returns true, if file has been uploaded succesfully.
    
        // HTTP methods with CURL
        string get(const string & url, const vector<string> & headers);
        bool post(const string & url, const vector<string> & headers, const vector<string> & body);
        bool put(const string & url, const vector<string> & headers, const vector<string> & body);
        void del();
    
        // Helper methods CURL
        static size_t write(void *contents, size_t size, size_t nmemb, void *userp);
        static size_t reader(void *ptr, size_t size, size_t nmemb, FILE *stream);
};

#endif