cmake_minimum_required(VERSION 3.20)
project(2D-Game_Development)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML REQUIRED COMPONENTS Graphics Window System Audio Network)

add_subdirectory("Zadanie 1 - hello world")
add_subdirectory("Zadanie 2 - poruszanie")
add_subdirectory("Zadanie 3 - poziomy")
add_subdirectory("Zadanie 4 - kamera")
add_subdirectory("Zadanie 5 - dwoch graczy")
add_subdirectory("Zadanie 6 - kolizje okregow")
add_subdirectory("Zadanie 7 - kolizje")
add_subdirectory("Zadanie 8 - skoki")
add_subdirectory("Zadanie 9 - pixel art")