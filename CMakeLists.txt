

cmake_minimum_required(VERSION 3.2)
project(template)

include_directories(TCP/inc)
include_directories(log/inc)
include_directories(time/inc)
include_directories(HTTP/inc)
include_directories(base/inc)
include_directories(mysql/inc)

include_directories(mysql-connector/include)

aux_source_directory(TCP/src DIR_SRCS)
aux_source_directory(log/src DIR_SRCS)
aux_source_directory(time/src DIR_SRCS)
aux_source_directory(HTTP/src DIR_SRCS)
aux_source_directory(base/src DIR_SRCS)
aux_source_directory(mysql/src DIR_SRCS)


LINK_DIRECTORIES(mysql-connector/lib) 
LINK_DIRECTORIES(/usr/local/lib64)

link_libraries("pthread" "mysqlclient")

aux_source_directory(./ DIR_SRCS)

set(TEST_MATH ${DIR_SRCS})   #这句话要是不加，则不会生成src下的目录


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -L/usr/local/lib64 -lstdc++ -DDEBUG")   #对应g++的输入参数

add_executable(out.exe ${TEST_MATH})

