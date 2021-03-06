#pragma once

#include <opencv2/core/core.hpp>

#include "Kernel.hpp"

namespace terrain
{

class ThermalErosion : public Kernel 
{
  // using Point = cv::Point2i;

  public:
    ThermalErosion();

    ThermalErosion(KernelType kernel_type);

    ~ThermalErosion();

    void operation(cv::Mat& img, Point center, std::vector<Point> neighbours);

    void apply(cv::Mat& img, int iterations=1);

    void moveMaterial(cv::Mat& img, Point move_from, Point move_to, float amount);

  private:
    float talus_angle = 0.0078; // T
    float magnitude = 0.5; // c
    // KernelType kernel_type;
};

} // namespace terrain