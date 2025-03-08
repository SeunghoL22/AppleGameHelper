#include "ScreenCapture.h"
#include "ImageProcessor.h"
#include "AppleGameSolver.h"
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <thread>

int main() {
    // 템플릿 이미지 로드
    std::vector<cv::Mat> templates;
    for (int i = 1; i <= 9; ++i) {
        std::string filename = "template_" + std::to_string(i) + ".png";
        cv::Mat temp = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        if (temp.empty()) {
            std::cerr << "Failed to load " << filename << std::endl;
            return -1;
        }
        templates.push_back(temp);
    }

    while (true) {
        // 화면 캡처
        cv::Mat capturedImage = captureScreen(540, 300, 800, 475);

        // 사과 배열 추출 (10행 x 17열)
        std::vector<std::vector<int>> appleGrid = extractAppleGrid(capturedImage, templates, 10, 17);

        // 사과 묶음 찾기
        std::vector<std::pair<int, int>> bundle = findAppleBundle(appleGrid);

        // 묶음에 속한 좌표를 set에 저장 (빠른 확인용)
        std::set<std::pair<int, int>> bundleSet(bundle.begin(), bundle.end());

        // 사과 배열 출력 (묶음에 속한 사과에 '.' 추가)
        std::cout << "Current Apple Grid with Bundle Marked:" << std::endl;
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 17; ++j) {
                int value = appleGrid[i][j];
                if (value == 0) {
                    std::cout << "0 "; // 빈 셀은 0으로 표시
                }
                else if (value != -1) {
                    std::cout << value; // 숫자 출력
                    if (bundleSet.count({ i, j })) {
                        std::cout << "."; // found bundle이면 . 추가
                    }
                    std::cout << " ";
                }
                else {
                    std::cout << "X "; // 인식 실패 시 X (필요 시 수정)
                }
            }
            std::cout << std::endl;
        }

        // 묶음 정보 출력
        if (!bundle.empty()) {
            std::cout << "Found a bundle: ";
            for (const auto& p : bundle) {
                std::cout << "(" << p.first << "," << p.second << ") ";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "No bundle found." << std::endl;
        }

        // 1초 대기
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}