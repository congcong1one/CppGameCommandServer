#include "t1.hpp"
t1::t1() {
    pipe = -1;
}
t1::t1(int pipe) {
    this->pipe = pipe;
}
t1::~t1() {
    if(pipe >= 0) {
        int result = close(pipe);
    if (result == -1) {
        std::cerr << "Error closing pipe: " << strerror(errno) << std::endl;
    }else {
        std::cout << "Pipe closed successfully." << std::endl;
    }

    pipe = -1;
    }
}
void t1::prt() { std::cout << "hello world" << std::endl; }
