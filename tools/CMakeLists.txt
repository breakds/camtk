add_executable(calibrate_from_video calibrate_from_video.cc)

set_target_properties(calibrate_from_video PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}")

target_link_libraries(
    calibrate_from_video
    opencv_core
    opencv_videoio
    checker_board
    calibration_result
    calibrator)
