#pragma once
#include "TextBuffer.h"
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

const D2D1_COLOR_F BackgroundColor = D2D1::ColorF(0, 0, 0);
const D2D1_COLOR_F TextColor = D2D1::ColorF(0.8, 0.8, 0.8);

class Renderer {
  public:
    ID2D1HwndRenderTarget* RenderTarget;
    ID2D1SolidColorBrush* TextBrush;
    IDWriteTextFormat* TextFormat;

    D2D_RECT_F WindowRect;

    Renderer() = default;
	Renderer(_In_ HWND hwnd);
    ~Renderer();

	HRESULT Resize(_In_ D2D_SIZE_U newSize) noexcept;

	HRESULT Render(TextBuffer buffer) noexcept;
};

