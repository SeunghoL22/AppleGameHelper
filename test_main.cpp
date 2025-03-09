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
    // ���� â ��Ŀ�� ���� (���� â ���� ����: "Apple Game")
    HWND hwnd = FindWindow(NULL, L"Apple Game");
    if (hwnd == NULL) {
        std::cerr << "Could not find game window. Ensure the game is running.\n";
    }
    else {
        SetForegroundWindow(hwnd);
        std::cout << "Game window focused.\n";
    }

    // ���ø� �̹��� �ε�
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
    bool exitRequested = false; // ���� �÷���

    while (!exitRequested) {
        // ESC Ű �Է� ���� (�񵿱�������)
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            std::cout << "ESC key detected. Waiting for release...\n";
            // Ű�� ������ ������ ���
            while (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            std::cout << "ESC key released. Terminating program...\n";
            exitRequested = true;
            continue; // ������ ���������� �ʰ� ���� �ݺ�����
        }

        // ȭ�� ĸó
        cv::Mat capturedImage = captureScreen(540, 300, 800, 475);
        if (capturedImage.empty()) {
            std::cerr << "Failed to capture screen" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms ���
            continue;
        }

        try {
            // ��� �迭 ���� (10�� x 17��)
            std::vector<std::vector<int>> appleGrid = extractAppleGrid(capturedImage, templates, 10, 17);

            // ��� ���� ã��
            std::vector<std::pair<int, int>> bundle = findAppleBundle(appleGrid);

            // ������ ���� ��ǥ�� set�� ���� (��¿�)
            std::set<std::pair<int, int>> bundleSet(bundle.begin(), bundle.end());

            // ��� �迭 ���
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

            // ���� ���� ��� �� ���콺 ����
            if (!bundle.empty()) {
                std::cout << "Found a bundle: ";
                for (const auto& p : bundle) {
                    std::cout << "(" << p.first << "," << p.second << ") ";
                }
                std::cout << std::endl;

                // ���콺�� ��� ���� ����
                int screenX = 540;          // ĸó ���� X
                int screenY = 300;          // ĸó ���� Y
                int gridWidth = 800 / 17;   // �� �ʺ�
                int gridHeight = 475 / 10;  // �� ����
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

        // 100ms ��� (1�� �� 100ms�� ����)
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
    }

    return 0;
}