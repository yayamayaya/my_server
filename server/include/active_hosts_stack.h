#ifndef ACTIVE_HOSTS_STACK_H
#define ACTIVE_HOSTS_STACK_H

#include <poll.h>
#include "debugging.h"

/*
struct host*(pollfd)*
{
    int fd;
    short events;
    short revents;
}
*/

#define INITIAL_HOSTS_STACK_SIZE 4

typedef struct pollfd_stack host_stack;
typedef struct host_Ops host_op_table;

typedef struct host_Ops
{
    ret_t (*add_host)(host_stack *self, struct pollfd new_host);
    no_ret_val_t (*rm_host)(host_stack *self, int fd_num);

    void (*dump)(host_stack *self);

    no_ret_val_t (*stack_destructor)(host_stack *self);
} host_op_table;

typedef struct pollfd_stack
{
    struct pollfd *hosts;

    unsigned long int hosts_number;
    unsigned long int stack_size;

    host_op_table methods;
} host_stack;

ret_t init_host_stack(host_stack **stk_ptr);

#endif