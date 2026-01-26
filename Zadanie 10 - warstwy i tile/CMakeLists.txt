add_executable(Zadanie10_Warstwy main.cpp
        ParallaxLayer.cpp
        ParallaxLayer.h
        Camera.cpp
        Camera.h
        Level.cpp
        Level.h
        Player.cpp
        Player.h)

target_link_libraries(Zadanie10_Warstwy PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie10_Warstwy POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie10_Warstwy>" /E /Y /I /Q
)