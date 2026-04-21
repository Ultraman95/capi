set(NAME market_data_simple_subscription)
project(${NAME})
add_executable(${NAME} main.cpp)
target_link_libraries(${NAME} PRIVATE ccapi_core)
