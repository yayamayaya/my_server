#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "active_hosts_stack.h"
//#include <unistd.h>
//#include <string.h>
#include "debugging.h"

no_ret_val_t host_stack_destructor(host_stack *self);

static ret_t add_host(host_stack *self, struct pollfd host);

static ret_t increase_stack_size(host_stack *self);

no_ret_val_t rm_host(host_stack *self, int fd_num);

void host_stk_dump(host_stack *self);


ret_t init_host_stack(host_stack **stk_ptr)
{
    assert(stk_ptr);

    host_stack *stack = (host_stack *)calloc(1, sizeof(host_stack));
    _RETURN_ON_TRUE(!stack, HOST_STACK_INIT_ERR, LOG("}> couldn't allocate memory for host stack\n"););

    stack->hosts  = (struct pollfd *)calloc(INITIAL_HOSTS_STACK_SIZE, sizeof(struct pollfd));
    _RETURN_ON_TRUE(!stack->hosts, HOST_ARR_INIT_ERR, LOG("}> couldn't allocate memory for pollfd array\n"););

    stack->hosts_number   = 0;
    stack->stack_size     = INITIAL_HOSTS_STACK_SIZE;

    stack->methods.add_host           = add_host;
    stack->methods.rm_host            = rm_host;
    stack->methods.stack_destructor   = host_stack_destructor;
    stack->methods.dump               = host_stk_dump;

    *stk_ptr = stack;

    return 0;
}

no_ret_val_t host_stack_destructor(host_stack *self)
{
    assert(self);

    self->stack_size    = 0;
    self->hosts_number  = 0;

    free(self->hosts);
    self->hosts = NULL;

    free(self);

    return;
}

ret_t increase_stack_size(host_stack *self)
{
    assert(self);

    LOG("}> increasing user staack size\n");

    self->stack_size <<= 2;
    struct pollfd *base_holder = (struct pollfd *)realloc(self->hosts, self->stack_size * sizeof(struct pollfd));
    _RETURN_ON_TRUE(!base_holder, HOST_ARR_REALLC_ERR, LOG("}> couldn't increase memory for pollfd arr\n"); self->stack_size >>= 2;);

    self->hosts = base_holder;

    LOG("}> success\n");

    return 0;
}

ret_t add_host(host_stack *self, struct pollfd new_host)
{
    assert(self);

    ret_t ret_val = 0;

    if (self->hosts_number == self->stack_size)
    {
        ret_val = increase_stack_size(self);
        _RETURN_ON_TRUE(ret_val, ret_val);
    }

    self->hosts[self->hosts_number] = new_host;
    self->hosts_number++;

    return 0;
}

no_ret_val_t rm_host(host_stack *self, int fd_num)
{
    assert(self);

    long unsigned int host_counter = 0;
    while (self->hosts[host_counter].fd != fd_num && host_counter < self->hosts_number)
        host_counter++;
    
    self->hosts_number--;
    memcpy(&self->hosts[host_counter], &self->hosts[self->hosts_number], sizeof(struct pollfd));
    memset(&self->hosts[self->hosts_number], 0, sizeof(struct pollfd));

    return;
}

void host_stk_dump(host_stack *self)
{
    assert(self);

    LOG("}> --------------------------------------------------------------------------\n");
    LOG("}> HOST stack dump: \n");
    LOG("}> stack size: %lu\n", self->stack_size);
    LOG("}> number of hosts: %lu\n", self->hosts_number);

    for (unsigned long int i = 0; i < self->hosts_number; i++)
    {
        LOG("}> -------------------------------------\n");
        LOG("}> host number: %lu\n", i);
        LOG("}> fd: %s\n", self->hosts[i].fd);
    }
    
    LOG("}> dump ended\n");
    LOG("}> --------------------------------------------------------------------------\n");

    return;
}

