#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>


constexpr size_t block_size = 64; // Size of each block
template <typename T>
class SlabAllocator {
    struct Slab {
        T* data;
        size_t count, used;
        Slab(size_t count) : count(count), used(0) {
            data = new T[count * block_size];
        }
        ~Slab() { delete[] data; }
    };

    std::vector<Slab*> slabs;
    size_t slab_size;

public:
    SlabAllocator(size_t slab_size) : slab_size(slab_size) {}

    T* allocate() {
        if (slabs.empty() || slabs.back()->used == slab_size)
            slabs.push_back(new Slab(slab_size));

        Slab* current = slabs.back();
        T* ptr = &current->data[current->used * block_size];
        ++current->used;
        return ptr;
    }

    ~SlabAllocator() {
        for (auto slab : slabs) delete slab;
    }
};

long long arithmetic_sum(int* data, size_t count) {
    long long sum = 0;
    for (size_t i = 0; i < count; ++i) {
        sum += data[i] * 3 + 7;  // basic arithmetic per element
    }
    return sum;
}

long long slab_arithmetic(std::vector<void*>& objects, size_t object_int_count) {
    long long sum = 0;
    for (void* obj : objects) {
        int* data = static_cast<int*>(obj);
        sum += arithmetic_sum(data, object_int_count);
    }
    return sum;
}


void* touch(void* ptr, size_t size) {
    volatile char* p = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; i += 64) p[i] = rand() % 256; // force page commit
    return ptr;
}

void allocate_and_perform_arithmetic(SlabAllocator<int>& slab, size_t count) {
    std::vector<void*> blocks;
    for (size_t i = 0; i < count; ++i) {
        void* block = slab.allocate();
        blocks.push_back(block);
        touch(block, block_size); // Touch to ensure memory is committed
    }

    long long total_sum = slab_arithmetic(blocks, block_size);
    std::cout << "Total sum: " << total_sum << std::endl;

}
int main(){
    std::chrono::high_resolution_clock::time_point start, end;
    SlabAllocator<int> slab(block_size); // 256 objects per slab

    start = std::chrono::high_resolution_clock::now();
    allocate_and_perform_arithmetic(slab, 1000);
    end = std::chrono::high_resolution_clock::now();

    std::cout << "Time taken: " 
              << std::chrono::duration<double>(end - start).count() << "s\n";

    return 0;
}