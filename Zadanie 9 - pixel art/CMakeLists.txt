add_executable(Zadanie9_PixelArt main.cpp)

target_link_libraries(Zadanie9_PixelArt PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie9_PixelArt POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie9_PixelArt>" /E /Y /I /Q
)