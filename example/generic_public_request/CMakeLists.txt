set(NAME generic_public_request)
project(${NAME})
add_executable(${NAME} main.cpp)
target_link_libraries(${NAME} PRIVATE ccapi_core)
