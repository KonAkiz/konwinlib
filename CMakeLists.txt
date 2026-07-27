cmake_minimum_required(VERSION 3.10)
project(konwinlib LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(test examples/main.c)
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_definitions(test PRIVATE _POSIX_C_SOURCE=200809L)

add_executable(minimal_test examples/minimal.c)
target_include_directories(minimal_test PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_definitions(minimal_test PRIVATE _POSIX_C_SOURCE=200809L)

if (UNIX AND NOT APPLE)
  find_package(X11 REQUIRED)
  target_link_libraries(test PRIVATE X11 Xrandr)
  target_link_libraries(minimal_test PRIVATE X11 Xrandr)
elseif (WIN32)
  target_link_libraries(test PRIVATE gdi32 user32 kernel32)
  target_link_libraries(minimal_test PRIVATE gdi32 user32 kernel32)
endif()
