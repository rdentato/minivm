#pragma once

#include "libc.h"
typedef uint8_t reg_t;
typedef char vm_opcode_t;

#include "obj.h"

enum vm_opcode_t
{
    VM_OPCODE_EXIT,
    VM_OPCODE_STORE_REG,
    VM_OPCODE_STORE_BYTE,
    VM_OPCODE_STORE_INT,
    VM_OPCODE_STORE_FUN,
    VM_OPCODE_FUN_DONE,
    VM_OPCODE_EQUAL,
    VM_OPCODE_EQUAL_NUM,
    VM_OPCODE_NOT_EQUAL,
    VM_OPCODE_NOT_EQUAL_NUM,
    VM_OPCODE_LESS,
    VM_OPCODE_LESS_NUM,
    VM_OPCODE_GREATER,
    VM_OPCODE_GREATER_NUM,
    VM_OPCODE_LESS_THAN_EQUAL,
    VM_OPCODE_LESS_THAN_EQUAL_NUM,
    VM_OPCODE_GREATER_THAN_EQUAL,
    VM_OPCODE_GREATER_THAN_EQUAL_NUM,
    VM_OPCODE_JUMP_ALWAYS,
    VM_OPCODE_JUMP_IF_FALSE,
    VM_OPCODE_JUMP_IF_TRUE,
    VM_OPCODE_JUMP_IF_EQUAL,
    VM_OPCODE_JUMP_IF_EQUAL_NUM,
    VM_OPCODE_JUMP_IF_NOT_EQUAL,
    VM_OPCODE_JUMP_IF_NOT_EQUAL_NUM,
    VM_OPCODE_JUMP_IF_LESS,
    VM_OPCODE_JUMP_IF_LESS_NUM,
    VM_OPCODE_JUMP_IF_GREATER,
    VM_OPCODE_JUMP_IF_GREATER_NUM,
    VM_OPCODE_JUMP_IF_LESS_THAN_EQUAL,
    VM_OPCODE_JUMP_IF_LESS_THAN_EQUAL_NUM,
    VM_OPCODE_JUMP_IF_GREATER_THAN_EQUAL,
    VM_OPCODE_JUMP_IF_GREATER_THAN_EQUAL_NUM,
    VM_OPCODE_INC,
    VM_OPCODE_INC_NUM,
    VM_OPCODE_DEC,
    VM_OPCODE_DEC_NUM,
    VM_OPCODE_ADD,
    VM_OPCODE_ADD_NUM,
    VM_OPCODE_SUB,
    VM_OPCODE_SUB_NUM,
    VM_OPCODE_MUL,
    VM_OPCODE_MUL_NUM,
    VM_OPCODE_DIV,
    VM_OPCODE_DIV_NUM,
    VM_OPCODE_MOD,
    VM_OPCODE_MOD_NUM,
    VM_OPCODE_CONCAT,
    VM_OPCODE_STATIC_CALL0,
    VM_OPCODE_STATIC_CALL1,
    VM_OPCODE_STATIC_CALL2,
    VM_OPCODE_STATIC_CALL,
    VM_OPCODE_REC0,
    VM_OPCODE_REC1,
    VM_OPCODE_REC2,
    VM_OPCODE_REC,
    VM_OPCODE_CALL0,
    VM_OPCODE_CALL1,
    VM_OPCODE_CALL2,
    VM_OPCODE_CALL,
    VM_OPCODE_RETURN,
    VM_OPCODE_PUTCHAR,
    VM_OPCODE_ARRAY_NEW,
    VM_OPCODE_STRING_NEW,
    VM_OPCODE_LENGTH,
    VM_OPCODE_INDEX_GET,
    VM_OPCODE_INDEX_SET,
    VM_OPCODE_TYPE,
    VM_OPCODE_MAX1,
    VM_OPCODE_MAX2P = 128,
};

typedef enum
{
    VM_TYPE_NUMBER = 0,
    VM_TYPE_FUNCTION = 1,
    VM_TYPE_ARRAY = 2,
    VM_TYPE_STRING = 3,
} vm_type_t;

typedef struct
{
    int index;
    int func;
    int bytecode;
    uint8_t outreg;
    void *locals;
} vm_stack_frame_t;

void vm_run(int len, const vm_opcode_t *mem);
