cmake_minimum_required(VERSION 3.30)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# Disable minimal build to enable all components
# idf_build_set_property(MINIMAL_BUILD ON)
project(hello_world)
