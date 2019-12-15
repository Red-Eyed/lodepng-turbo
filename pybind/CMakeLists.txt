cmake_minimum_required(VERSION 3.10)
project(example)

include_directories("./")
include_directories("~/miniconda3/envs/gen3/include/python3.7m/")
include_directories("../")
include_directories("pybind11/include")

link_directories("../")
message(${CMAKE_SOURCE_DIR})

add_subdirectory(pybind11)

pybind11_add_module(lodepng SHARED lodepng-turbo.cpp)
target_link_libraries(lodepng PRIVATE lodepngturbo pybind11::module)
# set_target_properties(lodepng PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
#                                          SUFFIX "${PYTHON_MODULE_EXTENSION}")