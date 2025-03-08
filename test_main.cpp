#include "ScreenCapture.h"
#include "ImageProcessor.h"
#include "AppleGameSolver.h"
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <thread>

int main() {
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

    while (true) {
        // ȭ�� ĸó
        cv::Mat capturedImage = captureScreen(540, 300, 800, 475);

        // ��� �迭 ���� (10�� x 17��)
        std::vector<std::vector<int>> appleGrid = extractAppleGrid(capturedImage, templates, 10, 17);

        // ��� ���� ã��
        std::vector<std::pair<int, int>> bundle = findAppleBundle(appleGrid);

        // ������ ���� ��ǥ�� set�� ���� (���� Ȯ�ο�)
        std::set<std::pair<int, int>> bundleSet(bundle.begin(), bundle.end());

        // ��� �迭 ��� (������ ���� ����� '.' �߰�)
        std::cout << "Current Apple Grid with Bundle Marked:" << std::endl;
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 17; ++j) {
                int value = appleGrid[i][j];
                if (value == 0) {
                    std::cout << "0 "; // �� ���� 0���� ǥ��
                }
                else if (value != -1) {
                    std::cout << value; // ���� ���
                    if (bundleSet.count({ i, j })) {
                        std::cout << "."; // found bundle�̸� . �߰�
                    }
                    std::cout << " ";
                }
                else {
                    std::cout << "X "; // �ν� ���� �� X (�ʿ� �� ����)
                }
            }
            std::cout << std::endl;
        }

        // ���� ���� ���
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

        // 1�� ���
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}