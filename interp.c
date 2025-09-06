#include "main.h"

void interpret_file([[maybe_unused]] const char* file_path)
{
    // test the f*ckin interpreter here
    init_vm();
    struct chunk chunk;
    init_chunk(&chunk);

    for (int i = 0; i < 300; i++) {
        write_constant(&chunk, i + 1, 1);
    }
    write_chunk(&chunk, OP_RETURN, 123);

    disassemble_chunk(&chunk, "test chunk");
    interpret(&chunk);
    free_vm();
    free_chunk(&chunk);
}
