#include "AppleGameSolver.h"
#include <vector>
#include <utility>
#include <algorithm>

std::vector<std::pair<int, int>> findAppleBundle(const std::vector<std::vector<int>>& grid) {
    size_t rows = grid.size();
    if (rows == 0) return {};
    size_t cols = grid[0].size();

    // 누적 합 배열 계산
    std::vector<std::vector<int>> prefix(rows + 1, std::vector<int>(cols + 1, 0));
    for (size_t i = 1; i <= rows; ++i) {
        for (size_t j = 1; j <= cols; ++j) {
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1] + grid[i - 1][j - 1];
        }
    }

    std::vector<int> bestBundle;
    std::vector<std::pair<int, int>> bestCoords;
    int bestScore = -1;  // 최고 점수 추적

    // 부분 격자 합 계산 함수
    auto getSum = [&](size_t r1, size_t c1, size_t r2, size_t c2) {
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
        };

    // 묶음에 7, 8, 9가 포함되어 있는지 확인하고 점수 부여
    auto calculateScore = [&](const std::vector<int>& bundle, size_t r1, size_t c1, size_t r2, size_t c2) {
        int score = 0;
        bool hasNineOne = (bundle.size() == 2 && bundle[0] == 1 && bundle[1] == 9);
        bool hasSevenEightNine = false;

        // 큰 숫자(7, 8, 9) 포함 여부 확인
        for (int num : bundle) {
            if (num == 7 || num == 8 || num == 9) {
                hasSevenEightNine = true;
                score += num * 10;  // 숫자 크기에 비례한 점수 부여
            }
        }

        // 우선순위 1: [9, 1] 묶음
        if (hasNineOne) {
            return 10000;  // 절대적인 최고 점수
        }

        // 우선순위 2: 7, 8, 9 포함
        if (hasSevenEightNine) {
            score += 1000;  // 기본 점수
        }

        // 우선순위 3: 묶음 크기
        score += bundle.size() * 100;  // 큰 묶음에 가중치

        // 우선순위 4: 중심부 위치 (2~7행, 5~11열)
        bool isCentral = (r1 >= 2 && r2 <= 7 && c1 >= 5 && c2 <= 11);  // 10×17에 맞춘 중심부
        if (isCentral) {
            score += 50;  // 중심부 보너스
        }

        return score;
        };

    // 모든 가능한 직사각형 탐색
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

                        // 점수가 더 높거나, 점수가 같을 때 사전순으로 더 작은 묶음 선택
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