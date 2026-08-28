#include "t1.hpp"

#include <utility>
#include <vector>
t1::t1() { pipe = -1; }
t1::t1(int pipe) { this->pipe = pipe; }
t1::t1(t1&& other) noexcept {
    pipe = other.pipe;
    other.pipe = -1;
}
t1& t1::operator=(t1&& other) noexcept {
    if (this != &other) {
        if (pipe >= 0) {
            int result = close(pipe);
            if (result == -1) {
                std::cerr << "Error closing pipe: " << strerror(errno)
                          << std::endl;
            } else {
                std::cout << "Pipe closed successfully." << std::endl;
            }
        }
        pipe = other.pipe;
        other.pipe = -1;
    }
    return *this;
}
int t1::movePipe() {
    int oldPipe = pipe;
    pipe = -1;
    return oldPipe;
}
t1::~t1() {
    if (pipe >= 0) {
        int result = close(pipe);
        if (result == -1) {
            std::cerr << "Error closing pipe: " << strerror(errno) << std::endl;
        } else {
            std::cout << "Pipe closed successfully." << std::endl;
        }

        pipe = -1;
    }
}
void t1::prt() { std::cout << "hello world" << std::endl; }

void sort(std::vector<int>& nums, int left, int right) {
    if (left >= right)
        return;
    int l = left;
    int r = right;
    int privot = nums[l];
    while (l < r) {
        while (l < r && nums[r] >= privot) {
            r--;
        }
        while (l < r && nums[l] <= privot) {
            l++;
        }
        std::swap(nums[l], nums[r]);
    }
    std::swap(nums[left], nums[l]);
    sort(nums, left, l - 1);
    sort(nums, l + 1, right);
}
void sort2()