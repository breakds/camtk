add_library(checker_board checker_board.cc)
target_link_libraries(checker_board opencv_core)

add_library(calibration_result calibration_result.cc)
target_link_libraries(calibration_result opencv_core)

add_library(calibrator calibrator.cc)
target_link_libraries(
    calibrator
    opencv_core opencv_highgui opencv_imgproc opencv_calib3d
    checker_board calibration_result)
