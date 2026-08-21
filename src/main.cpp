#include "t1.hpp"
#include <fcntl.h>
int main() {
    int _pipe[2];
    int result = pipe(_pipe);
    int p1,p2;
    p1 = _pipe[0];
    p2 = _pipe[1];
    if (result == -1) {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
        return 1;
    }
    {
        t1 t(p1);
        t.prt();
    }
    result = fcntl(p1, F_GETFD);
    if (result == -1) {
        std::cerr << "Error getting flags for pipe[0]: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Flags for pipe[0]: " << result << std::endl;
    }
    result = fcntl(p2, F_GETFD);
    if (result == -1) {
        std::cerr << "Error getting flags for pipe[1]: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Flags for pipe[1]: " << result << std::endl;
    }
    close(p2);
    return 0;
}
