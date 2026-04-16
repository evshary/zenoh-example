cmake_minimum_required(VERSION 3.20)

project(zenoh_pico_example LANGUAGES C)

find_package(zenohpico REQUIRED)

add_executable(zp_simple_pub simple_z_pub.c)
set_property(TARGET zp_simple_pub PROPERTY C_STANDARD 11)
target_link_libraries(zp_simple_pub PRIVATE zenohpico::lib)

add_executable(zp_simple_advanced_pub simple_z_advanced_pub.c)
set_property(TARGET zp_simple_advanced_pub PROPERTY C_STANDARD 11)
target_link_libraries(zp_simple_advanced_pub PRIVATE zenohpico::lib)
