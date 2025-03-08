#include "AppleGameSolver.h"
#include <vector>
#include <utility>

std::vector<std::pair<int, int>> findAppleBundle(std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // 1. ���� ���� ���� Ž��
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 0) continue; // 0�� �ǳʶ�
            if (grid[i][j] == -1) break;   // -1���� �ߴ�
            int sum = 0;
            std::vector<std::pair<int, int>> path;
            for (int k = j; k < cols; ++k) {
                if (grid[i][k] == -1) break; // -1���� �ߴ�
                if (grid[i][k] == 0) continue; // 0�� �ǳʶ�
                sum += grid[i][k];
                path.push_back({ i, k });
                if (sum == 10) {
                    return path; // ���� 10�̸� ��ǥ ��ȯ
                }
                else if (sum > 10) {
                    break; // ���� 10�� �ʰ��ϸ� �ߴ�
                }
            }
        }
    }

    // 2. ���� ���� ���� Ž��
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            if (grid[i][j] == 0) continue; // 0�� �ǳʶ�
            if (grid[i][j] == -1) break;   // -1���� �ߴ�
            int sum = 0;
            std::vector<std::pair<int, int>> path;
            for (int k = i; k < rows; ++k) {
                if (grid[k][j] == -1) break; // -1���� �ߴ�
                if (grid[k][j] == 0) continue; // 0�� �ǳʶ�
                sum += grid[k][j];
                path.push_back({ k, j });
                if (sum == 10) {
                    return path; // ���� 10�̸� ��ǥ ��ȯ
                }
                else if (sum > 10) {
                    break; // ���� 10�� �ʰ��ϸ� �ߴ�
                }
            }
        }
    }

    return {}; // ������ ������ �� ���� ��ȯ
}