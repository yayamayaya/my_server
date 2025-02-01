#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "request_stack.h"
//#include <unistd.h>
//#include <string.h>
#include "debugging.h"

no_ret_val_t req_stack_destructor(req_stack *self);

static ret_t add_req(req_stack *self, sockd_t new_req_sock);

static ret_t increase_stack_size(req_stack *self);

sockd_t pop_req(req_stack *self);

void req_stk_dump(req_stack *self);


ret_t init_req_stack(req_stack **stk_ptr)
{
    assert(stk_ptr);

    req_stack *stack = (req_stack *)calloc(1, sizeof(req_stack));
    _RETURN_ON_TRUE(!stack, REQ_STACK_INIT_ERR, LOG("}> couldn't allocate memory for host stack\n"););

    stack->requests  = (sockd_t *)calloc(INITIAL_NUMBER_OF_REQUESTS, sizeof(sockd_t));
    _RETURN_ON_TRUE(!stack->requests, REQ_STACK_MEM_ALC_ERR, LOG("}> couldn't allocate memory for pollfd array\n"););

    stack->req_number     = 0;
    stack->stack_size     = INITIAL_NUMBER_OF_REQUESTS;

    stack->methods.add_request        = add_req;
    stack->methods.pop_req            = pop_req;
    stack->methods.stack_destructor   = req_stack_destructor;
    stack->methods.dump               = req_stk_dump;

    *stk_ptr = stack;

    return 0;
}

no_ret_val_t req_stack_destructor(req_stack *self)
{
    assert(self);

    self->stack_size    = 0;
    self->req_number  = 0;

    free(self->requests);
    self->requests = NULL;

    free(self);

    return;
}

ret_t increase_stack_size(req_stack *self)
{
    assert(self);

    LOG("}> increasing request stack size\n");

    self->stack_size <<= 2;
    sockd_t *arr_holder = (sockd_t *)realloc(self->requests, self->stack_size * sizeof(sockd_t));
    _RETURN_ON_TRUE(!arr_holder, HOST_ARR_REALLC_ERR, LOG("}> couldn't increase memory for pollfd arr\n"); self->stack_size >>= 2;);

    self->requests = arr_holder;

    LOG("}> success\n");

    return 0;
}

ret_t add_req(req_stack *self, sockd_t new_req_sock)
{
    assert(self);

    ret_t ret_val = 0;

    if (self->req_number == self->stack_size)
    {
        ret_val = increase_stack_size(self);
        _RETURN_ON_TRUE(ret_val, ret_val);
    }

    self->requests[self->req_number] = new_req_sock;
    self->req_number++;

    return 0;
}

sockd_t pop_req(req_stack *self)
{
    assert(self);

    sockd_t current_req_socket = self->requests[0];

    self->req_number--;
    memmove(self->requests, self->requests + 1, self->req_number);
    
    return current_req_socket;
}

void req_stk_dump(req_stack *self)
{
    assert(self);

    LOG("}> --------------------------------------------------------------------------\n");
    LOG("}> HOST stack dump: \n");
    LOG("}> stack size: %lu\n", self->stack_size);
    LOG("}> number of hosts: %lu\n", self->req_number);

    for (unsigned long int i = 0; i < self->req_number; i++)
    {
        LOG("}> -------------------------------------\n");
        LOG("}> host number: %lu\n", i);
        LOG("}> fd: %d\n", self->requests[i]);
    }
    
    LOG("}> dump ended\n");
    LOG("}> --------------------------------------------------------------------------\n");

    return;
}