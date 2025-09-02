#pragma once
#include <windef.h>
#include <d2d1.h>
#include <dwrite.h>

const D2D1_COLOR_F BackgroundColor = D2D1::ColorF(0, 0, 0);

class Renderer {
    ID2D1HwndRenderTarget* RenderTarget;
    ID2D1SolidColorBrush* TextBrush;
    IDWriteTextFormat* TextFormat;

    D2D_RECT_F WindowRect;

	Renderer(HWND hwnd);

	HRESULT Resize(struct Renderer* renderer, D2D_SIZE_U newSize) noexcept;

	HRESULT Render(struct Renderer* renderer) noexcept;
};

