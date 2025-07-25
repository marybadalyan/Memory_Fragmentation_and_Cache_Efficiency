## 📄 **README: Custom Memory Allocator Comparison**

### 🚀 **Project Overview**

This project demonstrates and benchmarks **custom memory allocators**:

* **Pool Allocator**
* **Arena Allocator**
* **Slab Allocator**

These allocator designs help manage memory more efficiently than standard `malloc`/`new`, especially in performance-critical systems like game engines, embedded systems, or high-performance services.

We compare performance via:

* Allocation speed
* Arithmetic operation speed on allocated memory
* Cache locality and fragmentation impact


The goal is to understand how these allocators manage memory **more efficiently** in specific scenarios by:

* **Reducing memory fragmentation**
* **Improving cache locality**
* **Minimizing allocation overhead**

This project provides **clean, isolated implementations** of each allocator and includes **simple arithmetic workloads** to test their practical impact on allocation speed and memory access performance.

The project also includes **build scripts** (CMake) to make it portable across **Linux (GCC/Clang)** and **Windows (MSVC)**, along with profiling instructions for analyzing performance using tools like `perf`, `valgrind`, and **Visual Studio Profiler**.

---

### 📚 **What Are These Allocators?**

| Allocator           | Description                                                                                      | Use Cases                                 |
| ------------------- | ------------------------------------------------------------------------------------------------ | ----------------------------------------- |
| **Pool Allocator**  | Allocates **fixed-size small objects** from a pre-allocated pool. Fast, O(1) allocations.        | Small, frequent allocations, object pools |
| **Arena Allocator** | Allocates **large chunks (arenas)** and performs **linear allocations** within them.             | Fast bulk allocations, resets in bulk     |
| **Slab Allocator**  | Organizes memory into **slabs** of blocks; useful for managing objects of similar type and size. | Kernel allocators, caching, object reuse  |

---

### ⚙️ **Build Instructions**

```bash
# Clone the repository
git clone Memory_Fragmentation_and_Cache_Efficiency
cd Memory_Fragmentation_and_Cache_Efficiency
# Create build directory and compile
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/... /...

```
### 🚀 **Running the Programs**

| Executable        | Description                          | Example Command     |
| ----------------- | ------------------------------------ | ------------------- |
| `main`            | General performance runner           | `./main`            |
| `arena_allocator` | Arena allocator allocation benchmark | `./arena_allocator` |
| `slab_allocator`  | Slab allocator benchmark             | `./slab_allocator`  |
| `pool_allocator`  | Pool allocator benchmark             | `./pool_allocator`  |

---