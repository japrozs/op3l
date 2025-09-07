#include "main.h"

struct vm vm;

static void reset_stack()
{
    vm.stack_top = vm.stack;
}

void init_vm(void)
{
    reset_stack();
}

void free_vm(void)
{
}

void push(OP3L_VALUE value)
{
    *vm.stack_top = value;
    vm.stack_top++;
}

OP3L_VALUE pop()
{
    vm.stack_top--;
    return *vm.stack_top;
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
    return vm.chunk->constants.values[constant];
#undef READ_BYTE
}

static enum interpret_result run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)     \
    do {                  \
        double b = pop(); \
        double a = pop(); \
        push(a op b);     \
    } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (OP3L_VALUE* slot = vm.stack; slot < vm.stack_top; slot++) {
            printf("[ ");
            print_value(*slot);
            printf(" ]");
        }
        printf("\n");
        disassemble_instr(vm.chunk,
            (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
        case OP_CONSTANT: {
            OP3L_VALUE constant = READ_CONSTANT();
            push(constant);
            break;
        }
        case OP_CONSTANT_LONG: {
            OP3L_VALUE constant = READ_CONSTANT_LONG();
            push(constant);
            break;
        }
        case OP_ADD:
            BINARY_OP(+);
            break;
        case OP_SUBTRACT:
            BINARY_OP(-);
            break;
        case OP_MULTIPLY:
            BINARY_OP(*);
            break;
        case OP_DIVIDE:
            BINARY_OP(/);
            break;
        case OP_NEGATE:
            push(-pop());
            break;
        case OP_RETURN: {
            print_value(pop());
            printf("\n");
            return INTERPRET_OK;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

enum interpret_result interpret(const char* source)
{
    compile(source);
    return INTERPRET_OK;
}
