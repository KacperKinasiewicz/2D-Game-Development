add_executable(Zadanie7_KolizjeGraczy main.cpp)

target_link_libraries(Zadanie7_KolizjeGraczy PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie7_KolizjeGraczy POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie7_KolizjeGraczy>" /E /Y /I /Q
)