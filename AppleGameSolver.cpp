#include "AppleGameSolver.h"
#include <vector>
#include <utility>

std::vector<std::pair<int, int>> findAppleBundle(std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // 1. 가로 방향 묶음 탐색
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] <= 0) continue; // -1(인식 실패) 또는 0(초록색 격자) 건너뜀
            int sum = 0;
            std::vector<std::pair<int, int>> path;
            for (int k = j; k < cols; ++k) {
                if (grid[i][k] <= 0) break; // -1 또는 0에서 중단
                sum += grid[i][k];
                path.push_back({ i, k });
                if (sum == 10) {
                    return path; // 셀 값을 변경하지 않고 좌표만 반환
                }
                else if (sum > 10) {
                    break; // 합이 10을 초과하면 중단
                }
            }
        }
    }

    // 2. 세로 방향 묶음 탐색
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            if (grid[i][j] <= 0) continue; // -1(인식 실패) 또는 0(초록색 격자) 건너뜀
            int sum = 0;
            std::vector<std::pair<int, int>> path;
            for (int k = i; k < rows; ++k) {
                if (grid[k][j] <= 0) break; // -1 또는 0에서 중단
                sum += grid[k][j];
                path.push_back({ k, j });
                if (sum == 10) {
                    return path; // 셀 값을 변경하지 않고 좌표만 반환
                }
                else if (sum > 10) {
                    break; // 합이 10을 초과하면 중단
                }
            }
        }
    }

    return {}; // 묶음이 없으면 빈 벡터 반환
}