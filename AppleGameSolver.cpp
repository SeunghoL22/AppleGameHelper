#include "AppleGameSolver.h"
#include <vector>
#include <utility>

std::vector<std::pair<int, int>> findAppleBundle(std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // 1. ���� ���� ���� Ž��
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] <= 0) continue; // -1(�ν� ����) �Ǵ� 0(�ʷϻ� ����) �ǳʶ�
            int sum = 0;
            std::vector<std::pair<int, int>> path;
            for (int k = j; k < cols; ++k) {
                if (grid[i][k] <= 0) break; // -1 �Ǵ� 0���� �ߴ�
                sum += grid[i][k];
                path.push_back({ i, k });
                if (sum == 10) {
                    return path; // �� ���� �������� �ʰ� ��ǥ�� ��ȯ
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
            if (grid[i][j] <= 0) continue; // -1(�ν� ����) �Ǵ� 0(�ʷϻ� ����) �ǳʶ�
            int sum = 0;
            std::vector<std::pair<int, int>> path;
            for (int k = i; k < rows; ++k) {
                if (grid[k][j] <= 0) break; // -1 �Ǵ� 0���� �ߴ�
                sum += grid[k][j];
                path.push_back({ k, j });
                if (sum == 10) {
                    return path; // �� ���� �������� �ʰ� ��ǥ�� ��ȯ
                }
                else if (sum > 10) {
                    break; // ���� 10�� �ʰ��ϸ� �ߴ�
                }
            }
        }
    }

    return {}; // ������ ������ �� ���� ��ȯ
}