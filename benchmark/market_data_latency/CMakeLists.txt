set(NAME market_data_latency)
project(${NAME})
add_executable(${NAME} main.cpp)
target_link_libraries(${NAME} PRIVATE ccapi_core hdr_histogram_static)
