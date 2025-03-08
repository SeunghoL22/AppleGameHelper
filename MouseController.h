#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

#define NOMINMAX // Windows.h���� min, max ��ũ�� ��Ȱ��ȭ

#include <vector>
#include <utility>
#include <algorithm>
#include <Windows.h>

class MouseController {
public:
    static void moveMouse(int x, int y);                     // ���콺�� Ư�� ��ġ�� �̵�
    static void leftButtonDown();                           // ���콺 ���� ��ư ����
    static void leftButtonUp();                             // ���콺 ���� ��ư ����
    static void dragMouse(int startX, int startY, int endX, int endY); // ���簢�� ���·� �巡�� ����
    static void selectAppleBundle(const std::vector<std::pair<int, int>>& bundle,
        int screenX, int screenY, int gridWidth, int gridHeight); // ��� ���� ����
private:
    static void delay(int milliseconds);                    // ������ �߰�
};

#endif // MOUSE_CONTROLLER_H