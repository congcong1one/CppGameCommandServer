#pragma once
#include <cstdint>
#include <vector>

struct PacketHeader {
    uint16_t magic;
    uint32_t length;
    uint32_t msgId;
    uint32_t seq;
    uint32_t playerId;
};