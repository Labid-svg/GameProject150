cmake_minimum_required(VERSION 3.10)

project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "/home/nazmul-hassan/PROJECT150/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.6 REQUIRED COMPONENTS
    system
    window
    graphics
    audio
)

add_executable(MyGame
    main.cpp
    space_shooter.cpp
)

target_link_libraries(MyGame
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
)
