#include "AppleGameSolver.h"

std::vector<std::pair<int, int>> findAppleBundle(std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    // 재귀 DFS 함수 정의
    auto dfs = [&](auto&& self, int x, int y, int sum, std::vector<std::pair<int, int>>& path) -> bool {
        if (sum == 10) { // 합이 10이면 성공
            return true;
        }
        if (sum > 10 || x < 0 || x >= rows || y < 0 || y >= cols || visited[x][y] || grid[x][y] == -1) {
            return false; // 조건에 맞지 않으면 실패
        }

        visited[x][y] = true;
        path.push_back({ x, y });
        for (const auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (self(self, nx, ny, sum + grid[x][y], path)) {
                return true;
            }
        }
        path.pop_back();
        visited[x][y] = false;
        return false;
        };

    // 그리드에서 사과 묶음 탐색
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] != -1) {
                std::vector<std::pair<int, int>> path;
                if (dfs(dfs, i, j, 0, path)) {
                    // 찾은 묶음을 -1로 표시
                    for (const auto& p : path) {
                        grid[p.first][p.second] = -1;
                    }
                    return path;
                }
            }
        }
    }
    return {}; // 묶음이 없으면 빈 벡터 반환
}