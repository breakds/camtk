cmake_minimum_required(VERSION 3.10)

project(CamTK VERSION 0.8.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR})
add_compile_options(-Wall -Werror)

find_package(OpenCV 4.0 REQUIRED
    COMPONENTS core imgcodecs videoio features2d calib3d highgui)

if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV 4 not found")
endif()
message(STATUS "Use OpenCV ${OpenCV_VERSION}")

add_subdirectory(calibration)
add_subdirectory(tools)

# +-------------------+
# | Installation      |
# +-------------------+

install(DIRECTORY calibration/
    DESTINATION include/calibration
    FILES_MATCHING PATTERN "*.h")

install(TARGETS checker_board calibration_result calibrator
    DESTINATION lib/)
