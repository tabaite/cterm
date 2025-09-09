#pragma once
#include <sal.h>

constexpr int TEXT_BUFFER_SIZE = 16777216;

class TextBuffer {
  public:
    wchar_t* BufferStart;
    // Index number in terms of sizeof(wchar_t).
    int WriteIndex;
    // Used in terms of sizeof(wchar_t).
    int Used;

    TextBuffer(_Inout_ wchar_t* start) noexcept;
    void PushString(_In_ wchar_t* string, int len) noexcept;
};
