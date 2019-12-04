cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(majin 	VERSION 1.0
		DESCRIPTION "Modern Web Framework"
                LANGUAGES C)

set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Werror -g")
set(CMAKE_C_FLAGS "-std=c11 -Wall -Werror -g")

set(CMAKE_COLOR_MAKEFILE ON)

include_directories(lib)

add_library(_sstring SHARED src/mem.c src/sstring.c)
add_library(_ssocket SHARED src/ssocket.c)
add_library(_llist SHARED src/llist.c)
add_library(_mmath SHARED src/mmath.c)
add_library(_tree SHARED src/mem.c src/tree.c)
add_library(_dynarray SHARED src/mem.c src/dynarray.c)

add_executable(${PROJECT_NAME}_exec src/main.c)

target_link_libraries( ${PROJECT_NAME}_exec _mmath  _sstring _llist _dynarray _tree m pthread _ssocket )
#target_link_libraries( ${PROJECT_NAME}_exec _dynarray)
