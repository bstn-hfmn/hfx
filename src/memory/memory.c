#include "memory/memory.h"

#include <stdlib.h>

static struct ARENA g_Arena;

void HFX_MemoryArenaInit()
{
    g_Arena.chunks = 1;
    g_Arena.current = malloc(sizeof(struct ARENA_CHUNK));
    g_Arena.current->memory = calloc( HFX_MEMORY_ARENA_CHUNK_SIZE, sizeof(uint8_t));
    g_Arena.current->offset = 0;
    g_Arena.current->next = nullptr;
    g_Arena.head = g_Arena.current;
}

void* HFX_MemoryArenaAlloc(
    const usize size)
{
    if (!g_Arena.current)
        return nullptr;

    const uintptr_t base = (uintptr_t)(g_Arena.current->memory + g_Arena.current->offset);
    usize aligned_offset
        = (usize)((base + (HFX_MEMORY_ARENA_ALIGNMENT - 1)) & ~(uintptr_t)(HFX_MEMORY_ARENA_ALIGNMENT - 1))
        - (uintptr_t)g_Arena.current->memory;

    if (g_Arena.current->offset != aligned_offset)
        g_Arena.current->offset = aligned_offset;

    if (aligned_offset + size > HFX_MEMORY_ARENA_CHUNK_SIZE)
    {
        struct ARENA_CHUNK* chunk = malloc(sizeof(struct ARENA_CHUNK));
        chunk->memory = calloc( HFX_MEMORY_ARENA_CHUNK_SIZE, sizeof(uint8_t));
        chunk->next = nullptr;
        chunk->offset = size;

        g_Arena.current->next = chunk;
        g_Arena.current = chunk;
        aligned_offset = 0;
    }
    else
    {
        g_Arena.current->offset += size;
    }

    return g_Arena.current->memory + aligned_offset;
}

void HFX_MemoryArenaFree()
{
    struct ARENA_CHUNK* chunk = g_Arena.head;
    while (chunk)
    {
        struct ARENA_CHUNK* next = chunk->next;
        HFX_FREE(chunk->memory);
        HFX_FREE(chunk);
        chunk = next;
    }

    g_Arena.head = nullptr;
    g_Arena.current = nullptr;
}