#include "calibration/calibrator.h"

#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "spdlog/spdlog.h"

namespace camtk {

bool Calibrator::AddImage(cv::Mat image, bool visualize) {
  if (!image_points_.empty()) {
    if (image.size() != image_size_) {
      return false;
    }
  }

  image_size_ = image.size();

  // Create a gray scale version of the image. This will be used to refine
  // the corner points that we found from the detected checkerboard.
  cv::Mat gray_image;
  cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

  std::vector<cv::Point2f> corners;
  bool detected = cv::findChessboardCornersSB(image, checkerboard_.shape(), corners);

  // Reject if corners are not detected or not completely visible.
  if (!detected || corners.size() < checkerboard_.num_grid_points()) {
    return false;
  }

  cv::cornerSubPix(
      gray_image, corners, checkerboard_.shape(), cv::Size(-1, -1),
      cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 1e-5));

  cv::Mat canvas;
  image.copyTo(canvas);

  cv::drawChessboardCorners(canvas, checkerboard_.shape(), corners, true);

  if (visualize) {
    cv::imshow("Checkerboard", canvas);
    if (cv::waitKey(5) >= 0)
      ;
  }

  image_points_.emplace_back(std::move(corners));

  object_points_.emplace_back(object_points_.empty() ? checkerboard_.GenerateGridPoints()
                                                     : object_points_.front());
  return true;
}

CalibrationResult Calibrator::RunCalibration() const {
  if (image_points_.empty()) {
    spdlog::error("Abort calibration!");
    return CalibrationResult();
  }

  // Use the top right corner point as the fixed point.
  // int fixed_point_index         = checkerboard_.shape().width - 1;

  // Do not use fixed point for now.
  int fixed_point_index         = -1;
  cv::Mat camera_matrix         = cv::Mat::eye(3, 3, CV_64F);
  cv::Mat distortion_parameters = cv::Mat(8, 1, CV_64F);
  std::vector<cv::Mat> extrinsic_rotations;
  std::vector<cv::Mat> extrinsic_translations;
  std::vector<cv::Point3f> new_object_points(object_points_[0].size());
  double cost =
      cv::calibrateCameraRO(object_points_, image_points_, image_size_, fixed_point_index,
                            camera_matrix, distortion_parameters, extrinsic_rotations,
                            extrinsic_translations, new_object_points, cv::CALIB_USE_LU);

  return CalibrationResult::FromParameterMatrices(camera_matrix, distortion_parameters,
                                                  cost);
}

}  // namespace camtk
