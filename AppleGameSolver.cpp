#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>

std::vector<std::pair<int, int>> findAppleBundle(const std::vector<std::vector<int>>& grid) {
    size_t rows = grid.size();
    if (rows == 0) return {};
    size_t cols = grid[0].size();

    // 7, 8, 9의 빈도 계산
    std::unordered_map<int, int> frequency;
    for (const auto& row : grid) {
        for (int num : row) {
            if (num == 7 || num == 8 || num == 9) {
                frequency[num]++;
            }
        }
    }

    // 누적 합 배열 계산
    std::vector<std::vector<int>> prefix(rows + 1, std::vector<int>(cols + 1, 0));
    for (size_t i = 1; i <= rows; ++i) {
        for (size_t j = 1; j <= cols; ++j) {
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1] + grid[i - 1][j - 1];
        }
    }

    std::vector<int> bestBundle;
    std::vector<std::pair<int, int>> bestCoords;
    int bestScore = -1000000; // 매우 낮은 초기값으로 어떤 묶음이든 선택 가능

    // 부분 격자 합 계산 함수
    auto getSum = [&](size_t r1, size_t c1, size_t r2, size_t c2) {
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
        };

    // 응집도 계산 함수
    auto calculateCohesion = [&](const std::vector<std::pair<int, int>>& highValuePositions) {
        int cohesionScore = 0;
        for (size_t i = 0; i < highValuePositions.size(); ++i) {
            for (size_t j = i + 1; j < highValuePositions.size(); ++j) {
                int dr = std::abs(highValuePositions[i].first - highValuePositions[j].first);
                int dc = std::abs(highValuePositions[i].second - highValuePositions[j].second);
                if (dr + dc == 1) { // 맨해튼 거리가 1인 경우 인접
                    cohesionScore += 1000; // 인접한 쌍마다 1000점 추가
                }
            }
        }
        return cohesionScore;
        };

    // 묶음 점수 계산 함수
    auto calculateScore = [&](const std::vector<int>& bundle, const std::vector<std::pair<int, int>>& positions) {
        int score = 0;
        bool hasNineOne = (bundle.size() == 2 && bundle[0] == 1 && bundle[1] == 9);
        bool hasOne = false;
        bool hasTwo = false;
        int highValueNum = 0; // 7, 8, 9 중 포함된 숫자 (하나만 허용)
        std::vector<std::pair<int, int>> highValuePositions;

        // 묶음 분석
        for (size_t idx = 0; idx < bundle.size(); ++idx) {
            int num = bundle[idx];
            if (num == 1) hasOne = true;
            if (num == 2) hasTwo = true;
            if (num == 7 || num == 8 || num == 9) {
                if (highValueNum == 0) {
                    highValueNum = num; // 첫 번째 7, 8, 9만 기록
                    score += frequency[num] * 10; // 빈도에 비례한 점수
                    highValuePositions.push_back(positions[idx]);
                }
                else if (highValueNum != num) {
                    return -1000000; // 7, 8, 9가 두 개 이상 포함되면 무효화
                }
                else {
                    highValuePositions.push_back(positions[idx]); // 동일한 높은 숫자는 위치 추가
                }
            }
        }

        // 기본 점수: 묶음 크기에 비례
        score += bundle.size() * 50;

        // 우선순위 1: [9, 1] 묶음
        if (hasNineOne) score += 10000;

        // 7, 8, 9 포함 보너스
        if (highValueNum != 0) score += 1000;

        // 응집도 점수 추가
        if (highValuePositions.size() > 1) {
            score += calculateCohesion(highValuePositions);
        }

        // 패널티: 1과 2 포함
        if (hasOne) score -= 500;
        if (hasTwo) score -= 300;

        return score;
        };

    // 모든 가능한 직사각형 탐색
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

                        // 점수가 더 높거나, 같을 때 사전순으로 작은 묶음 선택
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