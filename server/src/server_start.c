#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "debugging.h"
#include "server_start.h"
#include "connection_work_func.h"
#include "user_work_func.h"
#include "request_check_func.h"
#include "sem_sync.h"

ret_t run_server()
{
    LOG(">> initialising server:\n");

    LOG("> creating status semaphore...\n");
    _RETURN_ON_TRUE(socket_sem_init() == -1, -1);

    LOG(">> creating second process:\n");
    //Creating user work process
    pid_t work_proc_id = fork();
    _RETURN_ON_TRUE(work_proc_id == -1, -1, LOG_ERR(">> couldn't create new process:"));
    _RETURN_ON_TRUE(work_proc_id, connection_manage_process());
    
    pid_t req_proc_id = fork();
    _RETURN_ON_TRUE(req_proc_id == -1, -1, LOG_ERR(">> couldn't create new process:"));
    _RETURN_ON_TRUE(req_proc_id, user_work_process(req_proc_id));

    return request_check_process();
}