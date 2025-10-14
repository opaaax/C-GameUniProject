cmake_minimum_required(VERSION 3.27)
project(Projekt_Gra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)
include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(fmt)

add_executable(Projekt_Gra main.cpp
        gameWindow.h
        player.h
        Projectile.h
        enemy.h
        collectibles.h
        portals.h
        traps.h
        shop.h
        shop.h
)
target_link_libraries(Projekt_Gra PRIVATE sfml-graphics sfml-audio sfml-system sfml-window fmt)
target_compile_features(Projekt_Gra PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
            TARGET Projekt_Gra
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Projekt_Gra>
            VERBATIM)
endif()

install(TARGETS Projekt_Gra)