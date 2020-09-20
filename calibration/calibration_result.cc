#include "calibration/calibration_result.h"

namespace camtk {

CalibrationResult CalibrationResult::FromParameterMatrices(cv::Mat camera_matrix,
                                                           cv::Mat distortion_array,
                                                           double cost_) {
  CalibrationResult result;

  result.success = true;
  result.cost    = cost_;

  result.fx = camera_matrix.at<double>(0, 0);
  result.fy = camera_matrix.at<double>(1, 1);
  result.cx = camera_matrix.at<double>(0, 2);
  result.cy = camera_matrix.at<double>(1, 2);

  result.k1 = distortion_array.at<double>(0);
  result.k2 = distortion_array.at<double>(1);
  result.p1 = distortion_array.at<double>(2);
  result.p2 = distortion_array.at<double>(3);
  result.k3 = distortion_array.at<double>(4);
  result.k4 = distortion_array.at<double>(5);

  return result;
}

}  // namespace camtk
