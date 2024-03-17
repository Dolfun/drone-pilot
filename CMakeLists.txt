cmake_minimum_required(VERSION 3.21)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(drone-pilot)

idf_build_set_property(COMPILE_OPTIONS "-Wno-missing-field-initializers" APPEND)