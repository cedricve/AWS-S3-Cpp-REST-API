cmake_minimum_required( VERSION 2.8.3)
project(Multithread)

# ----------------------------------------------
# Variables

	set(DEPENDENCIES "")
	set(LIBRARIES "")
 
# ----------------------------------------------
# Add dependencies

	include_directories(${CMAKE_SOURCE_DIR}/dependencies/filewatcher/include)
	add_subdirectory(dependencies/filewatcher/src)
    
    include_directories(${CMAKE_SOURCE_DIR}/dependencies/encode/include)
	add_subdirectory(dependencies/encode/src)
    
# ----------------------------------------------
# Add subdirectory

    include_directories(${CMAKE_SOURCE_DIR}/include)
	add_subdirectory(src)