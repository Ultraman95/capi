set(NAME reduce_build_time)
project(${NAME})
add_executable(${NAME} main.cpp my_session.cpp)
target_link_libraries(${NAME} PRIVATE ccapi_core)
