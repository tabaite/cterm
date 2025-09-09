#pragma once
#include "TextBuffer.h"
#include "Renderer.h"

class SessionState {
  public:
    Renderer Renderer;
    TextBuffer Buffer;

    SessionState(HWND hwnd, wchar_t* buffer);
};
