#include "ImageProcessor.h"
#include <iostream>
#include <cmath>

void saveImage(const cv::Mat& image, const std::string& filename) {
    cv::imwrite(filename, image);
}

int recognizeNumber(const cv::Mat& roi, const std::vector<cv::Mat>& templates) {
    if (roi.empty()) {
        std::cerr << "ROI가 비어 있음" << std::endl;
        return -1;
    }
    double maxVal = 0;
    int bestMatch = -1;
    for (int i = 0; i < templates.size(); ++i) {
        if (templates[i].empty()) {
            std::cerr << "템플릿 " << i + 1 << "이 비어 있음" << std::endl;
            return -1;
        }
        if (roi.cols < templates[i].cols || roi.rows < templates[i].rows) {
            std::cerr << "ROI(" << roi.cols << "x" << roi.rows << ")가 템플릿 " << i + 1
                << "(" << templates[i].cols << "x" << templates[i].rows << ")보다 작음" << std::endl;
            return -1;
        }
        std::cout << "ROI: " << roi.cols << "x" << roi.rows << ", 템플릿 " << i + 1 << ": "
            << templates[i].cols << "x" << templates[i].rows << std::endl;
        cv::Mat result;
        try {
            cv::matchTemplate(roi, templates[i], result, cv::TM_CCOEFF_NORMED);
            if (result.empty()) {
                std::cerr << "matchTemplate 결과가 비어 있음 (템플릿 " << i + 1 << ")" << std::endl;
                return -1;
            }
            double minVal, maxValTemp;
            cv::minMaxLoc(result, &minVal, &maxValTemp);
            if (maxValTemp > maxVal && maxValTemp > 0.65) { // 임계값 0.65로 더 완화
                maxVal = maxValTemp;
                bestMatch = i + 1;
            }
        }
        catch (const cv::Exception& e) {
            std::cerr << "cv::Exception 발생: " << e.what() << std::endl;
            return -1;
        }
    }
    return bestMatch != -1 ? bestMatch : -1;
}

std::vector<std::vector<int>> extractAppleGrid(const cv::Mat& image, const std::vector<cv::Mat>& templates, int rows, int cols) {
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    int gridWidth = image.cols / cols;  // 800 / 17 ≈ 47
    int gridHeight = image.rows / rows; // 475 / 10 ≈ 47

    std::vector<std::vector<int>> appleGrid(rows, std::vector<int>(cols, -1));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int base_x = j * gridWidth;
            int base_y = i * gridHeight;
            int adjustedWidth = (j == cols - 1) ? (image.cols - base_x) : gridWidth;
            int adjustedHeight = (i == rows - 1) ? (image.rows - base_y) : gridHeight;
            cv::Rect roiRect(base_x, base_y, adjustedWidth, adjustedHeight);

            roiRect &= cv::Rect(0, 0, image.cols, image.rows);
            if (roiRect.width <= 0 || roiRect.height <= 0) continue;

            cv::Mat roi = gray(roiRect);
            int number = recognizeNumber(roi, templates);
            appleGrid[i][j] = number;
        }
    }
    return appleGrid;
}