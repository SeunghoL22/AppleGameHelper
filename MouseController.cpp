#include "MouseController.h"
#include <cmath>
#include <iostream>

void MouseController::delay(int milliseconds) {
    Sleep(milliseconds); // Windows API의 Sleep 함수로 딜레이 추가
}

void MouseController::moveMouse(int x, int y) {
    //std::cout << "Moving mouse to (" << x << ", " << y << ")\n";
    SetCursorPos(x, y); // 마우스 커서를 (x, y)로 이동
    delay(10); // 이동 후 10ms 대기 (드래그를 위한 짧은 간격)
}

void MouseController::leftButtonDown() {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    UINT result = SendInput(1, &input, sizeof(INPUT));
    if (result != 1) {
        std::cerr << "Failed to send left button down event\n";
    }
    else {
    //    std::cout << "Left button down\n";
    }
    delay(10); // 클릭 후 10ms 대기
}

void MouseController::leftButtonUp() {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    UINT result = SendInput(1, &input, sizeof(INPUT));
    if (result != 1) {
        std::cerr << "Failed to send left button up event\n";
    }
    else {
    //    std::cout << "Left button up\n";
    }
    delay(10); // 클릭 후 10ms 대기
}

void MouseController::dragMouse(int startX, int startY, int endX, int endY) {
    //std::cout << "Dragging from (" << startX << ", " << startY << ") to (" << endX << ", " << endY << ")\n";
    moveMouse(startX, startY);    // 시작점으로 이동
    leftButtonDown();             // 왼쪽 버튼 누름

    // 직사각형 드래그를 위해 점진적으로 이동 (대각선 보간)
    int steps = std::max(abs(endX - startX), abs(endY - startY)) / 40; // 10픽셀 단위로 이동
    if (steps == 0) steps = 1;

    for (int i = 0; i <= steps; ++i) {
        double t = static_cast<double>(i) / steps;
        int x = static_cast<int>(startX + (endX - startX) * t);
        int y = static_cast<int>(startY + (endY - startY) * t);
        moveMouse(x, y); // 점진적 이동
    }

    moveMouse(endX, endY);        // 정확히 끝점에 도달
    leftButtonUp();               // 왼쪽 버튼 놓음
}

void MouseController::selectAppleBundle(const std::vector<std::pair<int, int>>& bundle,
    int screenX, int screenY, int gridWidth, int gridHeight) {
    if (bundle.empty()) {
        std::cout << "No bundle to select\n";
        return;
    }

    // bundle의 최소/최대 행과 열을 계산하여 직사각형 범위 결정
    int minRow = bundle[0].first;
    int maxRow = bundle[0].first;
    int minCol = bundle[0].second;
    int maxCol = bundle[0].second;

    for (const auto& p : bundle) {
        minRow = std::min(minRow, p.first);
        maxRow = std::max(maxRow, p.first);
        minCol = std::min(minCol, p.second);
        maxCol = std::max(maxCol, p.second);
    }

    // 직사각형 크기 확장을 위한 조정값 (위아래로 0.5 셀 크기만큼 확장, 좌우로 0.5 셀 크기만큼 확장)
    const float expandFactorY = 0.5f;  // 위아래로 0.5 셀 크기만큼 확장 (조정 가능)
    const float expandFactorX = 0.5f;  // 좌우로 0.5 셀 크기만큼 확장 (조정 가능)

    // 화면 좌표로 변환 (각 셀의 중심점 사용)
    int startX = screenX + static_cast<int>((minCol + 0.5 - expandFactorX) * gridWidth);  // 왼쪽 위 X (좌측으로 확장)
    int startY = screenY + static_cast<int>((minRow + 0.5 - expandFactorY) * gridHeight); // 왼쪽 위 Y (위로 확장)
    int endX = screenX + static_cast<int>((maxCol + 0.5 + expandFactorX) * gridWidth);    // 오른쪽 아래 X (우측으로 확장)
    int endY = screenY + static_cast<int>((maxRow + 0.5 + expandFactorY) * gridHeight);   // 오른쪽 아래 Y (아래로 확장)

    // 좌표 범위 확인 (확장된 범위가 게임 창을 벗어나지 않도록 조정)
    int gameMinX = screenX;
    int gameMaxX = screenX + 800;
    int gameMinY = screenY;
    int gameMaxY = screenY + 475;

    startX = std::max(gameMinX, std::min(gameMaxX, startX));
    startY = std::max(gameMinY, std::min(gameMaxY, startY));
    endX = std::max(gameMinX, std::min(gameMaxX, endX));
    endY = std::max(gameMinY, std::min(gameMaxY, endY));

    // 조정된 좌표 로그 출력
    //std::cout << "Adjusted drag coordinates: start(" << startX << ", " << startY
    //    << "), end(" << endX << ", " << endY << ")\n";

    // 마우스 드래그로 사과 묶음 선택 (직사각형 형태)
    dragMouse(startX, startY, endX, endY);
}
