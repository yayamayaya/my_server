#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <poll.h>
#include "data_base_stack.h"
#include "data_base_work.h"
#include "request_check_func.h"
#include "msg_ipc.h"
#include "debugging.h"
#include "descr_sending_funcs.h"

ret_t create_all_process_data(process_data_st *p_data);

no_ret_val_t stacks_destr(process_data_st *p_data);

user_stack *data_base_init();

ret_t update_active_hosts(process_data_st *p_data);

no_ret_val_t poll_cycle(process_data_st *p_data);

no_ret_val_t send_new_request(process_data_st *p_data, ret_t poll_ret_val);


//TO DO: перенос стека пользователей в процесс рабоыт с пользователями

ret_t request_check_process()
{
    LOG("}> initialising all data struct:\n");
    process_data_st p_data = {};
    ret_t ret_val = create_all_process_data(&p_data);
    _RETURN_ON_TRUE(ret_val, ret_val);

    LOG("}> waiting for first hosts:\n");

    poll_cycle(&p_data);

    p_data.active_hosts->methods.dump(p_data.active_hosts);

    stacks_destr(&p_data);
    
    return 0;
}

ret_t update_active_hosts(process_data_st *p_data)
{
    assert(p_data);

    static struct pollfd msg_descr = {0};
    msg_descr.fd        = p_data->conn_msg_s;
    msg_descr.events    = POLLIN;

    LOG("}> updating active hosts:\n");

    ret_t ret_val = poll(&msg_descr, 1, 0);
    //LOG("}> poll ret_val is: %d, revent is: %d\n", ret_val, msg_descr.revents);
    _RETURN_ON_TRUE(ret_val == -1, 0, LOG_ERR("}> poll error:"));
    if (!ret_val) return 0;
    LOG("}> HOST REVENT: %d\n", msg_descr.revents);

    int rcvd_descr = rcv_open_file_descriptor(p_data->conn_msg_s);
    LOG("> rcvd host descr == %d\n", rcvd_descr);
    _RETURN_ON_TRUE(rcvd_descr == -1, -1);

    struct pollfd host_descr = {};
    host_descr.fd       = rcvd_descr;
    host_descr.events   = POLLIN;
    _RETURN_ON_TRUE(p_data->active_hosts->methods.add_host(p_data->active_hosts, host_descr), HOST_ARR_REALLC_ERR);

    return 1;
}

ret_t create_all_process_data(process_data_st *p_data)
{
    assert(p_data);

    LOG("}> creating connection msg line ipc:\n");

    sockd_t conn_manager_listen_socket = open_unix_listen_socket(CONN_WORK_UNIX_SOCKET_PATH);
    _RETURN_ON_TRUE(conn_manager_listen_socket == -1, -1);
    sockd_t user_work_listen_socket    = open_unix_listen_socket(USER_WORK_UNIX_SOCKET_PATH);
    _RETURN_ON_TRUE(user_work_listen_socket == -1, -1, close(conn_manager_listen_socket));

    sockd_t conn_manager_socket \
        = accept_unix_connection(conn_manager_listen_socket, CONN_WORK_UNIX_SOCKET_PATH);
    _RETURN_ON_TRUE(conn_manager_socket == -1, -1,
        close(user_work_listen_socket));
    sockd_t user_work_socket    \
        = accept_unix_connection(user_work_listen_socket, USER_WORK_UNIX_SOCKET_PATH);
    _RETURN_ON_TRUE(user_work_socket == -1, -1,
        close(conn_manager_socket));

    p_data->conn_msg_s = conn_manager_socket;
    LOG("}> connection manager socket: %d\n", conn_manager_socket);
    p_data->work_msg_s = user_work_socket;
    LOG("}> user work socket: %d\n", user_work_socket);

    LOG("}> hosts stack initialisation:\n");
    host_stack *hosts = NULL;
    ret_t ret_val = init_host_stack(&hosts);
    _RETURN_ON_TRUE(ret_val, ret_val,
        close(p_data->conn_msg_s);
        close(p_data->work_msg_s));
    LOG("}> done\n");

    LOG("}> user stack initiation:\n");
    user_stack *user_base = data_base_init();
    _RETURN_ON_TRUE(!user_base, ret_val,
        hosts->methods.stack_destructor(hosts);
        close(p_data->conn_msg_s);
        close(p_data->work_msg_s));
    LOG("}> done\n");

    p_data->data_base       = user_base;
    p_data->active_hosts    = hosts;
    
    return 0;
}

no_ret_val_t stacks_destr(process_data_st *p_data)
{   
    assert(p_data);

    close(p_data->conn_msg_s);
    close(p_data->work_msg_s);

    unlink(CONN_WORK_UNIX_SOCKET_PATH);
    unlink(USER_WORK_UNIX_SOCKET_PATH);

    finish_db_work(p_data->data_base);
    p_data->active_hosts->methods.stack_destructor(p_data->active_hosts);

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

no_ret_val_t poll_cycle(process_data_st *p_data)
{
    assert(p_data);

    ret_t poll_ret_val = 0;

    while (1)
    {
        if (update_active_hosts(p_data) == 1) LOG("}> new host added\n");

        poll_ret_val = 0;
        poll_ret_val = poll(p_data->active_hosts->hosts, p_data->active_hosts->hosts_number, HOST_RQST_TIME);
        if (poll_ret_val == -1)
        {
            LOG_ERR("}> poll error:");
            continue;
        }
        
        LOG("}> poll_ret_val = %d\n", poll_ret_val);

        send_new_request(p_data, poll_ret_val);

        if (0) break;
    }

    return;
}

no_ret_val_t send_new_request(process_data_st *p_data, ret_t poll_ret_val)
{
    if (!poll_ret_val) return;

    p_data->active_hosts->methods.dump(p_data->active_hosts);

    for (long unsigned int host_num = 0; host_num < p_data->active_hosts->hosts_number; host_num++)
    {
        LOG("}> REVENT VALUE: %d, REVENT MASK: %d\n", p_data->active_hosts->hosts[host_num].revents, p_data->active_hosts->hosts[host_num].revents & POLLIN);
        if (p_data->active_hosts->hosts[host_num].revents & POLLIN)
        {
            LOG("}> found requesting socket in host num: %d\n", host_num);

            if (send_open_file_descriptor(p_data->work_msg_s, p_data->active_hosts->hosts[host_num].fd) == -1) \
                LOG_ERR("}> msg snd error:");

            //p_data->active_hosts->hosts[host_num].revents = 0;
        }
    }
    
    return;
}