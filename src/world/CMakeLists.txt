add_library(world STATIC
    entity.cc
    tile.cc
)

target_include_directories(world PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(world
    PUBLIC
        SFML::Graphics # Public because sprite_sheet.h includes SFML headers
)

target_compile_features(world PRIVATE cxx_std_23)