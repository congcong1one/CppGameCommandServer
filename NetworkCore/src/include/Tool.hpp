
#pragma once
#include <array>
#include <cstdint>
#include <cstring>

#include "entity/Buffer.hpp"
namespace Tool {
uint16_t readRingBufferUint16(Buffer& buffer) {
    uint16_t value = (static_cast<uint16_t>((buffer.data[buffer.head]) << 8) |
                      static_cast<uint16_t>(
                          buffer.data[(buffer.head + 1) % buffer.data.size()]));
    buffer.head = (buffer.head + 2) % buffer.data.size();
    buffer.size -= 2;
    return value;
}

uint32_t readRingBufferUint32(Buffer& buffer) {
    uint32_t value = (static_cast<uint32_t>(buffer.data[buffer.head]) << 24) |
                     (static_cast<uint32_t>(
                          buffer.data[(buffer.head + 1) % buffer.data.size()])
                      << 16) |
                     (static_cast<uint32_t>(
                          buffer.data[(buffer.head + 2) % buffer.data.size()])
                      << 8) |
                     static_cast<uint32_t>(
                         buffer.data[(buffer.head + 3) % buffer.data.size()]);
    buffer.head = (buffer.head + 4) % buffer.data.size();
    buffer.size -= 4;

    return value;
};

std::vector<char> readRingBuffer(Buffer& buffer, size_t length) {
    std::vector<char> result(length);
    for (size_t i = 0; i < length; ++i) {
        result[i] = buffer.data[(buffer.head + i) % buffer.data.size()];
    }
    buffer.head = (buffer.head + length) % buffer.data.size();
    buffer.size -= length;
    return result;
};  // namespace Tool

}  // namespace Tool