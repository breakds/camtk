#pragma once

#include <vector>

#include "opencv2/core.hpp"

namespace camtk {

// This describes the dimensions of the checker board to be used for
// the calibration.
class CheckerBoard {
 public:
  CheckerBoard(int height, int width, double square_side)
      : height_(height), width_(width), square_side_(square_side) {}

  cv::Size shape() const { return cv::Size(width_, height_); }

  size_t num_grid_points() const { return height_ * width_; }

  // This generate a matrix of grid points in physical world
  // coordinate system. Will be used in the calibration as the object
  // points.
  std::vector<cv::Point3f> GenerateGridPoints() const;

 private:
  // The height and width are for grid points (a.k.a. corners). As
  // an example, if you are using a checker board with 13 x 8
  // squares, you should put height = 9 and width = 14.
  int height_;
  int width_;

  // This describes the physical size of the squares on the checker
  // board. Unit in meters.
  double square_side_;
};

}  // namespace camtk
