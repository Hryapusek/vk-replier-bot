cmake_minimum_required(VERSION 3.10)
project(vk-bot)
set(CMAKE_BUILD_TYPE Debug)

find_library(JSONCPP jsoncpp REQUIRED)
find_library(CPR cpr REQUIRED)
find_library(CURL curl REQUIRED)
find_library(HTTPSERVER httpserver REQUIRED)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

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
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Wno-switch)
    endif()
endif()
set_target_properties(${TARGET} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED TRUE)
set(SOURCE_FILES_LIST
  main.cpp
  VkApi/Exceptions.cpp
  VkApi/Exceptions.hpp
  VkApi/Responses/Messages/MessagesGetByConversationMessageIdResponse.hpp
  VkApi/Responses/Messages/MessagesGetByConversationMessageIdResponse.cpp
  VkApi/Requests/Utils.hpp
  VkApi/Requests/Init.cpp
  VkApi/Requests/BaseRequest.hpp
  VkApi/Requests/BaseRequest.cpp
  VkApi/Requests/Messages/MessagesSendRequest.hpp
  VkApi/Requests/Messages/MessagesSendRequest.cpp
  VkApi/Requests/Messages/MessagesGetByConversationMessageIdRequest.hpp
  VkApi/Requests/Messages/MessagesGetByConversationMessageIdRequest.cpp
  VkApi/Objects/Message.hpp
  VkApi/Objects/Message.cpp
  VkApi/Callback/Event/Utils.hpp
  VkApi/Callback/Event/Utils.cpp
  VkApi/Callback/Event/Event.hpp
  VkApi/Callback/Event/Event.cpp
  VkApi/Callback/Event/EventObjects/NewMessage.hpp
  VkApi/Callback/Event/EventObjects/NewMessage.cpp
  VkApi/Callback/Event/EventObjects/EventObject.hpp
  Server/Server.hpp
  Server/Server.cpp
  Server/EventProcessingStrategyI.hpp
  MessageProcessing/TagsPool.hpp
  MessageProcessing/TagsPool.cpp
  MessageProcessing/MsgUtils.hpp
  MessageProcessing/MsgUtils.cpp
  MessageProcessing/TagsCmdsProcessingStrategy.hpp
  MessageProcessing/TagsCmdsProcessingStrategy.cpp
  MessageProcessing/CommandsPool.hpp
  MessageProcessing/CommandsPool.cpp
  MessageProcessing/ArgsExtractor.hpp
  MessageProcessing/ArgsExtractor.cpp
  MessageProcessing/Tags/AllTag.hpp
  MessageProcessing/Tags/AllTag.cpp
  MessageProcessing/Tags/AllImportantTag.hpp
  MessageProcessing/Tags/AllImportantTag.cpp
  MessageProcessing/Interfaces/TextTriggerI.hpp
  MessageProcessing/Interfaces/TextTriggerI.cpp
  MessageProcessing/Commands/StatusCommand.hpp
  MessageProcessing/Commands/StatusCommand.cpp
  MessageProcessing/Commands/RegTargetCommand.hpp
  MessageProcessing/Commands/RegTargetCommand.cpp
  MessageProcessing/Commands/RegSourceCommand.hpp
  MessageProcessing/Commands/RegSourceCommand.cpp
  MessageProcessing/Commands/RegGodlikeCommand.hpp
  MessageProcessing/Commands/RegGodlikeCommand.cpp
  MessageProcessing/Commands/DelTargetCommand.hpp
  MessageProcessing/Commands/DelTargetCommand.cpp
  MessageProcessing/Commands/DelTargetByNumCommand.hpp
  MessageProcessing/Commands/DelTargetByNumCommand.cpp
  MessageProcessing/Commands/DelSourceCommand.hpp
  MessageProcessing/Commands/DelSourceCommand.cpp
  MessageProcessing/Commands/ChangeModeCommand.hpp
  MessageProcessing/Commands/ChangeModeCommand.cpp
  Logging/Logger.hpp
  Logging/Logger.cpp
  Logging/easylogging++.h
  Logging/easylogging++.cc
  GeneralUtils/Result.hpp
  GeneralUtils/Result.cpp
  GeneralUtils/JsonUtils.hpp
  GeneralUtils/JsonUtils.cpp
  BusinessLogic/ConfigOperations.hpp
  BusinessLogic/ConfigOperations.cpp
  BusinessLogic/ConfigHolder.hpp
  BusinessLogic/ConfigHolder.cpp
  BusinessLogic/ConfigConditions.hpp
  BusinessLogic/ConfigConditions.cpp
  BusinessLogic/BusinessLogic.hpp
  BusinessLogic/BusinessLogic.cpp
  BusinessLogic/ConfigTypes/TargetsContainer.hpp
  BusinessLogic/ConfigTypes/TargetsContainer.cpp
  )
target_include_directories(${TARGET} PRIVATE . )
target_sources(${TARGET} PUBLIC ${SOURCE_FILES_LIST})
target_link_libraries(${TARGET} ${JSONCPP} ${CPR} ${CURL} ${HTTPSERVER})
target_compile_definitions(${TARGET} PUBLIC ELPP_THREAD_SAFE)
