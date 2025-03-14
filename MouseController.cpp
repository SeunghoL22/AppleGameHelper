#include "MouseController.h"
#include <cmath>
#include <iostream>

void MouseController::delay(int milliseconds) {
    Sleep(milliseconds); // Windows API�� Sleep �Լ��� ������ �߰�
}

void MouseController::moveMouse(int x, int y) {
    //std::cout << "Moving mouse to (" << x << ", " << y << ")\n";
    SetCursorPos(x, y); // ���콺 Ŀ���� (x, y)�� �̵�
    delay(10); // �̵� �� 10ms ��� (�巡�׸� ���� ª�� ����)
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
    delay(10); // Ŭ�� �� 10ms ���
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
    delay(10); // Ŭ�� �� 10ms ���
}

void MouseController::dragMouse(int startX, int startY, int endX, int endY) {
    //std::cout << "Dragging from (" << startX << ", " << startY << ") to (" << endX << ", " << endY << ")\n";
    moveMouse(startX, startY);    // ���������� �̵�
    leftButtonDown();             // ���� ��ư ����

    // ���簢�� �巡�׸� ���� ���������� �̵� (�밢�� ����)
    int steps = std::max(abs(endX - startX), abs(endY - startY)) / 40; // 10�ȼ� ������ �̵�
    if (steps == 0) steps = 1;

    for (int i = 0; i <= steps; ++i) {
        double t = static_cast<double>(i) / steps;
        int x = static_cast<int>(startX + (endX - startX) * t);
        int y = static_cast<int>(startY + (endY - startY) * t);
        moveMouse(x, y); // ������ �̵�
    }

    moveMouse(endX, endY);        // ��Ȯ�� ������ ����
    leftButtonUp();               // ���� ��ư ����
}

void MouseController::selectAppleBundle(const std::vector<std::pair<int, int>>& bundle,
    int screenX, int screenY, int gridWidth, int gridHeight) {
    if (bundle.empty()) {
        std::cout << "No bundle to select\n";
        return;
    }

    // bundle�� �ּ�/�ִ� ��� ���� ����Ͽ� ���簢�� ���� ����
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

    // ���簢�� ũ�� Ȯ���� ���� ������ (���Ʒ��� 0.5 �� ũ�⸸ŭ Ȯ��, �¿�� 0.5 �� ũ�⸸ŭ Ȯ��)
    const float expandFactorY = 0.5f;  // ���Ʒ��� 0.5 �� ũ�⸸ŭ Ȯ�� (���� ����)
    const float expandFactorX = 0.5f;  // �¿�� 0.5 �� ũ�⸸ŭ Ȯ�� (���� ����)

    // ȭ�� ��ǥ�� ��ȯ (�� ���� �߽��� ���)
    int startX = screenX + static_cast<int>((minCol + 0.5 - expandFactorX) * gridWidth);  // ���� �� X (�������� Ȯ��)
    int startY = screenY + static_cast<int>((minRow + 0.5 - expandFactorY) * gridHeight); // ���� �� Y (���� Ȯ��)
    int endX = screenX + static_cast<int>((maxCol + 0.5 + expandFactorX) * gridWidth);    // ������ �Ʒ� X (�������� Ȯ��)
    int endY = screenY + static_cast<int>((maxRow + 0.5 + expandFactorY) * gridHeight);   // ������ �Ʒ� Y (�Ʒ��� Ȯ��)

    // ��ǥ ���� Ȯ�� (Ȯ��� ������ ���� â�� ����� �ʵ��� ����)
    int gameMinX = screenX;
    int gameMaxX = screenX + 800;
    int gameMinY = screenY;
    int gameMaxY = screenY + 475;

    startX = std::max(gameMinX, std::min(gameMaxX, startX));
    startY = std::max(gameMinY, std::min(gameMaxY, startY));
    endX = std::max(gameMinX, std::min(gameMaxX, endX));
    endY = std::max(gameMinY, std::min(gameMaxY, endY));

    // ������ ��ǥ �α� ���
    //std::cout << "Adjusted drag coordinates: start(" << startX << ", " << startY
    //    << "), end(" << endX << ", " << endY << ")\n";

    // ���콺 �巡�׷� ��� ���� ���� (���簢�� ����)
    dragMouse(startX, startY, endX, endY);
}
