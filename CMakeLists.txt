cmake_minimum_required(VERSION 3.26)
project(DR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

include(GNUInstallDirs)

include(CTest)
find_program(MEMORYCHECK_COMMAND valgrind)
set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --show-leak-kinds=all")

# find_package(SDL)
find_package(cosmic REQUIRED)
set(DR_LINKS ${DR_LINKS} cosmic)

if (MSVC)
    # add_compile_options(/W4 /WX)
    include_directories("tmpinclude")
    set(DR_LINKS ${DR_LINKS} "${CMAKE_CURRENT_SOURCE_DIR}\\lib\\pdcurses.lib")
else()
    add_definitions(-D_POSIX_C_SOURCE)
    # add_compile_options(-Wall -Wextra -pedantic -Werror -ansi)
    set(DR_LINKS ${DR_LINKS} curses)
endif()

if (NOT MSVC)
    set(DR_LINKS ${DR_LINKS} m)
endif()

# if (SDL_LIBRARY)
#     set(DR_LINKS ${DR_LINKS} ${SDL_LIBRARY})
#     include_directories("${SDL_INCLUDE_DIR}")
# endif()

# if (SDL_MAIN_LIBRARY)
#     set(DR_LINKS ${DR_LINKS} ${SDL_MAIN_LIBRARY})
# endif()

include_directories(include)

file(GLOB DR_SOURCE src/*.c alt_main.c)

# add_executable(main WIN32 ${DR_SOURCE})
add_executable(main ${DR_SOURCE})
target_link_libraries(main ${DR_LINKS})


