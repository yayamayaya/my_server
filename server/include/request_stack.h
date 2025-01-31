#ifndef REQUEST_STACK_H
#define REQUEST_STACK_H

#include "debugging.h"
#include "connection_work_func.h"

#define INITIAL_NUMBER_OF_REQUESTS 5

typedef struct work_req_stack req_stack;
typedef struct req_Ops req_op_table;

typedef struct req_Ops
{
    ret_t (*add_request)(req_stack *self, sockd_t req_socket);
    sockd_t (*pop_req)(req_stack *self);

    void (*dump)(req_stack *self);

    no_ret_val_t (*stack_destructor)(req_stack *self);
} req_op_table;

typedef struct work_req_stack
{
    sockd_t *requests;

    unsigned long int req_number;
    unsigned long int stack_size;

    req_op_table methods;
} req_stack;

ret_t init_req_stack(req_stack **stk_ptr);

#endif