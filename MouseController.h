#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#define NOMINMAX // Windows.h에서 min, max 매크로 비활성화

#include <vector>
#include <utility>
#include <algorithm>
#include <Windows.h>

class MouseController {
public:
    static void moveMouse(int x, int y);                     // 마우스를 특정 위치로 이동
    static void leftButtonDown();                           // 마우스 왼쪽 버튼 누름
    static void leftButtonUp();                             // 마우스 왼쪽 버튼 놓음
    static void dragMouse(int startX, int startY, int endX, int endY); // 직사각형 형태로 드래그 수행
    static void selectAppleBundle(const std::vector<std::pair<int, int>>& bundle,
        int screenX, int screenY, int gridWidth, int gridHeight); // 사과 묶음 선택
private:
    static void delay(int milliseconds);                    // 딜레이 추가
};

#endif // MOUSE_CONTROLLER_H