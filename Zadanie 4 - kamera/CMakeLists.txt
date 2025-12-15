add_executable(Zadanie4_Kamera main.cpp)

target_link_libraries(Zadanie4_Kamera PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie4_Kamera POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie4_Kamera>" /E /Y /I /Q
)