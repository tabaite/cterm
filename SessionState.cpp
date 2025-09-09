#include "SessionState.h"

SessionState::SessionState(HWND hwnd, wchar_t* buffer)
    : Renderer(hwnd), Buffer(buffer) {}