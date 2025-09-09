#include "TextBuffer.h"
#include <cstring>

TextBuffer::TextBuffer(_Inout_ wchar_t* start) noexcept
    : WriteIndex(0), Used(0) {
    this->BufferStart = start;
}

void TextBuffer::PushString(_In_ wchar_t* string, int len) noexcept {
    int remainingChars = len;
    while (remainingChars > 0) {
        int copyLength = remainingChars > TEXT_BUFFER_SIZE - WriteIndex
                             ? TEXT_BUFFER_SIZE - WriteIndex
                             : remainingChars;
        memcpy(BufferStart + WriteIndex,
               string + len * sizeof(wchar_t) - remainingChars * sizeof(wchar_t),
               copyLength * sizeof(wchar_t));

        // readindex will never exceed TEXT_BUFFER_SIZE due
        // to bounds on copyLength. thus, the TEXT_BUFFER_SIZE
        // trick sets readindex to 0 when it reaches it.
        WriteIndex = (WriteIndex + copyLength) % TEXT_BUFFER_SIZE;
        Used = Used + copyLength > TEXT_BUFFER_SIZE ? TEXT_BUFFER_SIZE
                                                    : Used + copyLength;
        remainingChars -= copyLength;
    }
}
