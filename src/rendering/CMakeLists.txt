add_library(rendering STATIC
    sprite_sheet.cc
    level.cc
    level_renderer.cc
    color_palette.h
    graphics.cc
)

target_include_directories(rendering PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(rendering
    PRIVATE
        world
        SFML::Graphics # Public because sprite_sheet.h includes SFML headers
        nlohmann_json::nlohmann_json # Private because only sprite_sheet.cc uses json
)

target_compile_features(rendering PRIVATE cxx_std_23)