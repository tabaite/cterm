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

    IDWriteTextFormat* textFormat = nullptr;
    HRESULT textFormatSucceeded = dWriteFactory->CreateTextFormat(
        L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, 72, L"en-us", &textFormat);
    if (!textFormatSucceeded) {
        throw textFormatSucceeded;
    }

    textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

    RECT rect = {0};
    if (!GetClientRect(hwnd, &rect)) {
        throw static_cast<HRESULT>(GetLastError());
    }
    struct D2D_SIZE_U pixelSize;
    pixelSize.height = rect.bottom - rect.top;
    pixelSize.width = rect.right - rect.left;

    struct D2D1_HWND_RENDER_TARGET_PROPERTIES rtProperties;
    rtProperties.hwnd = hwnd;
    rtProperties.pixelSize = pixelSize;

    ID2D1HwndRenderTarget* hwndRenderTarget = nullptr;
    HRESULT rtSucceeded = factory->CreateHwndRenderTarget(
        D2D1_RENDER_TARGET_PROPERTIES(), rtProperties, &hwndRenderTarget);
    if (!rtSucceeded) {
        throw rtSucceeded;
    }

    auto testBrush = hwndRenderTarget->CreateSolidColorBrush()
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
