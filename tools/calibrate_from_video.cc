#include <cmath>
#include <string>
#include <vector>

#include "calibration/calibrator.h"
#include "calibration/checker_board.h"
#include "opencv2/videoio.hpp"
#include "popl.hpp"
#include "spdlog/spdlog.h"

using camtk::CalibrationResult;
using camtk::Calibrator;
using camtk::CheckerBoard;

int main(int argc, char** argv) {
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] %^[%L] %v%$");

  popl::OptionParser parser("Options");
  auto video_path =
      parser.add<popl::Value<std::string>>("f", "video", "The path to the input video");
  auto skip_frame = parser.add<popl::Value<int>>(
      "s", "skip-frame", "Sample frames by skipping this number of frames", 1);
  auto width  = parser.add<popl::Value<int>>("w", "width",
                                            "The number of grid points on each row", 14);
  auto height = parser.add<popl::Value<int>>(
      "h", "height", "The number of grid points on each column", 9);
  auto square_side = parser.add<popl::Value<double>>(
      "q", "square",
      "The physical length of the side of the checker board square (in meters).", 0.0178);
  auto visualize = parser.add<popl::Switch>(
      "v", "visualize",
      "Enable to show extracted grid points for every processed frame.");
  parser.parse(argc, argv);

  if (!video_path->is_set()) {
    spdlog::critical("Please specify the path to the input video with --video.");
    return -1;
  }

  cv::VideoCapture video = cv::VideoCapture(video_path->value(), cv::CAP_FFMPEG);

  if (!video.isOpened()) {
    spdlog::critical("Failed to open {}", video_path->value());
    return -1;
  } else {
    spdlog::info("Successfully opened video at {}.", video_path->value());
  }

  Calibrator calibrator(
      CheckerBoard(height->value(), width->value(), square_side->value()));

  cv::Mat frame;

  spdlog::info("Skip frame = {}", skip_frame->value());

  size_t count = 0;
  while (video.read(frame)) {
    ++count;
    if (count % skip_frame->value() != 0) {
      continue;
    }
    if (!calibrator.AddImage(frame, visualize->value())) {
      spdlog::warn("Failed to extract grid points from a frame!");
    } else {
      spdlog::info("Added one more frame. Has {} views now.", calibrator.num_views());
    }
  }

  CalibrationResult result = calibrator.RunCalibration();

  if (!result.success) {
    spdlog::critical("Calibration failed.");
    return -1;
  }

  spdlog::info("The cost is {}", result.cost);
  spdlog::info("fx = {}, fy = {}, cx = {}, cy = {}", result.fx, result.fy, result.cx,
               result.cy);
  spdlog::info("k1 = {}, k2 = {}, k3 = {}, k4 = {}", result.k1, result.k2, result.k3,
               result.k4);
  spdlog::info("p1 = {}, p2 = {}", result.p1, result.p2);

  return 0;
}
