cmake_minimum_required(VERSION 3.12)
project(beasttest)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include/
        /usr/local/openssl/include
        )

link_directories(/usr/local/openssl/lib
        /usr/local/lib)

link_libraries(pthread
        boost_system
        ssl
        crypto
        boost_iostreams)

add_executable(beasttest main.cpp)