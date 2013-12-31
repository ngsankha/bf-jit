#include <stdio.h>
#include <stdlib.h>
#include <jit/jit.h>

struct bf_loop {
    jit_label_t start;
    jit_label_t stop;
    struct bf_loop *parent;
};
typedef struct bf_loop *bf_loop_t;

void loop_start(jit_function_t function, bf_loop_t *loop) {
    bf_loop_t newloop = (bf_loop_t)jit_malloc(sizeof(struct bf_loop));
    newloop->start = jit_label_undefined;
    newloop->stop = jit_label_undefined;
    newloop->parent = *loop;
    jit_insn_label(function, &newloop->start);
    *loop = newloop;
}

void loop_stop(jit_function_t function, bf_loop_t *loop) {
    bf_loop_t curloop = *loop;
    if (curloop == NULL) return;
    jit_insn_branch(function, &curloop->start);
    jit_insn_label(function, &curloop->stop);
    *loop = curloop->parent;
}

jit_function_t bf_compile(jit_context_t cx, FILE *fp) {
    jit_type_t ubyte_ptr, params[1], putchar_params[1], signature, putchar_sig, getchar_sig;
    jit_value_t ptr, uptr, ubyte, tmp;
    bf_loop_t loop = NULL;

    ubyte_ptr = jit_type_create_pointer(jit_type_ubyte, 1);
    params[0] = ubyte_ptr;
    putchar_params[0] = jit_type_ubyte;
    signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, params, 1, 1);
    putchar_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_ubyte, putchar_params, 1, 1);
    getchar_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_ubyte, NULL, 0, 1);
    jit_function_t function = jit_function_create(cx, signature);
    ptr = jit_value_get_param(function, 0);
    uptr = jit_value_create_nint_constant(function, ubyte_ptr, 1);
    ubyte = jit_value_create_nint_constant(function, jit_type_ubyte, 1);

    while(!feof(fp)) {
        char c = fgetc(fp);
        switch(c) {
            case '>':
                tmp = jit_insn_add(function, ptr, uptr);
                jit_insn_store(function, ptr, tmp);
                break;
            case '<':
                tmp = jit_insn_sub(function, ptr, uptr);
                jit_insn_store(function, ptr, tmp);
                break;
            case '+':
                tmp = jit_insn_load_relative(function, ptr, 0, jit_type_ubyte);
                tmp = jit_insn_add(function, tmp, ubyte);
                tmp = jit_insn_convert(function, tmp, jit_type_ubyte, 0);
                jit_insn_store_relative(function, ptr, 0, tmp);
                break;
            case '-':
                tmp = jit_insn_load_relative(function, ptr, 0, jit_type_ubyte);
                tmp = jit_insn_sub(function, tmp, ubyte);
                tmp = jit_insn_convert(function, tmp, jit_type_ubyte, 0);
                jit_insn_store_relative(function, ptr, 0, tmp);
                break;
            case '.':
                tmp = jit_insn_load_relative(function, ptr, 0, jit_type_ubyte);
                jit_insn_call_native(function, "putchar", putchar, putchar_sig, &tmp, 1, JIT_CALL_NOTHROW);
                break;
            case ',':
                jit_insn_call_native(function, "getchar", getchar, getchar_sig, NULL, 0, JIT_CALL_NOTHROW);
                jit_insn_store_relative(function, ptr, 0, tmp);
                break;
            case '[':
                loop_start(function, &loop);
                tmp = jit_insn_load_relative(function, ptr, 0, jit_type_ubyte);
                jit_insn_branch_if_not(function, tmp, &loop->stop);
                break;
            case ']':
                loop_stop(function, &loop);
                break;
        }
    }

    jit_insn_return(function, NULL);
    jit_function_compile(function);

    return function;
}

int main(int argc, char const *argv[]) {
    jit_context_t cx = jit_context_create();

    char *data = calloc(50000, sizeof(char));
    jit_ptr arg1;
    void *args[1];

    FILE *fp = fopen(argv[1], "rb");
    jit_function_t function = bf_compile(cx, fp);
    fclose(fp);
    jit_context_build_end(cx);
    arg1 = data;
    args[0] = &arg1;
    jit_function_apply(function, args, NULL);
    jit_context_destroy(cx);
    return 0;
}