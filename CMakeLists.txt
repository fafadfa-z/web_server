

cmake_minimum_required(VERSION 3.2)
project(fafadfa_template)

include_directories(TCP/include)
include_directories(log/include)
include_directories(time/include)
include_directories(HTTP/include)
include_directories(base/include)
include_directories(mysql/include)

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


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -DDEBUG")   #对应g++的输入参数

add_executable(out.exe ${TEST_MATH})

