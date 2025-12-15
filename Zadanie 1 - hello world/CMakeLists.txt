add_executable(Zadanie1_HelloWorld main.cpp)

target_link_libraries(Zadanie1_HelloWorld PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

add_custom_command(TARGET Zadanie1_HelloWorld POST_BUILD
    COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR}" "$<TARGET_FILE_DIR:Zadanie1_HelloWorld>" /E /Y /I /Q
)