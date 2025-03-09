#include "ScreenCapture.h"
#include "ImageProcessor.h"
#include "AppleGameSolver.h"
#include "MouseController.h"
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <thread>
#include <Windows.h>

int main() {
    // 게임 창 포커스 설정 (게임 창 제목 가정: "Apple Game")
    HWND hwnd = FindWindow(NULL, L"Apple Game");
    if (hwnd == NULL) {
        std::cerr << "Could not find game window. Ensure the game is running.\n";
    }
    else {
        SetForegroundWindow(hwnd);
        std::cout << "Game window focused.\n";
    }

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

    std::cout << "Program started. Press ESC to quit at any time.\n";
    bool exitRequested = false; // 종료 플래그

    while (!exitRequested) {
        // ESC 키 입력 감지 (비동기적으로)
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            std::cout << "ESC key detected. Waiting for release...\n";
            // 키가 해제될 때까지 대기
            while (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            std::cout << "ESC key released. Terminating program...\n";
            exitRequested = true;
            continue; // 루프를 빠져나가지 않고 다음 반복으로
        }

        // 화면 캡처
        cv::Mat capturedImage = captureScreen(540, 300, 800, 475);
        if (capturedImage.empty()) {
            std::cerr << "Failed to capture screen" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms 대기
            continue;
        }

        try {
            // 사과 배열 추출 (10행 x 17열)
            std::vector<std::vector<int>> appleGrid = extractAppleGrid(capturedImage, templates, 10, 17);

            // 사과 묶음 찾기
            std::vector<std::pair<int, int>> bundle = findAppleBundle(appleGrid);

            // 묶음에 속한 좌표를 set에 저장 (출력용)
            std::set<std::pair<int, int>> bundleSet(bundle.begin(), bundle.end());

            // 사과 배열 출력
            std::cout << "Current Apple Grid with Bundle Marked:" << std::endl;
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 17; ++j) {
                    int value = appleGrid[i][j];
                    std::cout << (value != -1 ? value : 0);
                    if (bundleSet.count({ i, j })) std::cout << ".";
                    std::cout << " ";
                }
                std::cout << std::endl;
            }

            // 묶음 정보 출력 및 마우스 제어
            if (!bundle.empty()) {
                std::cout << "Found a bundle: ";
                for (const auto& p : bundle) {
                    std::cout << "(" << p.first << "," << p.second << ") ";
                }
                std::cout << std::endl;

                // 마우스로 사과 묶음 선택
                int screenX = 540;          // 캡처 시작 X
                int screenY = 300;          // 캡처 시작 Y
                int gridWidth = 800 / 17;   // 셀 너비
                int gridHeight = 475 / 10;  // 셀 높이
                MouseController::selectAppleBundle(bundle, screenX, screenY, gridWidth, gridHeight);
            }
            else {
                std::cout << "No bundle found." << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        // 100ms 대기 (1초 → 100ms로 줄임)
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
    }

    return 0;
}