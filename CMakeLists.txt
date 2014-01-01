project(meld-win-launcher)
cmake_minimum_required(VERSION 2.8.4)

if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -ansi")
endif()

add_executable(meld WIN32 meld.c meld.rc)
set_target_properties(meld PROPERTIES COMPILE_DEFINITIONS "USE_WINMAIN")
target_link_libraries(meld shlwapi)

add_executable(meldc meld.c meld.rc)
target_link_libraries(meldc shlwapi)
