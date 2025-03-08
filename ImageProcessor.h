#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>

void saveImage(const cv::Mat& image, const std::string& filename);
std::vector<std::vector<int>> extractAppleGrid(const cv::Mat& image, const std::vector<cv::Mat>& templates, int rows = 17, int cols = 10);

#endif // IMAGE_PROCESSOR_H