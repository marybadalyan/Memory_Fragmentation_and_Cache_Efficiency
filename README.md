1. Pool Allocator — Focus on Fixed Size, High-Frequency Allocations

A Pool Allocator manages a pool (preallocated block) of fixed-size memory chunks. All allocations are of the same size, and allocation/freeing is extremely fast because it just pulls from or returns to a free list.
General-purpose allocators like malloc() are slow and cause fragmentation when dealing with many small, same-sized allocations.
Pools optimize for speed and predictability by preallocating space for many objects.
Only works efficiently when you allocate same-size objects.



2. Arena Allocator — Focus on Bulk Allocation and Fast Reset

An Arena Allocator preallocates a large block of memory (the arena) and handles all allocations linearly within it. You don’t free individual allocations — you reset the whole arena at once.
Ideal for short-lived objects in batch-like processing where you can discard everything at once.
Removes overhead of tracking individual allocations/frees.
Cannot selectively free objects; all are freed together.


Slab Allocator — Focus on Grouping Same-Type Objects Efficiently
What is it?
A Slab Allocator organizes memory into slabs, each of which contains many objects of the same type/size. Within a slab, objects are allocated and freed individually, but the memory layout is optimized to reuse freed space effectively.
Prevents fragmentation by allocating objects in fixed-size blocks.

Best suited for objects of consistent size; managing slabs of different types can be complex.