#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>

class t1 {
public:
    t1();
    t1(int pipe);
    void prt();
    ~t1();
    int pipe;
};
