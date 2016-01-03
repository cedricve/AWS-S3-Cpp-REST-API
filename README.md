# AWS-S3-Cpp-REST-API

This repository is an example of how to use the AWS REST API with C++. For more detailed information please [refer to following post](https://blog.cedric.ws/how-to-use-aws-with-your-c-application).

## How to use
To use this project with your own AWS account, the only thing you'll need to add to the source code is [your AWS credentials in the main.cpp file](https://github.com/cedricve/AWS-S3-Cpp-REST-API/blob/master/src/main.cpp#L84-L85).

## Build
To compile this repository, execute following commands:

    git clone https://github.com/cedricve/AWS-S3-Cpp-REST-API
    cd AWS-S3-Cpp-REST-API
    mkdir build && cd build
    cmake .. && make
    ../bin/s3

## Library
A class S3 is included which contains three methods. For more background information of the actual REST API, please check the [documentation page of AWS](http://docs.aws.amazon.com/AmazonS3/latest/API/APIRest.html).

### string getBuckets();
Get all your buckets.

### string getObjects(const string & bucket); 
Get all objects from a specific bucket.

### bool putObject(const string & file);
Add a file to your bucket.
