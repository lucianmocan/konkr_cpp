cmake_minimum_required(VERSION 3.5)
project(konkr LANGUAGES CXX)


file(GLOB_RECURSE ALL_SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
    add_custom_target(
        clang_format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=Google ${ALL_SOURCE_FILES}
        COMMENT "Running clang-format on all source files"
    )
    add_custom_target(
        clang_format_check
        COMMAND ${CLANG_FORMAT_EXE} -n -Werror -style=Google ${ALL_SOURCE_FILES}
        COMMENT "Checking clang-format on all source files"
        COMMAND_EXPAND_LISTS
    )
else()
    message(WARNING "clang-format not found! Code formatting will be skipped.")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# nlohmann::json
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2) # Or latest version
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  TGUI
  GIT_REPOSITORY https://github.com/texus/TGUI.git
  GIT_TAG        v1.8.0 # Or v1.9.0 if available and compatible
)
FetchContent_MakeAvailable(TGUI)

add_subdirectory(src/rendering)
add_subdirectory(src/world)
add_subdirectory(src/game_logic)
add_subdirectory(src/ui)

add_executable(main src/main.cc)
target_compile_features(main PRIVATE cxx_std_23)

target_link_libraries(main
    PRIVATE
    rendering
    ui
    SFML::Graphics
    nlohmann_json
    TGUI::TGUI
    )

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(ASSETS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(ASSETS_DESTINATION_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

if(EXISTS ${ASSETS_SOURCE_DIR})

    add_custom_command(
        TARGET main POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SOURCE_DIR} ${ASSETS_DESTINATION_DIR}
        COMMENT "Copying assets directory to build output"
        VERBATIM 
    )
else()
    message(WARNING "Assets source directory not found: ${ASSETS_SOURCE_DIR}")
endif()