#include "Renderer.h"
#include "winerror.h"

Renderer::Renderer(_In_ HWND hwnd) {
    ID2D1Factory* factory = nullptr;
    HRESULT factorySucceeded = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), NULL,
        reinterpret_cast<void**>(&factory));
    if (FAILED(factorySucceeded)) {
        throw factorySucceeded;
    }
    IDWriteFactory* dWriteFactory = nullptr;
    HRESULT dWriteFactorySucceeded = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&dWriteFactory));
    if (FAILED(dWriteFactorySucceeded)) {
        throw dWriteFactorySucceeded;
    }

    IDWriteTextFormat* textFormat = nullptr;
    HRESULT textFormatSucceeded = dWriteFactory->CreateTextFormat(
        L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL, 72, L"en-us", &textFormat);
    if (FAILED(textFormatSucceeded)) {
        throw textFormatSucceeded;
    }

    textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

    RECT rect = {0};
    if (!GetClientRect(hwnd, &rect)) {
        throw static_cast<HRESULT>(GetLastError());
    }
    D2D_RECT_F d2dRect;
    d2dRect.bottom = static_cast<FLOAT>(rect.bottom);
    d2dRect.top = static_cast<FLOAT>(rect.top);
    d2dRect.left = static_cast<FLOAT>(rect.left);
    d2dRect.right = static_cast<FLOAT>(rect.right);
    D2D_SIZE_U pixelSize;
    pixelSize.height = rect.bottom - rect.top;
    pixelSize.width = rect.right - rect.left;

    ID2D1HwndRenderTarget* hwndRenderTarget = nullptr;
    HRESULT rtSucceeded = factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, pixelSize),
        &hwndRenderTarget);
    if (FAILED(rtSucceeded)) {
        throw rtSucceeded;
    }

    ID2D1SolidColorBrush* testBrush = nullptr;
    HRESULT testBrushSucceeded =
        hwndRenderTarget->CreateSolidColorBrush(&TextColor, NULL, &testBrush);
    if (FAILED(testBrushSucceeded)) {
        throw testBrushSucceeded;
    }

    this->RenderTarget = hwndRenderTarget;
    this->TextBrush = testBrush;
    this->TextFormat = textFormat;
    this->WindowRect = d2dRect;
}

Renderer::~Renderer() {
    this->TextFormat->Release();
    this->TextBrush->Release();
    this->RenderTarget->Release();
}

HRESULT Renderer::Resize(_In_ D2D_SIZE_U newSize) noexcept {
    D2D_RECT_F newRect;
    newRect.bottom = static_cast<FLOAT>(newSize.height);
    newRect.top = 0.0;
    newRect.right = static_cast<FLOAT>(newSize.width);
    newRect.left = 0.0;

    this->WindowRect = newRect;
    return this->RenderTarget->Resize(newSize);
}

HRESULT Renderer::Render() noexcept {
    this->RenderTarget->BeginDraw();
    this->RenderTarget->Clear(BackgroundColor);
    this->RenderTarget->DrawTextW(L"who up... jorkin it", 20, this->TextFormat,
                                  this->WindowRect, this->TextBrush);
    return this->RenderTarget->EndDraw();
}
