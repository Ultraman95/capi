set(NAME test_order_latency)
project(${NAME})
add_executable(${NAME} main.cpp)
target_link_libraries(${NAME} PRIVATE ccapi_core)
