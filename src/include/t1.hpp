#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

class t1 {
public:
    t1();
    t1(int pipe);
    void prt();
    t1(t1&& other) noexcept;
    int movePipe();
    t1& operator=(t1&& other) noexcept;
    ~t1();
    int pipe;
};
