#include "main.h"

void interpret_file([[maybe_unused]] const char* file_path)
{
    // test the f*ckin interpreter here
    // init_vm();
    // struct chunk chunk;
    // init_chunk(&chunk);

    // write_constant(&chunk, 1.2, 123);
    // write_constant(&chunk, 3.4, 123);
    // write_chunk(&chunk, OP_ADD, 123);
    // write_constant(&chunk, 5.6, 123);
    // write_chunk(&chunk, OP_DIVIDE, 123);
    // write_chunk(&chunk, OP_NEGATE, 123);
    // write_chunk(&chunk, OP_RETURN, 123);

    // disassemble_chunk(&chunk, "test chunk");
    // interpret(&chunk);
    // free_vm();
    // free_chunk(&chunk);

    char* source = read_file(file_path);
    enum interpret_result result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR)
        exit(65);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit(70);
}
