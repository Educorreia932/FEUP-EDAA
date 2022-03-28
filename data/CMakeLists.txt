cmake_minimum_required(VERSION 3.17)
project(data_process)

# set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ################ ORIGINAL ################
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/pkdd15-i/train.csv)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/pkdd15-i/train.zip)
        message("Downloading pkdd15-i/train.zip")
        file(DOWNLOAD https://dl.dropboxusercontent.com/s/7culexod1kn5xxr/train.zip ${CMAKE_CURRENT_SOURCE_DIR}/pkdd15-i/train.zip SHOW_PROGRESS)
    else()
        message("pkdd15-i/train.zip found, not downloading")
    endif()
    message("Unzipping pkdd15-i/train.zip into pkdd15-i/train.csv")
    execute_process(COMMAND unzip ${CMAKE_CURRENT_SOURCE_DIR}/pkdd15-i/train.zip -d ${CMAKE_CURRENT_SOURCE_DIR}/pkdd15-i)
else()
    message("pkdd15-i/train.csv found, not unzipping pkdd15-i/train.zip")
endif()

# ################ DATA ################
add_custom_target(
    pkdd15-i
    COMMAND python3 process.py < train.csv > pkdd15-i.runs
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/pkdd15-i
)
