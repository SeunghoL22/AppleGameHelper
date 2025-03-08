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
            //std::cerr << "템플릿 " << i + 1 << "이 비어 있음" << std::endl;
            continue;
        }
        if (roi.cols < templates[i].cols || roi.rows < templates[i].rows) {
            //std::cerr << "ROI(" << roi.cols << "x" << roi.rows << ")가 템플릿 " << i + 1
            //    << "(" << templates[i].cols << "x" << templates[i].rows << ")보다 작음" << std::endl;
            continue;
        }
        //std::cout << "ROI: " << roi.cols << "x" << roi.rows << ", 템플릿 " << i + 1 << ": "
        //    << templates[i].cols << "x" << templates[i].rows << std::endl;
        cv::Mat result;
        try {
            cv::matchTemplate(roi, templates[i], result, cv::TM_CCOEFF_NORMED);
            if (result.empty()) {
                std::cerr << "matchTemplate 결과가 비어 있음 (템플릿 " << i + 1 << ")" << std::endl;
                continue;
            }
            double minVal, maxValTemp;
            cv::minMaxLoc(result, &minVal, &maxValTemp);
            if (maxValTemp > maxVal && maxValTemp > 0.8) { // 임계값 0.65 → 0.8로 높임
                maxVal = maxValTemp;
                bestMatch = i + 1;
            }
        }
        catch (const cv::Exception& e) {
            std::cerr << "cv::Exception 발생: " << e.what() << std::endl;
            continue;
        }
    }
    return bestMatch != -1 ? bestMatch : -1;
}

std::vector<std::vector<int>> extractAppleGrid(const cv::Mat& image, const std::vector<cv::Mat>& templates, int rows, int cols) {
    // 그레이스케일 변환
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // 격자 크기 계산
    int gridWidth = image.cols / cols;  // 800 / 17 ≈ 47
    int gridHeight = image.rows / rows; // 475 / 10 ≈ 47

    // 10x17 격자 초기화 (-1로 채움)
    std::vector<std::vector<int>> appleGrid(rows, std::vector<int>(cols, -1));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // ROI 설정
            int base_x = j * gridWidth;
            int base_y = i * gridHeight;
            int adjustedWidth = (j == cols - 1) ? (image.cols - base_x) : gridWidth;
            int adjustedHeight = (i == rows - 1) ? (image.rows - base_y) : gridHeight;
            cv::Rect roiRect(base_x, base_y, adjustedWidth, adjustedHeight);

            // ROI가 이미지 범위를 벗어나지 않도록 조정
            roiRect &= cv::Rect(0, 0, image.cols, image.rows);
            if (roiRect.width <= 0 || roiRect.height <= 0) continue;

            cv::Mat roiColor = image(roiRect);
            cv::Mat roiGray;

            // 전처리: 흰색 격자 무늬 영향 줄이기 위해 블러링 적용
            cv::medianBlur(roiColor, roiColor, 3);
            cv::cvtColor(roiColor, roiGray, cv::COLOR_BGR2GRAY);

            // 초록색 및 빨간색 감지
            cv::Mat hsv;
            cv::cvtColor(roiColor, hsv, cv::COLOR_BGR2HSV);

            // 빨간색 마스크 (H: 0-10 또는 160-180)
            cv::Mat redMask1, redMask2;
            cv::inRange(hsv, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), redMask1);
            cv::inRange(hsv, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), redMask2);
            cv::Mat redMask;
            cv::addWeighted(redMask1, 1.0, redMask2, 1.0, 0.0, redMask);
            double redRatio = cv::countNonZero(redMask) / static_cast<double>(redMask.total());

            // 초록색 마스크
            cv::Mat greenMask;
            cv::inRange(hsv, cv::Scalar(50, 120, 100), cv::Scalar(70, 255, 255), greenMask);
            double greenRatio = cv::countNonZero(greenMask) / static_cast<double>(greenMask.total());

            // 결정 로직: 빨간색 비율이 낮으면 사과가 없는 것으로 간주
            if (redRatio < 0.15) { // 빨간색 비율이 15% 미만이면 초록색 배경으로 간주
                appleGrid[i][j] = 0;
                continue;
            }

            // 숫자 인식
            int number = recognizeNumber(roiGray, templates);
            appleGrid[i][j] = (number != -1) ? number : -1; // 실패 시 -1 유지
        }
    }
    return appleGrid;
}