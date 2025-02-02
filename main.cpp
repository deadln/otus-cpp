#include <iostream>
#include <atomic>

#include "async.h"

int main(int, char *[]) {
    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    auto h3 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "10\n20\n30\n40\n50\n10\n20\n30\n40\n50\n10\n20\n30\n40\n50\n10\n20\n30\n40\n50\n", 60);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h3, "a1\na2\n{\na3\na4\na5\na6\na7\na8\n}\na9\na10\na11\na12\n", 42);
    async::receive(h2, "10\n20\n30\n40\n50\n10\n20\n30\n40\n50\n", 30);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
    async::disconnect(h2);
    async::disconnect(h3);

    return 0;
}
