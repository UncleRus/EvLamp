cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/esp-idf-lib/components)

set(PROJECT_VER 0.7.4)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(EvLamp)
