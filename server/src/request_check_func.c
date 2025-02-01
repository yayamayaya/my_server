#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <poll.h>
#include "data_base_stack.h"
#include "data_base_work.h"
#include "request_check_func.h"
#include "msg_ipc.h"
#include "debugging.h"

ret_t create_all_stacks(stacks_st *stacks);

no_ret_val_t stacks_destr(stacks_st *stacks);

user_stack *data_base_init();

ret_t update_active_hosts(host_stack *hosts, const msqd_t msgd);

ret_t poll_cycle(stacks_st *stacks, const msqd_t msgd);

ret_t send_new_request(stacks_st *stacks, ret_t poll_ret_val);


ret_t request_check_process()
{
    LOG("}> creating ipc:\n");
    msqd_t msgd = init_ipc(1, 0);
    _RETURN_ON_TRUE(msgd == -1, -1);

    LOG("}> recieving test msg:\n");
    struct msgbuf rcv_test = {};
    msgrcv(msgd, &rcv_test, sizeof(sockd_t), 1, 0);
    LOG("}> test number recieved is: \"%d\"\n", rcv_test.descr);

    LOG("}> initialising stacks:\n");
    stacks_st stacks = {};
    ret_t ret_val = create_all_stacks(&stacks);
    _RETURN_ON_TRUE(ret_val, ret_val);

    LOG("}> waiting for first hosts:\n");
    ret_t update_ret_val = 0;
    while (1)
    {
        update_ret_val = update_active_hosts(stacks.active_hosts, msgd);
        if (update_ret_val == 1) break;
        _RETURN_ON_TRUE(update_ret_val, update_ret_val);
    }
    LOG("}> first host connected, starting full work:\n");



    stacks.active_hosts->methods.dump(stacks.active_hosts);

    stacks_destr(&stacks);
    
    return 0;
}

ret_t update_active_hosts(host_stack *hosts, const msqd_t msgd)
{
    assert(hosts);

    static struct pollfd msg_descr = {0, POLLIN, 0};
    msg_descr.fd = msgd;

    if (!poll(&msg_descr, 1, HOST_UPDATE_TIME)) return 0;

    struct msgbuf data = {};
    _RETURN_ON_TRUE(msgrcv(msgd, &data, sizeof(sockd_t), 1, 0) == -1, -1, LOG("}> msg rcv error:"));

    struct pollfd host_descr = {};
    host_descr.fd       = data.descr;
    host_descr.events   = POLLIN;
    _RETURN_ON_TRUE(hosts->methods.add_host(hosts, host_descr), HOST_ARR_REALLC_ERR);

    return 1;
}

ret_t create_all_stacks(stacks_st *stacks)
{
    assert(stacks);

    LOG("}> request queue creation:\n");
    req_stack *req_queue = NULL;
    ret_t ret_val = init_req_stack(&req_queue);
    _RETURN_ON_TRUE(ret_val, ret_val);
    LOG("}> done\n");

    LOG("}> hosts stack initialisation:\n");
    host_stack *hosts = NULL;
    ret_val = init_host_stack(&hosts);
    _RETURN_ON_TRUE(ret_val, ret_val, req_queue->methods.stack_destructor(req_queue));
    LOG("}> done\n");

    LOG("}> user stack initiation:\n");
    user_stack *user_base = data_base_init();
    _RETURN_ON_TRUE(!user_base, ret_val,
        req_queue->methods.stack_destructor(req_queue);
        hosts->methods.stack_destructor(hosts));
    LOG("}> done\n");

    stacks->data_base       = user_base;
    stacks->active_hosts    = hosts;
    stacks->requests_queue  = req_queue;
    
    return 0;
}

no_ret_val_t stacks_destr(stacks_st *stacks)
{   
    assert(stacks);
        
    stacks->data_base->methods.stack_destructor(stacks->data_base);
    stacks->active_hosts->methods.stack_destructor(stacks->active_hosts);
    stacks->requests_queue->methods.stack_destructor(stacks->requests_queue);

    return;
}

user_stack *data_base_init()
{
    LOG("}> creating data base stack:\n");
    user_stack *user_base = NULL;
    ret_t ret_val = init_user_stack(&user_base);
    _RETURN_ON_TRUE(ret_val, NULL);

    LOG("}> parsing data base:\n");
    ret_val = read_data_base(user_base);
    _RETURN_ON_TRUE(ret_val, NULL);

    LOG("}> dumping data base:\n");
    user_base_dump(user_base);

    return user_base;
}

ret_t poll_cycle(stacks_st *stacks, const msqd_t msgd)
{
    assert(stacks);

    ret_t poll_ret_val = 0;

    while (1)
    {
        if (update_active_hosts(stacks->active_hosts, msgd) == 1) LOG("}> new host added\n");

        poll_ret_val = poll(stacks->active_hosts->hosts, stacks->active_hosts->hosts_number, HOST_RQST_TIME);
        
        
    }
}

ret_t send_new_request(stacks_st *stacks, ret_t poll_ret_val)
{
    if (!poll_ret_val) return 0;

    for (long unsigned int i = 0; i < stacks->active_hosts->hosts_number; i++)
        if (stacks->active_hosts->hosts[i].revents & POLLIN)
        {
            //msgsnd()
        }
    
}