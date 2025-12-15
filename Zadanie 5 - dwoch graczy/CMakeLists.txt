add_executable(Zadanie5_DwochGraczy main.cpp)

target_link_libraries(Zadanie5_DwochGraczy PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie5_DwochGraczy POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie5_DwochGraczy>" /E /Y /I /Q
)