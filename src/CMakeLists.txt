# ----------------------------------------------
# Executable + define linking libraries

    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
	add_executable(s3 main.cpp S3.cpp Helper.cpp)
	add_dependencies(s3 ${DEPENDENCIES})
	target_link_libraries(s3 ${LIBRARIES} curl)