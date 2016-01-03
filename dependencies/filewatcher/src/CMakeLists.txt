# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# Build library

add_library(FILEWATCHER_LIBRARY 
    FileWatcher.cpp
    FileWatcherLinux.cpp
    FileWatcherOSX.cpp
    FileWatcherWin32.cpp
    Guard.cpp)
    
set(DEPENDENCIES ${DEPENDENCIES} FILEWATCHER_LIBRARY PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} FILEWATCHER_LIBRARY PARENT_SCOPE)