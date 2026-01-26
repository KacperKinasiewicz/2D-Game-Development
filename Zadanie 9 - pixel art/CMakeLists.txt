add_executable(Zadanie9_Pixel main.cpp
        Camera.cpp
        Camera.h
        Level.cpp
        Level.h
        Player.cpp
        Player.h
        GameOverScreen.cpp
        GameOverScreen.h)

target_link_libraries(Zadanie9_Pixel PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie9_Pixel POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie9_Pixel>" /E /Y /I /Q
)