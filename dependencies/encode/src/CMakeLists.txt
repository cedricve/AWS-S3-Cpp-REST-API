# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# Build encode

add_library(ENCODE_LIBRARY 
    base64.cpp
    HMAC_SHA1.cpp
    SHA1.cpp)
    
set(DEPENDENCIES ${DEPENDENCIES} ENCODE_LIBRARY PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} ENCODE_LIBRARY PARENT_SCOPE)