#include "calibration/checker_board.h"

namespace camtk {

std::vector<cv::Point3f> CheckerBoard::GenerateGridPoints() const {
  std::vector<cv::Point3f> matrix;
  matrix.reserve(num_grid_points());
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      matrix.emplace_back(j * square_side_, i * square_side_, 0.0);
    }
  }
  return matrix;
}

}  // namespace camtk
