#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cstring>
#include <windows.h>

constexpr int allocCount = 100000;
constexpr int blockSize = 128;
constexpr int fixedblockSize = 64; // cache aligned size

HANDLE heap = GetProcessHeap();

long long perform_arithmetic(void* ptr, size_t block_size) {
    long long sum = 0;
    int* int_ptr = static_cast<int*>(ptr);
    for (size_t i = 0; i < block_size / sizeof(int); ++i) {
        sum += int_ptr[i];
    }
    return sum;
}

void* touch(void* ptr, size_t size) {
    volatile char* p = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; i += 64) p[i] = rand() % 256; // commit pages
    return ptr;
}

void* allocateBlock(size_t size) {
    void* block = HeapAlloc(heap, 0, size);
    if (!block) {
        std::cerr << "HeapAlloc failed\n";
        exit(1);
    }
    return block;
}

long long allocatingRandomSizeBlocks() {
    std::vector<void*> blocks(allocCount);
    long long total_sum = 0;

    for (int i = 0; i < allocCount; ++i) {
        size_t size = blockSize + (rand() % 128);
        blocks[i] = allocateBlock(size);
        touch(blocks[i], size);
        total_sum += perform_arithmetic(blocks[i], size);
    }

    // Fragmentation simulation: free half the blocks
    for (int i = 0; i < allocCount; i += 2)
        HeapFree(heap, 0, blocks[i]);
    for (int i = 1; i < allocCount; i += 2)
        HeapFree(heap, 0, blocks[i]);

    return total_sum;
}

long long allocatingFixedSizeBlocks() {
    std::vector<void*> blocks(allocCount);
    long long total_sum = 0;

    for (int i = 0; i < allocCount; ++i) {
        blocks[i] = allocateBlock(fixedblockSize);
        touch(blocks[i], fixedblockSize);
        total_sum += perform_arithmetic(blocks[i], fixedblockSize);
    }

    for (int i = 0; i < allocCount; ++i)
        HeapFree(heap, 0, blocks[i]);

    return total_sum;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    long long sum_random = allocatingRandomSizeBlocks();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Random size blocks total sum: " << sum_random << "\n";
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << " seconds\n";

    start = std::chrono::high_resolution_clock::now();
    long long sum_fixed = allocatingFixedSizeBlocks();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Fixed size blocks total sum: " << sum_fixed << "\n";
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << " seconds\n";

    return 0;
}
