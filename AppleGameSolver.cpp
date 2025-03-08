#include "AppleGameSolver.h"

std::vector<std::pair<int, int>> findAppleBundle(std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    // ��� DFS �Լ� ����
    auto dfs = [&](auto&& self, int x, int y, int sum, std::vector<std::pair<int, int>>& path) -> bool {
        if (sum == 10) { // ���� 10�̸� ����
            return true;
        }
        if (sum > 10 || x < 0 || x >= rows || y < 0 || y >= cols || visited[x][y] || grid[x][y] == -1) {
            return false; // ���ǿ� ���� ������ ����
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

    // �׸��忡�� ��� ���� Ž��
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] != -1) {
                std::vector<std::pair<int, int>> path;
                if (dfs(dfs, i, j, 0, path)) {
                    // ã�� ������ -1�� ǥ��
                    for (const auto& p : path) {
                        grid[p.first][p.second] = -1;
                    }
                    return path;
                }
            }
        }
    }
    return {}; // ������ ������ �� ���� ��ȯ
}