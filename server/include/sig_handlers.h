#ifndef SIGINT_HANDLER_H
#define SIGINT_HANDLER_H

#include "debugging.h"

void sigint_handler(int signum);

ret_t set_sigint_handler();

ret_t check_kill_server_var();

void sigrt_handler(int signum);

ret_t set_sigrt_handler();

ret_t check_unix_sockets_status();

#endif