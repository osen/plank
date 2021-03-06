cmake_minimum_required(VERSION 2.6)
project(PLANK)

#if(MSVC)
#  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
#endif()

include_directories(src)

add_library(plank
  src/plank/palloc.c
  src/plank/vector.c
  src/plank/sstream.c
)

add_library(plankmm
  src/plankmm/plank.cpp
)

add_executable(ctest1 src/ctests/test1.c)
target_link_libraries(ctest1 plank)

add_executable(ctest2 src/ctests/test2.c)
target_link_libraries(ctest2 plank)

add_executable(ctest3 src/ctests/test3.c)
target_link_libraries(ctest3 plank)

add_executable(ctest4 src/ctests/test4.c)
target_link_libraries(ctest4 plank)

add_executable(cxxtest1 src/cxxtests/test1.cpp)
target_link_libraries(cxxtest1 plankmm)

add_executable(cxxtest2 src/cxxtests/test2.cpp)
target_link_libraries(cxxtest2 plankmm)

add_executable(cxxtest3 src/cxxtests/test3.cpp)
target_link_libraries(cxxtest3 plankmm)

add_executable(cxxtest4 src/cxxtests/test4.cpp)
target_link_libraries(cxxtest4 plankmm)
