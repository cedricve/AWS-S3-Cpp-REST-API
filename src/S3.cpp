#include "S3.h"

using namespace std;

void S3::setPublicKey(string key)
{
    m_publicKey = key;
}

void S3::setPrivateKey(string key)
{
    m_privateKey = key;
}

string S3::authorize(const string request)
{
    BYTE digest[20];
    CHMAC_SHA1 hmac_sha1;
    hmac_sha1.HMAC_SHA1((BYTE*)request.c_str(), request.size(),(BYTE*)m_privateKey.c_str(),m_privateKey.size(),digest);
    string signature(base64_encode((unsigned char*)digest,20));
    return "AWS " + m_publicKey + ":" + signature;
}

string S3::getDate()
{
    time_t lt;
    time(&lt);
    struct tm * tmTmp;
    tmTmp = gmtime(&lt);

    char * buf = new char[37];
    strftime(buf,37,"%a, %d %b  %Y %X GMT",tmTmp);
    
    string stringBuffer(buf);
    return stringBuffer; 
}

string S3::getBuckets()
{
    vector<string> headers;
    
    // Host URL
    headers.push_back("Host: s3.amazonaws.com");
    
    // Date
    string date = getDate();
    headers.push_back("Date: " + date);
    
    // Authorize request
    string request = "GET\n";
    request += "\n";
    request += "\n";
    request += date + "\n";
    request += "/";
    headers.push_back("Authorization: " + authorize(request));
    
    // Url
    string url = "https://s3.amazonaws.com";
    
    // Execute get operation
    return get(url, headers);
}

string S3::getObjects(const string & bucket)
{
    vector<string> headers;
    
    // Host URL
    headers.push_back("Host: " + bucket + ".s3.amazonaws.com");
    
    // Date
    string date = getDate();
    headers.push_back("Date: " + date);
    
    // Authorize request
    string request = "GET\n";
    request += "\n";
    request += "\n";
    request += date + "\n";
    request += "/" + bucket + "/";
    headers.push_back("Authorization: " + authorize(request));
    
    // Url
    string url = "https://s3.amazonaws.com";
    
    // Execute get operation
    return get(url, headers);
}

bool S3::putObject(const string & file)
{
    vector<string> headers;
    
    // File name
    vector<string> tokens;
    Helper::tokenize(file, tokens, "/");
    string fileName = Helper::urlencode(tokens[tokens.size()-1]);
    
    // Host URL
    headers.push_back("Host: kerberostest.s3.amazonaws.com");
    
    // Date
    string date = getDate();
    headers.push_back("Date: " + date);
    
    // Authorize request
    string request = "PUT\n";
    request += "\n";
    request += "\n";
    request += date + "\n";
    request += "/kerberostest/" + fileName;
    headers.push_back("Authorization: " + authorize(request));
    
    // Url
    string url = "https://kerberostest.s3.amazonaws.com/" + fileName;
    
    // Body
    vector<string> body;
    body.push_back(file);
    
    // Execute get operation
    return put(url, headers, body);
}

string S3::get(const string & url, const vector<string> & headers)
{
    // Initialize curl 
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curlHandle = curl_easy_init();

    // Initialize headers
    curl_slist* httpHeaders = NULL;
    for(int i = 0; i < headers.size(); i++)
    {
        httpHeaders = curl_slist_append(httpHeaders, headers[i].c_str());
    }
    
    // Execute
    std::string output = "";
    if(curlHandle) 
    {
        curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION,1);//Set automaticallly redirection
        curl_easy_setopt(curlHandle, CURLOPT_MAXREDIRS,1);//Set max redirection times
        curl_easy_setopt(curlHandle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);//Set http version 1.1fer
        curl_easy_setopt(curlHandle, CURLOPT_HEADER, true);//Set header true
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, httpHeaders);//Set headers
        curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());//Set URL
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &output);

        curl_easy_perform(curlHandle);//Perform
        curl_global_cleanup();
    }
    
    return output;
}

bool S3::put(const string & url, const vector<string> & headers, const vector<string> & body)
{
    // Initialize curl 
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curlHandle = curl_easy_init();
    CURLcode result;
    
    // Initialize headers
    curl_slist* httpHeaders = NULL;
    for(int i = 0; i < headers.size(); i++)
    {
        httpHeaders = curl_slist_append(httpHeaders, headers[i].c_str());
    }
    
    // Intialize body
    string file = body[0];
    FILE * fd = fopen(file.c_str(), "rb"); /* open file to upload */  
    struct stat file_info;
    fstat(fileno(fd), &file_info);
    
    // Execute
    std::string output = "";
    if(curlHandle) 
    {
        curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION,1);//Set automaticallly redirection
        curl_easy_setopt(curlHandle, CURLOPT_MAXREDIRS,1);//Set max redirection times
        curl_easy_setopt(curlHandle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);//Set http version 1.1fer
        curl_easy_setopt(curlHandle, CURLOPT_HEADER, true);//Set header true
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, httpHeaders);//Set headers
        curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());//Set URL
        curl_easy_setopt(curlHandle, CURLOPT_TRANSFER_ENCODING, 1L); 
        curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curlHandle, CURLOPT_UPLOAD, 1L); 
        curl_easy_setopt(curlHandle, CURLOPT_READDATA, fd);
        curl_easy_setopt(curlHandle, CURLOPT_READFUNCTION, reader);
        curl_easy_setopt(curlHandle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &output);
        
        result = curl_easy_perform(curlHandle);//Perform
        curl_global_cleanup();
        
        return true;
        return (result == CURLE_OK);
    }
    
    return false;
}

size_t S3::write(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t S3::reader(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t retcode = fread(ptr, size, nmemb, stream);
    return retcode;
}
