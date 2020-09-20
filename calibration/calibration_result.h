#pragma once

#include "opencv2/core.hpp"

namespace camtk {

struct CalibrationResult {
  bool success = false;

  // The RMS cost of the reprojection.
  double cost = 0.0;

  // Focal point related parameters. Theoretically fx and fy should
  // be equal for standard lenses, and in practical the difference
  // should be very small.
  double fx = 0.0;
  double fy = 0.0;

  // The translation parameters. Note that the unit is in pixels,
  // which means that if you see result far beyond your image's
  // size, something must go wrong.
  double cx = 0.0;
  double cy = 0.0;

  // The following parameters are for distortions. Please refer to
  // the OpenCV documentation where the concepts are detailed.
  double k1 = 0.0;
  double k2 = 0.0;
  double k3 = 0.0;
  double k4 = 0.0;
  double p1 = 0.0;
  double p2 = 0.0;

  // This factory function translate the OpenCV calibrateCamera
  // result into the CalibrationResult format.
  static CalibrationResult FromParameterMatrices(cv::Mat camera_matrix,
                                                 cv::Mat distortion_array, double cost_);
};

}  // namespace camtk
