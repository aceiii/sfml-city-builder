cmake_minimum_required(VERSION 3.4)
project(sfml_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(SOURCE_FILES
        src/main.cpp src/game_state.h src/game.cpp src/game.h src/game_state_start.cpp src/game_state_start.h src/game_state_editor.cpp src/game_state_editor.h src/texture_manager.cpp src/texture_manager.h src/animation_handler.cpp src/animation_handler.h src/tile.cpp src/tile.h)

add_executable(sfml_test ${SOURCE_FILES})

find_package(SFML REQUIRED)

set(SFML_LIBS
    sfml-system
    sfml-window
    sfml-graphics)

target_link_libraries(sfml_test ${SFML_LIBS})

add_custom_command(TARGET sfml_test COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:sfml_test>/data)
