#include "SessionState.h"
#include "resource.h"
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

constexpr int SESSION_WND_OFFSET = 0;

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    auto Title = L"title";
    auto ClassName = L"cterm";
    auto MenuName = L"CTERM!";

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(Renderer) + sizeof(uintptr_t);
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CTERM));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MenuName;
    wcex.lpszClassName = ClassName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    auto atom = RegisterClassExW(&wcex);

    hInst = hInstance;

    HWND hWnd =
        CreateWindowW(ClassName, Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
                      CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) {
        return FALSE;
    }

    wchar_t* sessionBuffer =
        reinterpret_cast<wchar_t*>(malloc(TEXT_BUFFER_SIZE));

    SessionState session(hWnd, sessionBuffer);

    SetWindowLongPtr(hWnd, SESSION_WND_OFFSET,
                     reinterpret_cast<LONG_PTR>(&session));

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
    switch (message) {
    case WM_KEYDOWN: {
        SessionState* session = reinterpret_cast<SessionState*>(
            GetWindowLongPtr(hWnd, SESSION_WND_OFFSET));
        if (!session)
            return 0;

        BYTE keyState[256] = {0};
        if (!GetKeyboardState(keyState)) {
            return 0;
        }
        keyState[VK_CONTROL] = 0;
        keyState[VK_MENU] = 0;
        keyState[VK_LCONTROL] = 0;
        keyState[VK_LMENU] = 0;
        keyState[VK_RCONTROL] = 0;
        keyState[VK_RMENU] = 0;

        wchar_t outKeyBuffer[2] = {0, 0};
        UINT scanCode = (lParam >> 16) & 0xFF;
        int len = ToUnicode(wParam, scanCode, keyState, outKeyBuffer, 2, 0);

        session->Buffer.PushString(outKeyBuffer, len);
        break;
    }
    case WM_SIZE: {
        SessionState* session = reinterpret_cast<SessionState*>(
            GetWindowLongPtr(hWnd, SESSION_WND_OFFSET));
        if (!session)
            return 0;
        D2D_SIZE_U newSize;
        newSize.width = static_cast<UINT32>(LOWORD(lParam));
        newSize.height = static_cast<UINT32>(HIWORD(lParam));
        session->Renderer.Resize(newSize);
        break;
    }
    case WM_PAINT: {
        SessionState* session = reinterpret_cast<SessionState*>(
            GetWindowLongPtr(hWnd, SESSION_WND_OFFSET));
        if (!session)
            return 0;
        session->Renderer.Render(session->Buffer);
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}