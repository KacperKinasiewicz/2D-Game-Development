add_executable(Zadanie3_Poziomy main.cpp)

target_link_libraries(Zadanie3_Poziomy PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie3_Poziomy POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie3_Poziomy>" /E /Y /I /Q
)