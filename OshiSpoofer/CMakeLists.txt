cmake_minimum_required(VERSION 3.5)

project(OshiSpoofer LANGUAGES C)

set_property(GLOBAL PROPERTY C_STANDARD 23)

add_library(OshiSpoofer SHARED main.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(OshiSpoofer PRIVATE Threads::Threads)
target_compile_options(OshiSpoofer PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_include_directories(OshiSpoofer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/jni)
target_include_directories(OshiSpoofer PRIVATE include/jni/linux)
