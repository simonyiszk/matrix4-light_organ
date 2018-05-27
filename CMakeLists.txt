cmake_minimum_required(VERSION 2.6)
project(matrix_light_organ)

include_directories("fft-real/")  

find_package(ALSA REQUIRED)                                                                                  
include_directories(${ALSA_INCLUDE_DIRS})                  

add_executable(matrix_light_organ main.cpp)

target_link_libraries(matrix_light_organ ${ALSA_LIBRARIES})       

install(TARGETS matrix_light_organ RUNTIME DESTINATION bin)
