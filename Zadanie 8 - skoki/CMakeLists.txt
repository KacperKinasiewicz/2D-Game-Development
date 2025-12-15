add_executable(Zadanie8_Skoki main.cpp)

target_link_libraries(Zadanie8_Skoki PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie8_Skoki POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie8_Skoki>" /E /Y /I /Q
)