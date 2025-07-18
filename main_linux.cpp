#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cstring>
#include <unistd.h>  // sbrk
#include <malloc.h>  // mallinfo2

constexpr int allocCount = 100000;
constexpr int blockSize = 128;
constexpr int fixedblockSize = 64;

void* sbrkAllocate(size_t size) {
    void* ptr = sbrk(size);
    if (ptr == (void*) -1) {
        std::cerr << "sbrk() failed\n";
        exit(1);
    }
    return ptr;
}

void touch(void* ptr, size_t size) {
    volatile char* p = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; i += 64) p[i] = rand() % 256;
}

long long perform_arithmetic(int* ptr, size_t count) {
    long long sum = 0;
    for (size_t i = 0; i < count; ++i) sum += ptr[i] * 3 + 7;
    return sum;
}

long long random_sbrk_test() {
    std::vector<void*> blocks(allocCount);
    size_t total_sum = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < allocCount; ++i) {
        size_t size = blockSize + (rand() % 128);
        blocks[i] = sbrkAllocate(size);
        touch(blocks[i], size);

        // Perform arithmetic inside the block
        size_t count = size / sizeof(int);
        total_sum += perform_arithmetic(static_cast<int*>(blocks[i]), count);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "[SBRK] Sum=" << total_sum << ", Time=" 
              << std::chrono::duration<double>(end - start).count() << "s\n";
    return total_sum;
}

long long fixed_sbrk_test() {
    std::vector<void*> blocks(allocCount);
    long long total_sum = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < allocCount; ++i) {
        blocks[i] = sbrkAllocate(fixedblockSize);
        touch(blocks[i], fixedblockSize);

        size_t count = fixedblockSize / sizeof(int);
        total_sum += perform_arithmetic(static_cast<int*>(blocks[i]), count);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "[Fixed SBRK] Sum=" << total_sum << ", Time=" 
              << std::chrono::duration<double>(end - start).count() << "s\n";
    return total_sum;
}

int main() {
    random_sbrk_test();
    fixed_sbrk_test();
    return 0;
}
