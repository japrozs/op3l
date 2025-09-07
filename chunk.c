#include "main.h"

void init_chunk(struct chunk_t* chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    init_value_array(&chunk->constants);
}

void write_chunk(struct chunk_t* chunk, uint8_t byte, int line)
{
    if (chunk->capacity < chunk->count + 1) {
        int old_capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
            old_capacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines,
            old_capacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void* reallocate(void* pointer, [[maybe_unused]] size_t old_size, size_t new_size)
{
    if (new_size == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, new_size);
    if (result == NULL)
        exit(1);
    return result;
}

void free_chunk(struct chunk_t* chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}

int add_constant(struct chunk_t* chunk, OP3L_VALUE value)
{
    write_value_array(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void write_constant(struct chunk_t* chunk, OP3L_VALUE value, int line)
{
    int index = add_constant(chunk, value);
    if (index < 256) {
        write_chunk(chunk, OP_CONSTANT, line);
        write_chunk(chunk, (uint8_t)index, line);
    } else {
        write_chunk(chunk, OP_CONSTANT_LONG, line);
        // Wwite low/mid/high bytes of index
        write_chunk(chunk, (index >> 0) & 0xFF, line);
        write_chunk(chunk, (index >> 8) & 0xFF, line);
        write_chunk(chunk, (index >> 16) & 0xFF, line);
    }
}
