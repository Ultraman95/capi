# trade/ —— 下游消费者,直接链接已构建的 libccapi_core.a,不重编 lib,也不 install。
#
# 配套约定:在仓根有一个专门给 lib 的构建树 build/lib(独立于 example/benchmark):
#   cmake -S src -B build/lib -G Ninja              # 一次性配置
#   ninja -C build/lib                              # 改了 lib 源码就跑这条
# 之后 trade 这边:
#   cmake -S trade -B trade/build -G Ninja          # find_package(ccapi) 自动定位
#   ninja -C trade/build                            # 只重链 trade
#
# 如需指向自定义 lib 构建树,传 -Dccapi_DIR=<path-to-build-dir>。

cmake_minimum_required(VERSION 3.14)
project(trade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()
if(UNIX AND NOT APPLE)
  find_program(MOLD_PROGRAM mold)
  if(MOLD_PROGRAM)
    add_link_options("-fuse-ld=mold")
  endif()
endif()

# 默认从仓根 build/lib(src/ standalone build)定位 ccapi。
# 用户也可以传 -Dccapi_DIR=<path-to-build-dir> 自行指定。
find_package(ccapi REQUIRED PATHS ${CMAKE_SOURCE_DIR}/../build/lib)

set(CCAPI_TRADE_TARGETS market_data_subscriber)
foreach(t IN LISTS CCAPI_TRADE_TARGETS)
  add_subdirectory(${t})
endforeach()
