#include "TextBuffer.h"
#include <cstring>

TextBuffer::TextBuffer(_Inout_ wchar_t* start) noexcept
    : WriteIndex(0), ReadIndex(0) {
    this->BufferStart = start;
}

void TextBuffer::PushString(_In_ wchar_t* string, int len) noexcept {
    // first, we determine how much space we have left in the buffer.
    // wraparounds require us to do extra work and should be avoided.
    const int remainingPushSpace = TEXT_BUFFER_SIZE - WriteIndex;

    int splitAtIndex = len > remainingPushSpace ? remainingPushSpace : 0;
    // if the entire string can be pushed onto the buffer:
    // first part: len, second part: 0
    // if the string was split at a newline to fill space better:
    // first part: newLine, second part: len - newLine
    // if the entire string needs to be wrapped around:
    // first part: 0, second part: len

    memcpy(BufferStart + WriteIndex, string, splitAtIndex * sizeof(wchar_t));
    this->WriteIndex += splitAtIndex;

    int remainingBytes = len - splitAtIndex;
    int overwriteStart = splitAtIndex;
    while (remainingBytes > 0) {
        int copyLength = remainingBytes > TEXT_BUFFER_SIZE - ReadIndex
                             ? TEXT_BUFFER_SIZE - ReadIndex
                             : remainingBytes;
        memcpy(BufferStart + ReadIndex, string + overwriteStart, copyLength);

        // readindex will never exceed TEXT_BUFFER_SIZE due
        // to bounds on copyLength. thus, the TEXT_BUFFER_SIZE
        // trick sets readindex to 0 when it reaches it.
        ReadIndex = (ReadIndex + copyLength) % TEXT_BUFFER_SIZE;
        remainingBytes -= copyLength;
    }
}
