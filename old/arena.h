#ifndef CC_ARENA_H
#define CC_ARENA_H

#include <stddef.h>
#include <stdint.h>

/// An arena block.
typedef struct ArenaBlock ArenaBlock;

/// An arena.
typedef struct {
  uint8_t *ptr;          // Pointer to the current block
  size_t default_size;   // Default block size
  size_t commited_size;  // Current block committed size
  size_t allocated_size; // Current block allocated size
  ArenaBlock *used;      // Used blocks
  ArenaBlock *free;      // Free blocks
} Arena;

/// Create an arena with a custom default block size.
/// @param default_size Default block size.
Arena arena_create(size_t default_size);

/// Allocate memory from the arena.
/// @param arena The arena.
void *arena_alloc(Arena *arena, size_t size);

/// Allocate zeroed memory from the arena.
/// @param arena The arena.
void *aren_calloc(Arena *arena, size_t size);

/// Clear the arena without freeing memory.
/// @param arena The arena to clear.
void arena_clear(Arena *arena);

/// Garbage collect the arena.
/// @param arena The arena to garbage collect.
void arena_garbage_collect(Arena *arena);

/// Undo the last allocation.
/// @param arena The arena.
/// @param size The size of the allocation.
void arena_undo(Arena *arena, size_t size);

/// Get the total number of bytes allocated.
/// @param arena The arena.
size_t arena_total_bytes_allocated(const Arena *arena);

/// Get the total number of blocks allocated.
/// @param arena The arena.
uint32_t arena_total_blocks_allocated(const Arena *arena);

/// Destroy the arena and free all memory.
/// @param arena The arena to destroy.
void arena_destroy(Arena *arena);

#endif // CC_ARENA_H