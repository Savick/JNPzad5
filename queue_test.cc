#include <iostream>
#include <exception>
#include <cassert>

#include "PriorityQueue.hh"

int main() {
PriorityQueue<int, int> Q;
assert(Q.empty());

Q.insert(1, 42);
Q.insert(2, 13);

assert(Q.size() == 2);
assert(Q.maxKey() == 1);
assert(Q.maxValue() == 42);
assert(Q.minKey() == 2);
assert(Q.minValue() == 13);

Q.deleteMax();
Q.deleteMin();
Q.deleteMin();

assert(Q.empty());

Q.insert(1, 100);
Q.insert(2, 100);
Q.insert(3, 300);

try {
Q.changeValue(4, 400);
} catch (PriorityQueueNotFoundException const & pqnfex) {
std::cout << pqnfex.what() << std::endl;
} catch (...) {
assert(! "exception missing!");
}

Q.changeValue(2, 200);

try {
while (true) {
std::cout << Q.minValue() << std::endl;
Q.deleteMin();
}
assert(! "Q.minValue() on empty Q did not throw!");
} catch (PriorityQueueEmptyException const & pqeex) {
std::cout << pqeex.what() << std::endl;
} catch (...) {
assert(! "exception missing!");
}

try {
PriorityQueue<int, int>::key_type k = Q.minKey();
assert(! "Q.minKey() on empty Q did not throw!");
} catch (std::exception const & ex) {
std::cout << ex.what() << std::endl;
} catch (...) {
assert(! "exception missing!");
}

PriorityQueue<int, int> T;
T.insert(1, 1);
T.insert(2, 4);
Q.insert(3, 9);
Q.insert(4, 16);
Q.merge(T);
assert(Q.size() == 4);
assert(Q.minValue() == 1);
assert(Q.maxValue() == 16);
assert(T.empty());

PriorityQueue<int, int> S = Q;
swap(Q, T);
assert(T == S);

std::cout << "ALL OK!" << std::endl;
return 0;
}
