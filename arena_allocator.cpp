#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

constexpr size_t block_size = 64; // Size of each block

class Arena {
    char* buffer;
    size_t capacity, offset;

public:
    Arena(size_t size) : capacity(size), offset(0) {
        buffer = new char[capacity];
    }

    void* allocate(size_t size) {
        if (offset + size > capacity) throw std::bad_alloc();
        void* ptr = buffer + offset;
        offset += size;
        return ptr;
    }

    void reset() { offset = 0; }

    ~Arena() { delete[] buffer; }
};

long long arithmetic_sum(int* data, size_t count) {
    long long sum = 0;
    for (size_t i = 0; i < count; ++i) {
        sum += data[i] * 3 + 7;  // basic arithmetic per element
    }
    return sum;
}

long long arena_arithmetic(int* arena_data, size_t total_int_count) {
    return arithmetic_sum(arena_data, total_int_count);
}

void* touch(void* ptr, size_t size) {
    volatile char* p = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; i += 64) p[i] = rand() % 256; // force page commit
    return ptr;
}

void allocate_and_perform_arithmetic(Arena& arena, size_t count) {
    std::vector<void*> blocks;
    for (size_t i = 0; i < count; ++i) {
        void* block = arena.allocate(block_size);
        blocks.push_back(block);
        touch(block, block_size); // Touch to ensure memory is committed
    }

    long long total_sum = arena_arithmetic(static_cast<int*>(blocks[0]), 1000);
    std::cout << "Total sum: " << total_sum << std::endl;

    // Deallocate blocks
    arena.reset();
}
int main(){
    std::chrono::high_resolution_clock::time_point start, end;
    Arena arena(1000 * block_size); // Single arena for all blocks

    start = std::chrono::high_resolution_clock::now();
    allocate_and_perform_arithmetic(arena, 1000);
    end = std::chrono::high_resolution_clock::now();

    std::cout << "Time taken: " 
              << std::chrono::duration<double>(end - start).count() << "s\n";

    return 0;
}