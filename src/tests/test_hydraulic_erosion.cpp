#include <opencv2/opencv.hpp>

#include "HydraulicErosion.hpp"
#include "DiamondSquare.hpp"
#include "Voronoi.hpp"
#include "common.hpp"

using namespace cv;
using namespace std;

float x, y;
bool clicked = false;

void imshow2(std::string windowName, cv::Mat img)
{
  namedWindow(windowName, cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow(windowName, 1000, 1000);
  cv::imshow(windowName, img);
}

int main(int argc, char** argv)
{
 // Get coeff for Voronoi
  std::vector<float> coeffs;
  for (int i = 1; i < argc - 1; ++i)
    coeffs.push_back(std::stoi(argv[i]));

  int iterations = std::stoi(argv[argc-1]);

  int n = pow(2, 9) + 1; // 513 x 513 image
  int rows, cols; rows = cols = n;
  float persistence = 0.5; // Higher = more jagged terrain
  
  DiamondSquare diamondSquare;
  auto heightmap_ds = diamondSquare.generate(n, persistence);

  int n_points = 50;
  Voronoi vrn = Voronoi(rows, cols, coeffs, n_points);
  auto heightmap_vrn = vrn.generate();

  // cv::Mat combined = combine({ heightmap_ds, heightmap_vrn }, { 1 / 3, 2 / 3 });
  cv::Mat combined;
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);
  cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

  HydraulicErosion hydraulic(MOORE, rows, cols);
  cv::Mat eroded;
  combined.copyTo(eroded);
  hydraulic.apply(eroded, iterations);
  // cv::normalize(eroded, eroded, 1, 0, cv::NORM_MINMAX);

  imshow2("Base", combined);
  imshow2("Eroded", eroded);
  imshow2("heightmap", hydraulic.heightmap);
  //normalize(hydraulic.watermap*255, hydraulic.watermap, 1, 0, cv::NORM_MINMAX);
  //normalize(hydraulic.sedimentmap*255, hydraulic.sedimentmap, 1, 0, cv::NORM_MINMAX);
  imshow2("watermap", hydraulic.watermap);
  imshow2("sedmap", hydraulic.sedimentmap);
  // cout << hydraulic.watermap;
  // cout << hydraulic.sedimentmap;
 
  cv::waitKey(0);

  return 0;
}