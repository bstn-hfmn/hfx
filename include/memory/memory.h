#ifndef HFX_MEMORY_H
#define HFX_MEMORY_H

#include "common.h"

#define HFX_ALLOC(x) HFX_MemoryArenaAlloc(x)

#define HFX_MEMORY_ARENA_ALIGNMENT 8
#define HFX_MEMORY_ARENA_CHUNK_SIZE 4096

struct ARENA_CHUNK {
  uint8_t* memory;
  usize offset;

  struct ARENA_CHUNK* next;
};

struct ARENA {
  usize chunks;
  struct ARENA_CHUNK* head;
  struct ARENA_CHUNK* current;
};

typedef struct ARENA *PARENA, ARENA_T;
typedef struct ARENA_CHUNK *PARENA_CHUNK, ARENA_CHUNK_T;

void                  HFX_MemoryArenaInit();
void*                 HFX_MemoryArenaAlloc(
    usize size);

void                  HFX_MemoryArenaFree();

#endif //HFX_MEMORY_H
