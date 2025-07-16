#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <malloc.h> // mallinfo2 for fragmentation stats
#endif

constexpr int allocCount = 100000;
constexpr int blockSize = 128;

#ifdef _WIN32
HANDLE heap = GetProcessHeap();
#endif

void* touch(void* ptr, size_t size) {
    volatile char* p = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; i += 64) p[i] = 0; // force page commit
    return ptr;
}

int main() {
    std::vector<void*> blocks(allocCount);

    // Allocate blocks
#ifdef _WIN32
    for (int i = 0; i < allocCount; ++i)
        blocks[i] = HeapAlloc(heap, 0, blockSize + (rand() % 128));

    for (int i = 0; i < allocCount; i += 2)
        HeapFree(heap, 0, blocks[i]); // free half → fragmentation
#else
    for (int i = 0; i < allocCount; ++i) {
        blocks[i] = malloc(blockSize + (rand() % 128));
        touch(blocks[i], blockSize + (rand() % 128));
    }

    for (int i = 0; i < allocCount; i += 2)
        free(blocks[i]); // free half
#endif

    // Fragmentation Stats (Linux only)
#ifndef _WIN32
    struct mallinfo2 mi = mallinfo2();
    std::cout << "Total allocated: " << mi.uordblks << " bytes\n";
    std::cout << "Free memory (free fragments): " << mi.fordblks << " bytes\n";
#endif

    // Cleanup remaining
#ifdef _WIN32
    for (int i = 1; i < allocCount; i += 2)
        HeapFree(heap, 0, blocks[i]);
#else
    for (int i = 1; i < allocCount; i += 2)
        free(blocks[i]);
#endif

    return 0;
}
