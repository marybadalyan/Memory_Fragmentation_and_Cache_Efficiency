#include <vector>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <cstring>


class PoolAllocator {
    char* pool;
    size_t block_size, total_blocks;
    std::vector<void*> free_list;

public:
    PoolAllocator(size_t block_size, size_t total_blocks)
        : block_size(block_size), total_blocks(total_blocks) {
        pool = new char[block_size * total_blocks];
        for (size_t i = 0; i < total_blocks; ++i)
            free_list.push_back(pool + i * block_size);
    }

    void* allocate() {
        if (free_list.empty()) throw std::bad_alloc();
        void* ptr = free_list.back();
        free_list.pop_back();
        return ptr;
    }

    void deallocate(void* ptr) {
        free_list.push_back(ptr);
    }

    ~PoolAllocator() { delete[] pool; }
};

long long arithmetic_sum(int* data, size_t count) {
    long long sum = 0;
    for (size_t i = 0; i < count; ++i) {
        sum += data[i] * 3 + 7;  // basic arithmetic per element
    }
    return sum;
}

long long pool_arithmetic(std::vector<void*>& blocks, size_t block_size) {
    long long sum = 0;
    for (void* ptr : blocks) {
        int* data = static_cast<int*>(ptr);
        size_t count = block_size / sizeof(int);
        sum += arithmetic_sum(data, count);
    }
    return sum;
}
void* touch(void* ptr, size_t size) {
    volatile char* p = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; i += 64) p[i] = rand() % 256; // force page commit
    return ptr;
}

void allocate_and_perform_arithmetic(PoolAllocator& allocator, size_t block_size, size_t count) {
    std::vector<void*> blocks;
    for (size_t i = 0; i < count; ++i) {
        void* block = allocator.allocate();
        blocks.push_back(block);
        touch(block, block_size); // Touch to ensure memory is committed
    }

    long long total_sum = pool_arithmetic(blocks, block_size);
    std::cout << "Total sum: " << total_sum << std::endl;

    // Deallocate blocks
    for (void* block : blocks) {
        allocator.deallocate(block);
    }
}
int main(){
    std::chrono::high_resolution_clock::time_point start, end;
    size_t block_size = 64; // Size of each block
    PoolAllocator allocator(block_size, 1000); // 1000 blocks

    start = std::chrono::high_resolution_clock::now();
    allocate_and_perform_arithmetic(allocator, block_size, 1000);
    end = std::chrono::high_resolution_clock::now();

    std::cout << "Time taken: " 
              << std::chrono::duration<double>(end - start).count() << "s\n";

    return 0;
}