add_executable(Zadanie2_Poruszanie main.cpp)

target_link_libraries(Zadanie2_Poruszanie PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie2_Poruszanie POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie2_Poruszanie>" /E /Y /I /Q
)