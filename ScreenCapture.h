#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <windows.h>

cv::Mat captureScreen(int x, int y, int width, int height);

#endif // SCREEN_CAPTURE_H