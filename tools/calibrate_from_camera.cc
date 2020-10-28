#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "calibration/calibrator.h"
#include "calibration/checker_board.h"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "popl.hpp"
#include "spdlog/spdlog.h"

using camtk::CalibrationResult;
using camtk::Calibrator;
using camtk::CheckerBoard;

std::unique_ptr<cv::VideoCapture> ConnectCamera(int id) {
  auto video = std::make_unique<cv::VideoCapture>(id, cv::CAP_ANY);

  if (!video->isOpened()) {
    spdlog::critical("Cannot connect to the camera.");
    std::abort();
  }

  // TODO (breakds): make this more flexible for all cameras.
  //
  // In this case have to specify the MJPEG format to access 1280x720@30fps of
  // this camera:
  // https://www.amazon.com/gp/product/B071DDB1JY/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1
  video->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
  video->set(cv::CAP_PROP_FRAME_WIDTH, 1080.0);
  video->set(cv::CAP_PROP_FRAME_HEIGHT, 720.0);

  return video;
}

int main(int argc, char** argv) {
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] %^[%L] %v%$");

  popl::OptionParser parser("Options");
  auto width  = parser.add<popl::Value<int>>("w", "width",
                                            "The number of grid points on each row", 14);
  auto height = parser.add<popl::Value<int>>(
      "h", "height", "The number of grid points on each column", 9);
  auto square_side = parser.add<popl::Value<double>>(
      "q", "square",
      "The physical length of the side of the checker board square (in meters).", 0.0178);
  parser.parse(argc, argv);

  // By default use the first camera, id = 0.
  std::unique_ptr<cv::VideoCapture> video = ConnectCamera(0);

  Calibrator calibrator(
      CheckerBoard(height->value(), width->value(), square_side->value()));

  cv::Mat frame;

  while (video->read(frame)) {
    cv::imshow("Frame", frame);

    int key = cv::waitKey(5);

    if (key == 'q') {
      break;
    }

    if (key == 'c') {
      if (!calibrator.AddImage(frame, true)) {
        spdlog::warn("Failed to extract grid points from a frame!");
      } else {
        spdlog::info("Added one more frame. Has {} views now.", calibrator.num_views());
        CalibrationResult result = calibrator.RunCalibration();
        spdlog::info("------------------------------------------------------------");
        spdlog::info("The cost is {}", result.cost);
        spdlog::info("fx = {}, fy = {}, cx = {}, cy = {}", result.fx, result.fy,
                     result.cx, result.cy);
        spdlog::info("k1 = {}, k2 = {}, k3 = {}, k4 = {}", result.k1, result.k2,
                     result.k3, result.k4);
        spdlog::info("p1 = {}, p2 = {}", result.p1, result.p2);
      }
    }
  }

  return 0;
}
