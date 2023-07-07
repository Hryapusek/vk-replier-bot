cmake_minimum_required(VERSION 3.10)
project(vk-bot)
set(CMAKE_BUILD_TYPE Debug)

find_library(JSONCPP jsoncpp REQUIRED)
find_library(CPR cpr REQUIRED)
find_library(CURL curl REQUIRED)
find_library(HTTPSERVER httpserver REQUIRED)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost 1.74.0 COMPONENTS log log_setup)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!!!")
endif()

if(NOT JSONCPP)
    message(FATAL_ERROR "JSONCPP not found!!!")
endif()

if(NOT CPR)
    message(FATAL_ERROR "CPR not found!!!")
endif()

if(NOT CURL)
    message(FATAL_ERROR "CURL not found!!!")
endif()

set(TARGET vk-bot)
add_executable(${TARGET})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        target_compile_options(${TARGET} PRIVATE /W4 /WX)
    else()
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
set_target_properties(${TARGET} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED TRUE)
set(SOURCE_FILES_LIST
  main.cpp
  JsonUtils.cpp
  ConfigReader.cpp
  VkApi/Exceptions.cpp
  VkApi/Callback/Event/EventObjects/NewMessage.cpp
  VkApi/Callback/Event/Event.cpp
  VkApi/Callback/Event/Utils.cpp
  VkApi/Objects/Message.cpp
  VkApi/Requests/Messages/MessagesSendRequest.cpp
  VkApi/Requests/BaseRequest.cpp
  VkApi/Exceptions.cpp
  MessageProcessing/MessageProcessing.cpp
  BotTypes/TargetsTable.cpp
  MessageProcessing/Commands/RegChecker.cpp
  MessageProcessing/Commands/RegSource.cpp
  MessageProcessing/Commands/RegTarget.cpp
  MessageProcessing/Tags/TagAll.cpp
  MessageProcessing/Tags/TagAllImportant.cpp
  MessageProcessing/Utils.cpp
)
target_sources(${TARGET} PRIVATE ${SOURCE_FILES_LIST})
target_include_directories(${TARGET} PRIVATE BotTypes MessageProcessing VkApi)
include_directories(${Boost_INCLUDE_DIRS}) 
target_link_libraries(${TARGET} ${Boost_LIBRARIES})
target_link_libraries(${TARGET} ${JSONCPP} ${CPR} ${CURL} ${HTTPSERVER})