cmake_minimum_required(VERSION 3.5)
project(debtc)

include_directories( /home/rmnsv/mysql-connector-c++-1.1.7-linux-glibc2.5-x86-64bit/include/ /home/rmnsv/mysql-connector-c++-1.1.7-linux-glibc2.5-x86-64bit/include/cppconn /home/rmnsv/mysql-connector-c++-1.1.7-linux-glibc2.5-x86-64bit/lib)
link_directories(/home/rmnsv/mysql-connector-c++-1.1.7-linux-glibc2.5-x86-64bit/lib/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "")
set(SOURCE_FILES main.cpp handler.cpp database.cpp de.cpp html.cpp database.h handler.h de.h html.h global.h global.cpp structs.h structs.cpp)
add_executable(debtc ${SOURCE_FILES})

target_link_libraries(debtc mysqlcppconn mysqlcppconn curl pthread)
