cmake_minimum_required(VERSION 3.20)

project(StaticPool_test LANGUAGES C CXX)

add_executable(StaticPool_test 
    main.c
    ../static_pool.c
)

include_directories(
    ../
    )
