#include "main.h"

struct vm vm;

void init_vm()
{
}

void free_vm()
{
}

OP3L_VALUE READ_CONSTANT_LONG()
{
#define READ_BYTE() (*vm.ip++)
    uint8_t low = READ_BYTE();
    uint8_t mid = READ_BYTE();
    uint8_t high = READ_BYTE();
    uint32_t constant = ((uint32_t)high << 16)
        | ((uint32_t)mid << 8)
        | (uint32_t)low;
    return constant;
#undef READ_BYTE
}

static enum interpret_result run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        disassemble_instr(vm.chunk,
            (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
        case OP_CONSTANT: {
            OP3L_VALUE constant = READ_CONSTANT();
            print_value(constant);
            printf("\n");
            break;
        }
        case OP_CONSTANT_LONG: {
            uint32_t constant = READ_CONSTANT_LONG();
            printf("%u", constant);
            printf("\n");
            break;
        }
        case OP_RETURN: {
            return INTERPRET_OK;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}

enum interpret_result interpret(struct chunk* chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}
