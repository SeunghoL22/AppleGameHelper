#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <queue>
#include <iostream>

using namespace std;

// 격자를 문자열로 변환하여 캐시 키로 사용
string gridToString(const vector<vector<int>>& grid) {
    string key;
    for (const auto& row : grid) {
        for (int num : row) {
            key += to_string(num) + ",";
        }
        key += ";";
    }
    return key;
}

// 사과의 개수 계산
int countApples(const vector<vector<int>>& grid) {
    int count = 0;
    int rows = grid.size();
    int cols = rows > 0 ? grid[0].size() : 0;
    cout << "countApples: Grid size = " << rows << "x" << cols << endl;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] != 0) count++;
        }
    }
    cout << "Apple count: " << count << endl;
    return count;
}

// 번들 내 9와 8의 응집성 계산
int calculateCohesion(const vector<vector<int>>& grid, const vector<pair<int, int>>& positions) {
    int cohesion = 0;
    int n = positions.size();
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));

    for (const auto& pos : positions) {
        visited[pos.first][pos.second] = true;
    }

    for (int i = 0; i < n; ++i) {
        int r = positions[i].first;
        int c = positions[i].second;
        int num = grid[r][c];
        if (num != 9 && num != 8) continue;

        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size() &&
                visited[nr][nc] && (grid[nr][nc] == 9 || grid[nr][nc] == 8)) {
                cohesion++;
            }
        }
    }

    return cohesion / 2;
}

// 높은 숫자(9, 8, 7) 제거 개수를 최대화 (사과 50개 초과에서 사용)
int maxHighValueRemoved(const vector<vector<int>>& grid, int depth, int maxDepth, unordered_map<string, int>& cache, int& maxPossible) {
    if (depth >= maxDepth) return 0;

    string gridKey = gridToString(grid);
    if (cache.count(gridKey)) return cache[gridKey];

    int rows = grid.size();
    int cols = grid[0].size();
    int maxRemoved = 0;

    vector<vector<int>> prefix(rows + 1, vector<int>(cols + 1, 0));
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1] + grid[i - 1][j - 1];
        }
    }

    auto getSum = [&](int r1, int c1, int r2, int c2) {
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
        };

    auto comp = [](const pair<int, vector<pair<int, int>>>& a, const pair<int, vector<pair<int, int>>>& b) {
        return a.first < b.first;
        };
    priority_queue<pair<int, vector<pair<int, int>>>, vector<pair<int, vector<pair<int, int>>>>, decltype(comp)> pq(comp);

    for (int r1 = 0; r1 < rows; ++r1) {
        for (int r2 = r1; r2 < rows; ++r2) {
            for (int c1 = 0; c1 < cols; ++c1) {
                for (int c2 = c1; c2 < cols; ++c2) {
                    if (getSum(r1, c1, r2, c2) == 10) {
                        vector<int> bundle;
                        vector<pair<int, int>> positions;
                        for (int i = r1; i <= r2; ++i) {
                            for (int j = c1; j <= c2; ++j) {
                                if (grid[i][j] != 0) {
                                    bundle.push_back(grid[i][j]);
                                    positions.push_back({ i, j });
                                }
                            }
                        }

                        int maxNum = 0;
                        int lowValuePenalty = 0;
                        for (int num : bundle) {
                            if (num == 9) { maxNum = 9; break; }
                            else if (num == 8) maxNum = max(maxNum, 8);
                            else if (num == 7) maxNum = max(maxNum, 7);
                            if (num == 1 || num == 2) lowValuePenalty++;
                        }
                        int cohesion = calculateCohesion(grid, positions);
                        int score = maxNum + cohesion - lowValuePenalty;
                        pq.push({ score, positions });
                    }
                }
            }
        }
    }

    while (!pq.empty()) {
        auto [_, positions] = pq.top();
        pq.pop();

        int highValueCount = 0;
        for (const auto& pos : positions) {
            int num = grid[pos.first][pos.second];
            if (num == 7 || num == 8 || num == 9) highValueCount++;
        }

        vector<vector<int>> newGrid = grid;
        for (const auto& pos : positions) {
            newGrid[pos.first][pos.second] = 0;
        }

        int futureRemoved = maxHighValueRemoved(newGrid, depth + 1, maxDepth, cache, maxPossible);
        int totalRemoved = highValueCount + futureRemoved;

        if (totalRemoved + (maxDepth - depth) < maxPossible) continue;

        maxRemoved = max(maxRemoved, totalRemoved);
        maxPossible = max(maxPossible, totalRemoved);
    }

    cache[gridKey] = maxRemoved;
    return maxRemoved;
}

// 최대 사과 제거 개수를 계산 (사과 50개 이하에서 사용)
int maxApplesRemoved(const vector<vector<int>>& grid, int depth, int maxDepth, unordered_map<string, int>& cache, int& maxPossible) {
    if (depth >= maxDepth) return 0;

    string gridKey = gridToString(grid);
    if (cache.count(gridKey)) return cache[gridKey];

    int rows = grid.size();
    int cols = grid[0].size();
    int maxRemoved = 0;

    vector<vector<int>> prefix(rows + 1, vector<int>(cols + 1, 0));
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1] + grid[i - 1][j - 1];
        }
    }

    auto getSum = [&](int r1, int c1, int r2, int c2) {
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
        };

    auto comp = [](const pair<int, vector<pair<int, int>>>& a, const pair<int, vector<pair<int, int>>>& b) {
        return a.first < b.first;
        };
    priority_queue<pair<int, vector<pair<int, int>>>, vector<pair<int, vector<pair<int, int>>>>, decltype(comp)> pq(comp);

    for (int r1 = 0; r1 < rows; ++r1) {
        for (int r2 = r1; r2 < rows; ++r2) {
            for (int c1 = 0; c1 < cols; ++c1) {
                for (int c2 = c1; c2 < cols; ++c2) {
                    if (getSum(r1, c1, r2, c2) == 10) {
                        vector<int> bundle;
                        vector<pair<int, int>> positions;
                        for (int i = r1; i <= r2; ++i) {
                            for (int j = c1; j <= c2; ++j) {
                                if (grid[i][j] != 0) {
                                    bundle.push_back(grid[i][j]);
                                    positions.push_back({ i, j });
                                }
                            }
                        }

                        int appleCount = bundle.size();
                        int lowValuePenalty = 0;
                        for (int num : bundle) {
                            if (num == 1 || num == 2) lowValuePenalty++;
                        }
                        int cohesion = calculateCohesion(grid, positions);
                        int score = appleCount + cohesion - lowValuePenalty;
                        pq.push({ score, positions });
                    }
                }
            }
        }
    }

    while (!pq.empty()) {
        auto [_, positions] = pq.top();
        pq.pop();

        int currentRemoved = positions.size();
        vector<vector<int>> newGrid = grid;
        for (const auto& pos : positions) {
            newGrid[pos.first][pos.second] = 0;
        }

        int futureRemoved = maxApplesRemoved(newGrid, depth + 1, maxDepth, cache, maxPossible);
        int totalRemoved = currentRemoved + futureRemoved;

        if (totalRemoved + (maxDepth - depth) * 10 < maxPossible) continue;

        maxRemoved = max(maxRemoved, totalRemoved);
        maxPossible = max(maxPossible, totalRemoved);
    }

    cache[gridKey] = maxRemoved;
    return maxRemoved;
}

// 최적의 번들 찾기 함수
vector<pair<int, int>> findAppleBundle(const vector<vector<int>>& grid) {
    int rows = grid.size();
    if (rows == 0) return {};
    int cols = grid[0].size();

    int appleCount = countApples(grid);
    cout << "Grid size in findAppleBundle: " << rows << "x" << cols << endl;

    vector<vector<int>> prefix(rows + 1, vector<int>(cols + 1, 0));
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1] + grid[i - 1][j - 1];
        }
    }

    auto getSum = [&](int r1, int c1, int r2, int c2) {
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
        };

    // 사과 개수가 160~170개일 때 재귀 탐색 없이 번들 선택
    if (appleCount >= 160 && appleCount <= 170) {
        cout << "Non-recursive mode: Apple count between 160 and 170" << endl;
        auto comp = [](const pair<int, vector<pair<int, int>>>& a, const pair<int, vector<pair<int, int>>>& b) {
            return a.first < b.first;
            };
        priority_queue<pair<int, vector<pair<int, int>>>, vector<pair<int, vector<pair<int, int>>>>, decltype(comp)> pq(comp);

        for (int r1 = 0; r1 < rows; ++r1) {
            for (int r2 = r1; r2 < rows; ++r2) {
                for (int c1 = 0; c1 < cols; ++c1) {
                    for (int c2 = c1; c2 < cols; ++c2) {
                        if (getSum(r1, c1, r2, c2) == 10) {
                            vector<int> bundle;
                            vector<pair<int, int>> positions;
                            for (int i = r1; i <= r2; ++i) {
                                for (int j = c1; j <= c2; ++j) {
                                    if (grid[i][j] != 0) {
                                        bundle.push_back(grid[i][j]);
                                        positions.push_back({ i, j });
                                    }
                                }
                            }

                            int maxNum = 0;
                            int lowValuePenalty = 0;
                            for (int num : bundle) {
                                if (num == 9) { maxNum = 9; break; }
                                else if (num == 8) maxNum = max(maxNum, 8);
                                else if (num == 7) maxNum = max(maxNum, 7);
                                if (num == 1 || num == 2) lowValuePenalty++;
                            }
                            int cohesion = calculateCohesion(grid, positions);
                            int score = maxNum + cohesion - lowValuePenalty;
                            pq.push({ score, positions });
                        }
                    }
                }
            }
        }

        if (!pq.empty()) {
            return pq.top().second;
        }
        return {};
    }

    // 사과 개수가 159개 이하일 때 재귀 탐색
    int maxDepth;
    if (appleCount <= 30) {
        maxDepth = 8; // 사과 30개 이하: 8단계 (maxApplesRemoved)
    }
    else if (appleCount <= 60) {
        maxDepth = 5; // 사과 60개 이하: 5단계 (maxApplesRemoved)
    }
    else if (appleCount >= 100 && appleCount <= 159) {
        maxDepth = 3; // maxHighValueRemoved
    }
    else if (appleCount > 60 && appleCount < 100) {
        maxDepth = 4; // maxHighValueRemoved
    }
    else {
        cout << "No recursion: Apple count > 170 or invalid" << endl;
        return {};
    }

    cout << "Recursive mode: Max depth = " << maxDepth << endl;

    vector<pair<int, int>> bestCoords;
    int bestScore = -1;
    unordered_map<string, int> cache;
    int maxPossible = 0;

    for (int r1 = 0; r1 < rows; ++r1) {
        for (int r2 = r1; r2 < rows; ++r2) {
            for (int c1 = 0; c1 < cols; ++c1) {
                for (int c2 = c1; c2 < cols; ++c2) {
                    if (getSum(r1, c1, r2, c2) == 10) {
                        vector<int> bundle;
                        vector<pair<int, int>> positions;
                        for (int i = r1; i <= r2; ++i) {
                            for (int j = c1; j <= c2; ++j) {
                                if (grid[i][j] != 0) {
                                    bundle.push_back(grid[i][j]);
                                    positions.push_back({ i, j });
                                }
                            }
                        }

                        int highValueCount = 0;
                        int appleCountInBundle = bundle.size();
                        int lowValuePenalty = 0;
                        for (int num : bundle) {
                            if (num == 7 || num == 8 || num == 9) highValueCount++;
                            if (num == 1 || num == 2) lowValuePenalty++;
                        }
                        int cohesion = calculateCohesion(grid, positions);

                        vector<vector<int>> newGrid = grid;
                        for (const auto& pos : positions) {
                            newGrid[pos.first][pos.second] = 0;
                        }

                        int futureRemoved;
                        if (appleCount > 50) { // 사과 50개 초과: maxHighValueRemoved
                            futureRemoved = maxHighValueRemoved(newGrid, 1, maxDepth, cache, maxPossible);
                        }
                        else { // 사과 50개 이하: maxApplesRemoved
                            futureRemoved = maxApplesRemoved(newGrid, 1, maxDepth, cache, maxPossible);
                        }

                        int totalRemoved = (appleCount > 50 ? highValueCount : appleCountInBundle) + futureRemoved;
                        int adjustedScore = totalRemoved + cohesion - lowValuePenalty;

                        if (adjustedScore > bestScore) {
                            bestScore = adjustedScore;
                            bestCoords = positions;
                        }
                    }
                }
            }
        }
    }

    return bestCoords;
}