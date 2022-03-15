cmake_minimum_required(VERSION 3.17)
project(map_process)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)

include_directories("../structures/include" "../utils/include" "rapidxml")
file(GLOB_RECURSE SRC "../structures/src/*.cpp" "../utils/src/*.cpp")
file(GLOB_RECURSE SRC_DRIVERS "map_process.cpp")

# ################ MAIN ################
add_executable(map_process ${SRC} ${SRC_DRIVERS})

# ################ ORIGINAL ################
if(NOT EXISTS original/map.xml)
    if(NOT EXISTS original/map.zip)
        message("Downloading map.zip")
        file(DOWNLOAD https://drive.google.com/uc?id=1cEqe8l_cDt1A1aqe7wAWVAMhYqSejv9L ${CMAKE_BINARY_DIR}/original/map.zip SHOW_PROGRESS)
    else()
        message("map.zip found, not downloading")
    endif()
    message("Unzipping map.zip into map.xml")
    execute_process(COMMAND unzip original/map.zip -d original)
else()
    message("map.xml found, not unzipping map.zip")
endif()

# ################ DATA ################
add_custom_target(data COMMAND map_process ${CMAKE_CURRENT_SOURCE_DIR}/processed/AMP < ${CMAKE_BINARY_DIR}/original/map.xml)
