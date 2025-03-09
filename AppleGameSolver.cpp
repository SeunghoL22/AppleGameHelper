#include "AppleGameSolver.h"
#include <vector>
#include <utility>
#include <algorithm>

std::vector<std::pair<int, int>> findAppleBundle(const std::vector<std::vector<int>>& grid) {
    size_t rows = grid.size();
    if (rows == 0) return {};
    size_t cols = grid[0].size();

    // ���� �� �迭 ���
    std::vector<std::vector<int>> prefix(rows + 1, std::vector<int>(cols + 1, 0));
    for (size_t i = 1; i <= rows; ++i) {
        for (size_t j = 1; j <= cols; ++j) {
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1] + grid[i - 1][j - 1];
        }
    }

    std::vector<int> bestBundle;
    std::vector<std::pair<int, int>> bestCoords;
    int bestScore = -1;  // �ְ� ���� ����

    // �κ� ���� �� ��� �Լ�
    auto getSum = [&](size_t r1, size_t c1, size_t r2, size_t c2) {
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
        };

    // ������ 7, 8, 9�� ���ԵǾ� �ִ��� Ȯ���ϰ� ���� �ο�
    auto calculateScore = [&](const std::vector<int>& bundle, size_t r1, size_t c1, size_t r2, size_t c2) {
        int score = 0;
        bool hasNineOne = (bundle.size() == 2 && bundle[0] == 1 && bundle[1] == 9);
        bool hasSevenEightNine = false;

        // ū ����(7, 8, 9) ���� ���� Ȯ��
        for (int num : bundle) {
            if (num == 7 || num == 8 || num == 9) {
                hasSevenEightNine = true;
                score += num * 10;  // ���� ũ�⿡ ����� ���� �ο�
            }
        }

        // �켱���� 1: [9, 1] ����
        if (hasNineOne) {
            return 10000;  // �������� �ְ� ����
        }

        // �켱���� 2: 7, 8, 9 ����
        if (hasSevenEightNine) {
            score += 1000;  // �⺻ ����
        }

        // �켱���� 3: ���� ũ��
        score += bundle.size() * 100;  // ū ������ ����ġ

        // �켱���� 4: �߽ɺ� ��ġ (2~7��, 5~11��)
        bool isCentral = (r1 >= 2 && r2 <= 7 && c1 >= 5 && c2 <= 11);  // 10��17�� ���� �߽ɺ�
        if (isCentral) {
            score += 50;  // �߽ɺ� ���ʽ�
        }

        return score;
        };

    // ��� ������ ���簢�� Ž��
    for (size_t r1 = 0; r1 < rows; ++r1) {
        for (size_t r2 = r1; r2 < rows; ++r2) {
            for (size_t c1 = 0; c1 < cols; ++c1) {
                for (size_t c2 = c1; c2 < cols; ++c2) {
                    if (getSum(r1, c1, r2, c2) == 10) {
                        std::vector<int> bundle;
                        for (size_t i = r1; i <= r2; ++i) {
                            for (size_t j = c1; j <= c2; ++j) {
                                if (grid[i][j] != 0) {
                                    bundle.push_back(grid[i][j]);
                                }
                            }
                        }
                        std::sort(bundle.begin(), bundle.end());

                        int score = calculateScore(bundle, r1, c1, r2, c2);

                        // ������ �� ���ų�, ������ ���� �� ���������� �� ���� ���� ����
                        if (score > bestScore || (score == bestScore && bundle < bestBundle)) {
                            bestScore = score;
                            bestBundle = bundle;
                            bestCoords.clear();
                            for (size_t i = r1; i <= r2; ++i) {
                                for (size_t j = c1; j <= c2; ++j) {
                                    if (grid[i][j] != 0) {
                                        bestCoords.push_back({ static_cast<int>(i), static_cast<int>(j) });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return bestCoords;
}