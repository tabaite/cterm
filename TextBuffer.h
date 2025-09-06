#pragma once
#include <cstdint>

#define TEXT_BUFFER_SIZE 16777216

class TextBuffer {
  public:
    wchar_t* BufferStart;
    unsigned int StartIndex : 24;
    unsigned int EndIndex : 24;

    TextBuffer(_Inout_ wchar_t* start);
    void PushString(_In_ wchar_t* string, uint32_t len);
};
