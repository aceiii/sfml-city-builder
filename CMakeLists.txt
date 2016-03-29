cmake_minimum_required(VERSION 3.4)
project(sfml_test)

set(sfml_test_VERSION_MAJOR 1)
set(sfml_test_VERSION_MINOR 0)
set(sfml_test_VERSION_PATCH 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(SOURCE_FILES
        src/main.cpp src/game_state.h src/game.cpp src/game.h src/game_state_start.cpp src/game_state_start.h src/game_state_editor.cpp src/game_state_editor.h src/texture_manager.cpp src/texture_manager.h src/animation_handler.cpp src/animation_handler.h src/tile.cpp src/tile.h src/map.cpp src/map.h src/gui.cpp src/gui.h src/city.cpp src/city.h src/resource_manager.cpp src/resource_manager.h)

add_executable(sfml_test MACOSX_BUNDLE ${SOURCE_FILES})

find_package(SFML REQUIRED)

set(SFML_LIBS
    sfml-system
    sfml-window
    sfml-graphics)

target_link_libraries(sfml_test ${SFML_LIBS})

if (APPLE)
    set(MACOSX_DEPLOYMENT_TARGET 10.7)
    set(CMAKE_BUNDLE_VERSION  "${sfml_test_VERSION_MAJOR}.${sfml_test_VERSION_MINOR}.${sfml_test_VERSION_PATCH}")
    set(CMAKE_BUNDLE_NAME "CMake ${CMAKE_BUNDLE_VERSION}")

    set_target_properties(sfml_test PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/mac/Info.plist"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_BUNDLE_VERSION}"
            MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_BUNDLE_VERSION}"
            MACOSX_BUNDLE_COPYRIGHT "Copyright © 2016. All rights reserved. Blah")

    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    target_link_libraries(sfml_test ${COREFOUNDATION_LIBRARY})

    add_custom_command(TARGET sfml_test COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/data)
else()
    add_custom_command(TARGET sfml_test COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:sfml_test>/data)
endif()
