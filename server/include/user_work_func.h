#ifndef USER_WORK_PROCESS_FUNCS_H
#define USER_WORK_PROCESS_FUNCS_H

#include "debugging.h"

#define UNIX_SOCKET_POLL_TIMEOUT 5000

ret_t user_work_process(const pid_t req_pid);

#endif