cmake_minimum_required(VERSION 3.0) 

project(lib)

set(SOURCES 
	./Cell.cpp
	./Solver.cpp
	)
	
set(HEADERS
	./Cell.h 
	./Solver.h
	./SystemPam.h
	)

add_library(lib STATIC ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC .)
