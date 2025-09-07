#include "main.h"

void init_value_array(struct value_array_t* array)
{
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void write_value_array(struct value_array_t* array, OP3L_VALUE value)
{
    if (array->capacity < array->count + 1) {
        int old_capacity = array->capacity;
        array->capacity = GROW_CAPACITY(old_capacity);
        array->values = GROW_ARRAY(OP3L_VALUE, array->values,
            old_capacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void free_value_array(struct value_array_t* array)
{
    FREE_ARRAY(OP3L_VALUE, array->values, array->capacity);
    init_value_array(array);
}

void print_value(OP3L_VALUE value)
{
    printf("%g", value);
}
