#ifndef USER_WORK_FUNCS
#define USER_WORK_FUNCS

#include "debugging.h"
#include "data_base_stack.h"
#include "request_stack.h"
#include "active_hosts_stack.h"

#define HOST_UPDATE_TIME 1000
#define HOST_RQST_TIME   5000

ret_t request_check_process();

typedef struct stacks_st
{
    user_stack *data_base;
    host_stack *active_hosts;
    req_stack  *requests_queue;
} stacks_st;

#endif