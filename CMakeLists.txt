cmake_minimum_required(VERSION 3.5)
project(konkr LANGUAGES CXX)

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

add_subdirectory(rendering)

add_executable(main main.cc)
target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main
    PRIVATE
    rendering
    SFML::Graphics
    nlohmann_json)

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