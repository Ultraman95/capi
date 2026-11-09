set(NAME market_data_subscriber)
add_executable(${NAME} main.cpp)
target_link_libraries(${NAME} PRIVATE ccapi::ccapi_core)
