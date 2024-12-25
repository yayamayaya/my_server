cmake_minimum_required(VERSION 3.15)

project(server_interface)

set(CMAKE_C_COMPILER ${C_COMPILER})

#add_definitions(-DDEBUG)

add_compile_definitions(-DBIN_PATH=\"${CMAKE_CURRENT_BINARY_DIR}\")

add_definitions(
-D _DEBUG -ggdb3 -O0 -Wall -Wextra -Waggressive-loop-optimizations
-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion
-Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op
-Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow
-Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn
-Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef
-Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers
-Wno-narrowing -Wno-varargs -Wstack-protector -fcheck-new -fstack-protector
-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla
)


add_subdirectory(server ${CMAKE_CURRENT_BINARY_DIR}/server_interface)
#add_subdirectory(task_2 ${CMAKE_CURRENT_BINARY_DIR}/task_2)

