cmake_minimum_required(VERSION 3.20)

project(zenoh_pico_example LANGUAGES C)

find_package(zenohpico REQUIRED)

add_executable(zp_simple_pubsub simple_z_pubsub.c)
set_property(TARGET zp_simple_pubsub PROPERTY C_STANDARD 11)
target_link_libraries(zp_simple_pubsub PRIVATE zenohpico::lib)

add_executable(zp_simple_advanced_pubsub simple_z_advanced_pubsub.c)
set_property(TARGET zp_simple_advanced_pubsub PROPERTY C_STANDARD 11)
target_link_libraries(zp_simple_advanced_pubsub PRIVATE zenohpico::lib)
