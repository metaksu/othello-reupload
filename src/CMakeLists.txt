cmake_minimum_required(VERSION 2.6)

project("googletest-mva")

if (CMAKE_VERSION VERSION_LESS "3.1")
  add_definitions(-std=c++11)
else()
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(LINUX)
  add_compile_options(-DWITH_LINUX)
elseif(APPLE)
  add_compile_options(-DWITH_APPLE) 
else()
  add_compile_options(-DWITH_LINUX)
endif()
 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
add_executable(othello_app othello_game.c othello_board_t.c othello_game_tree_t.c othello_npc.c)
target_link_libraries(othello_app ${GTEST_LIBRARIES} -lpthread -lm)

add_executable(othello_board_tests othello_board_tests.cc othello_board_t.c othello_game_tree_t.c othello_npc.c)
target_link_libraries(othello_board_tests ${GTEST_LIBRARIES} -lpthread -lm)

add_executable(othello_npc_tests othello_npc_tests.cc othello_game_tree_t.c othello_board_t.c othello_npc.c)
target_link_libraries(othello_npc_tests ${GTEST_LIBRARIES} -lpthread -lm)

