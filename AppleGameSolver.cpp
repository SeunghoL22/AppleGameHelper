#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>

std::vector<std::pair<int, int>> findAppleBundle(const std::vector<std::vector<int>>& grid) {
    size_t rows = grid.size();
    if (rows == 0) return {};
    size_t cols = grid[0].size();

    // 7, 8, 9�� �� ���
    std::unordered_map<int, int> frequency;
    for (const auto& row : grid) {
        for (int num : row) {
            if (num == 7 || num == 8 || num == 9) {
                frequency[num]++;
            }
        }
    }

    // ���� �� �迭 ���
    std::vector<std::vector<int>> prefix(rows + 1, std::vector<int>(cols + 1, 0));
    for (size_t i = 1; i <= rows; ++i) {
        for (size_t j = 1; j <= cols; ++j) {
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1] + grid[i - 1][j - 1];
        }
    }

    std::vector<int> bestBundle;
    std::vector<std::pair<int, int>> bestCoords;
    int bestScore = -1000000; // �ſ� ���� �ʱⰪ���� � �����̵� ���� ����

    // �κ� ���� �� ��� �Լ�
    auto getSum = [&](size_t r1, size_t c1, size_t r2, size_t c2) {
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
        };

    // ������ ��� �Լ�
    auto calculateCohesion = [&](const std::vector<std::pair<int, int>>& highValuePositions) {
        int cohesionScore = 0;
        for (size_t i = 0; i < highValuePositions.size(); ++i) {
            for (size_t j = i + 1; j < highValuePositions.size(); ++j) {
                int dr = std::abs(highValuePositions[i].first - highValuePositions[j].first);
                int dc = std::abs(highValuePositions[i].second - highValuePositions[j].second);
                if (dr + dc == 1) { // ����ư �Ÿ��� 1�� ��� ����
                    cohesionScore += 1000; // ������ �ָ��� 1000�� �߰�
                }
            }
        }
        return cohesionScore;
        };

    // ���� ���� ��� �Լ�
    auto calculateScore = [&](const std::vector<int>& bundle, const std::vector<std::pair<int, int>>& positions) {
        int score = 0;
        bool hasNineOne = (bundle.size() == 2 && bundle[0] == 1 && bundle[1] == 9);
        bool hasOne = false;
        bool hasTwo = false;
        int highValueNum = 0; // 7, 8, 9 �� ���Ե� ���� (�ϳ��� ���)
        std::vector<std::pair<int, int>> highValuePositions;

        // ���� �м�
        for (size_t idx = 0; idx < bundle.size(); ++idx) {
            int num = bundle[idx];
            if (num == 1) hasOne = true;
            if (num == 2) hasTwo = true;
            if (num == 7 || num == 8 || num == 9) {
                if (highValueNum == 0) {
                    highValueNum = num; // ù ��° 7, 8, 9�� ���
                    score += frequency[num] * 10; // �󵵿� ����� ����
                    highValuePositions.push_back(positions[idx]);
                }
                else if (highValueNum != num) {
                    return -1000000; // 7, 8, 9�� �� �� �̻� ���ԵǸ� ��ȿȭ
                }
                else {
                    highValuePositions.push_back(positions[idx]); // ������ ���� ���ڴ� ��ġ �߰�
                }
            }
        }

        // �⺻ ����: ���� ũ�⿡ ���
        score += bundle.size() * 50;

        // �켱���� 1: [9, 1] ����
        if (hasNineOne) score += 10000;

        // 7, 8, 9 ���� ���ʽ�
        if (highValueNum != 0) score += 1000;

        // ������ ���� �߰�
        if (highValuePositions.size() > 1) {
            score += calculateCohesion(highValuePositions);
        }

        // �г�Ƽ: 1�� 2 ����
        if (hasOne) score -= 500;
        if (hasTwo) score -= 300;

        return score;
        };

    // ��� ������ ���簢�� Ž��
    for (size_t r1 = 0; r1 < rows; ++r1) {
        for (size_t r2 = r1; r2 < rows; ++r2) {
            for (size_t c1 = 0; c1 < cols; ++c1) {
                for (size_t c2 = c1; c2 < cols; ++c2) {
                    if (getSum(r1, c1, r2, c2) == 10) {
                        std::vector<int> bundle;
                        std::vector<std::pair<int, int>> positions;
                        for (size_t i = r1; i <= r2; ++i) {
                            for (size_t j = c1; j <= c2; ++j) {
                                if (grid[i][j] != 0) {
                                    bundle.push_back(grid[i][j]);
                                    positions.push_back({ static_cast<int>(i), static_cast<int>(j) });
                                }
                            }
                        }
                        std::sort(bundle.begin(), bundle.end());

                        int score = calculateScore(bundle, positions);

                        // ������ �� ���ų�, ���� �� ���������� ���� ���� ����
                        if (score > bestScore || (score == bestScore && bundle < bestBundle)) {
                            bestScore = score;
                            bestBundle = bundle;
                            bestCoords = positions;
                        }
                    }
                }
            }
        }
    }

    return bestCoords;
}