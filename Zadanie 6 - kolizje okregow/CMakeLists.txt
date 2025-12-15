add_executable(Zadanie6_KolizjeOkregow main.cpp)

target_link_libraries(Zadanie6_KolizjeOkregow PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie6_KolizjeOkregow POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie6_KolizjeOkregow>" /E /Y /I /Q
)