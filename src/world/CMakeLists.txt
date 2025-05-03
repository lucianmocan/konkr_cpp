add_library(world STATIC
    entity.cc
    tile.cc
    human_unit.cc
    townhall.cc
    castle.cc
    bandit.cc
    player.cc
)

target_include_directories(world PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(world
    PRIVATE
        SFML::Graphics
)

target_compile_features(world PRIVATE cxx_std_23)