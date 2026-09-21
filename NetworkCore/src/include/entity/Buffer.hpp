#pragma once
#include <vector>
struct Buffer {
    std::vector<char> data;
    int head;
    int size;
};