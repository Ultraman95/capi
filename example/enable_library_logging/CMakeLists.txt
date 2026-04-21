set(NAME enable_library_logging)
project(${NAME})
# NOTE: This example demonstrates plugging a custom ccapi::Logger. Internal
# CCAPI_LOGGER_* statements in libccapi_core are compile-time switched; to
# actually see library-level traces, rebuild ccapi_core itself with:
#   cmake -B build -DCCAPI_LOG_LEVEL=TRACE
# (setting CCAPI_ENABLE_LOG_TRACE here alone would only affect this TU's own
# log calls, not the library's.)
add_executable(${NAME} main.cpp)
target_link_libraries(${NAME} PRIVATE ccapi_core)
