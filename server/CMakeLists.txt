
project(server_interface_implementation)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(
    server_interface_implementation
    ${SRCS} 
    ${HEADERS}
)

target_include_directories(
    server_interface_implementation
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)