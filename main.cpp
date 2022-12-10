#include <iostream>
#include <fstream>
int main() {
    std::fstream scan("test");
    long lac;
    scan<<1<<2;
    lac=scan.tellg();
    return 0;
}
