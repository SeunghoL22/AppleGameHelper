#include "ScreenCapture.h"
#include "ImageProcessor.h"
#include "AppleGameSolver.h"
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <thread>

int main() {
    cv::Mat capturedImage = captureScreen(540, 300, 800, 475); // 이미지 크기 예시

    std::vector<cv::Mat> templates;
    for (int i = 1; i <= 9; ++i) {
        std::string filename = "template_" + std::to_string(i) + ".png";
        cv::Mat temp = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        templates.push_back(temp);
    }

    // 10행 x 17열로 설정
    std::vector<std::vector<int>> appleGrid = extractAppleGrid(capturedImage, templates, 10, 17);

    // 출력: 행은 10, 열은 17
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 17; ++j) {
            std::cout << (appleGrid[i][j] != -1 ? appleGrid[i][j] : 0) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}