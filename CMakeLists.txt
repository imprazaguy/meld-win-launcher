project(meld-win-launcher)
cmake_minimum_required(VERSION 2.8.4)

add_executable(meld WIN32 meld.c meld.rc)
set_target_properties(meld PROPERTIES COMPILE_DEFINITIONS "USE_WINMAIN")

add_executable(meldc meld.c meld.rc)
