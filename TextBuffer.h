#pragma once
#include <sal.h>

constexpr int TEXT_BUFFER_SIZE = 16777216;

class TextBuffer {
  public:
    wchar_t* BufferStart;
    int WriteIndex;
    int ReadIndex;

    TextBuffer(_Inout_ wchar_t* start) noexcept;
    void PushString(_In_ wchar_t* string, int len) noexcept;
};
