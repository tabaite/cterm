#pragma once
constexpr int INPUT_BUFFER_SIZE = 4096;

class CommandInputBuffer {
  public:
    wchar_t* Buffer;
    int CursorPosition;
    int Used;

    CommandInputBuffer(wchar_t* buffer) noexcept;
};
