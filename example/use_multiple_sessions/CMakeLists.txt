set(NAME use_multiple_sessions)
project(${NAME})
add_executable(${NAME} main.cpp)
target_link_libraries(${NAME} PRIVATE ccapi_core)
