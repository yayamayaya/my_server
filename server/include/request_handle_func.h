#ifndef USER_WORK_FUNCS
#define USER_WORK_FUNCS

#include "debugging.h"
#include "data_base_stack.h"
#include "connection_work_func.h"
#include "active_hosts_stack.h"

#define HOST_UPDATE_TIME 5000
#define HOST_RQST_TIME   5000

ret_t request_check_process();

typedef struct all_required_data
{
    user_stack *data_base;
    host_stack *active_hosts;
    sockd_t     conn_msg_s;
    sockd_t     work_msg_s;
} process_data_st;

#endif