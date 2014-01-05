#ifndef BF_JIT

#define BF_JIT

#include <stdio.h>
#include <stdlib.h>
#include <jit/jit.h>

struct bf_loop {
    jit_label_t start;
    jit_label_t stop;
    struct bf_loop *parent;
};
typedef struct bf_loop *bf_loop_t;

typedef struct ops {
    char token;
    int count;
} ops;

void loop_start(jit_function_t, bf_loop_t*);
void loop_stop(jit_function_t, bf_loop_t*);
jit_function_t bf_compile(jit_context_t, FILE*);
void emitOpcodes(jit_function_t, jit_value_t, ops*);
void emitIncrement(jit_function_t, jit_value_t, ops*);
void emitDecrement(jit_function_t, jit_value_t, ops*);
void emitIncrementPtr(jit_function_t, jit_value_t, ops*);
void emitDecrementPtr(jit_function_t, jit_value_t, ops*);
jit_type_t ubyte_ptr;

#define OPTIMIZE_TOKEN(x) if(unit->token == x)        \
        unit->count++;                                  \
    else {                                              \
        emitOpcodes(function, ptr, unit);               \
        unit->token = x;                              \
        unit->count = 1;                                \
    }                                                   \
    break;

#endif