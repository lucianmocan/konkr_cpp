add_library(ui STATIC
    user_interface.cc
)

target_include_directories(ui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(ui
    PRIVATE
        world
        SFML::Graphics # Public because sprite_sheet.h includes SFML headers
        nlohmann_json::nlohmann_json # Private because only sprite_sheet.cc uses json
        TGUI::TGUI
)

target_compile_features(ui PRIVATE cxx_std_23)