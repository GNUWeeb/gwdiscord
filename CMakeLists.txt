
cmake_minimum_required (VERSION 3.6)
project(gwdiscord)

if(NOT SLEEPY_DISCORD_CMAKE)
	add_subdirectory(spm-discord)
endif()

add_executable(
	gwdiscord
	main.cpp
	src/Main.cpp
	src/Main.hpp
)

target_link_libraries(
	gwdiscord
	sleepy-discord
)
