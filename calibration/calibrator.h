#pragma once

#include <vector>

#include "calibration/calibration_result.h"
#include "calibration/checker_board.h"
#include "opencv2/core.hpp"

namespace camtk {

class Calibrator {
 public:
  Calibrator(CheckerBoard&& checkerboard) : checkerboard_(std::move(checkerboard)) {}

  // Returns true if the grid points (corners) are detected and the poses of
  // the corners are different enough from the previous image. In this case,
  // the detected will be collected as image points.
  //
  // If visualize is set to true, the extracted grid points will be
  // visualized.
  bool AddImage(cv::Mat image, bool visualize = false);

  // Run the calibration with the added images, for which the grid
  // points (corners) are extracted in image_points_.
  //
  // Returns the calibration parameters for the camera.
  CalibrationResult RunCalibration() const;

  size_t num_views() const { return image_points_.size(); }

 private:
  CheckerBoard checkerboard_;

  std::vector<std::vector<cv::Point2f>> image_points_{};
  std::vector<std::vector<cv::Point3f>> object_points_{};
  cv::Size image_size_{0, 0};
};

}  // namespace camtk
