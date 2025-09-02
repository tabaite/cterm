#include "Renderer.h"
#include "winerror.h"

Renderer::Renderer(HWND hwnd) {
    ID2D1Factory* factory = nullptr;
    HRESULT factorySucceeded = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), NULL,
        reinterpret_cast<void**>(&factory));
    if (!factorySucceeded) {
        throw factorySucceeded;
    }
    IDWriteFactory* dWriteFactory = nullptr;
    HRESULT dWriteFactorySucceeded = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&dWriteFactory));
    if (!dWriteFactorySucceeded) {
        throw dWriteFactorySucceeded;
    }

    IDWriteTextFormat textFormat = dWriteFactory->CreateTextFormat(L"Arial");
}

HRESULT Renderer::Resize(struct Renderer* renderer, D2D_SIZE_U newSize) noexcept {
    D2D_RECT_F newRect;
    newRect.bottom = static_cast<FLOAT>(newSize.height);
    newRect.top = 0.0;
    newRect.right = static_cast<FLOAT>(newSize.width);
    newRect.left = 0.0;

    renderer->WindowRect = newRect;
    return renderer->RenderTarget->Resize(newSize);
}

HRESULT Renderer::Render(struct Renderer* renderer) noexcept {
    renderer->RenderTarget->BeginDraw();
    renderer->RenderTarget->Clear(BackgroundColor);
    renderer->RenderTarget->DrawTextW(L"who up... jorkin it", 20, renderer->TextFormat,
                                  renderer->WindowRect, renderer->TextBrush);
    return renderer->RenderTarget->EndDraw();
}
