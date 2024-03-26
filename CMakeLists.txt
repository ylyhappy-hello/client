cmake_minimum_required(VERSION 3.10.0)

project(SocketClient VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDRD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/config)
file(COPY ${RESOURCES_DIR}/conf.toml DESTINATION ${CMAKE_BINARY_DIR}/config)
file(COPY ${RESOURCES_DIR}/yolov8n.onnx DESTINATION ${CMAKE_BINARY_DIR}/config)

include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)
include_directories(
  "src/"
)
file(GLOB_RECURSE SRCFILES CONFIGURE_DEPENDS "src/**.cpp")

add_executable(${PROJECT_NAME} ${SRCFILES})
add_custom_target(resources_target ALL
    SOURCES ${RESOURCES_DIR}/conf.toml
    DEPENDS ${RESOURCES_DIR}/conf.toml
)
add_dependencies(${PROJECT_NAME} resources_target)

target_link_libraries(${PROJECT_NAME} tomlplusplus::tomlplusplus
  "${CMAKE_SOURCE_DIR}/qiniuyun-c-sdk/lib/libqiniu.so"
  )
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" "${tomlplusplus_BINARY_DIR}/include" "qiniuyun-c-sdk/include" )


