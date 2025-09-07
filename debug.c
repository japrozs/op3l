#include "main.h"

void disassemble_chunk(struct chunk_t* chunk, const char* name)
{
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassemble_instr(chunk, offset);
    }
}

static int simple_instr(const char* name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

static int constant_instr(const char* name, struct chunk_t* chunk, int offset)
{
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %6u '", name, constant);
    print_value(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

static int constant_long_instr(const char* name, struct chunk_t* chunk, int offset)
{
    uint8_t low = chunk->code[offset + 1];
    uint8_t mid = chunk->code[offset + 2];
    uint8_t high = chunk->code[offset + 3];
    uint32_t constant = ((uint32_t)high << 16)
        | ((uint32_t)mid << 8)
        | (uint32_t)low;
    printf("%-16s %6u '", name, constant);
    print_value(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 4;
}

int disassemble_instr(struct chunk_t* chunk, int offset)
{
    printf("%04d ", offset);

    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_CONSTANT:
        return constant_instr("OP_CONSTANT", chunk, offset);
    case OP_CONSTANT_LONG:
        return constant_long_instr("OP_CONSTANT_LONG", chunk, offset);
    case OP_ADD:
        return simple_instr("OP_ADD", offset);
    case OP_SUBTRACT:
        return simple_instr("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
        return simple_instr("OP_MULTIPLY", offset);
    case OP_DIVIDE:
        return simple_instr("OP_DIVIDE", offset);
    case OP_NEGATE:
        return simple_instr("OP_NEGATE", offset);
    case OP_RETURN:
        return simple_instr("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
