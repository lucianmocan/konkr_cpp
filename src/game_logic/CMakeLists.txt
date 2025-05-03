add_library(game_logic STATIC
    game.cc
)

target_include_directories(game_logic PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(game_logic
    PRIVATE
        world
        rendering
        SFML::Graphics
)

target_compile_features(game_logic PRIVATE cxx_std_23)