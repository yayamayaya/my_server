#include <stdio.h>
#include <signal.h>
#include "sig_handlers.h"

static volatile sig_atomic_t kill_server            = 0;
static volatile sig_atomic_t unix_sockets_status    = 0;

void sigrt_handler(int signum)
{
    (void)signum;

    unix_sockets_status = 1;
}

void sigint_handler(int signum)
{
    (void)signum;

    kill_server = 1;
}

ret_t set_sigint_handler()
{
    struct sigaction action = {};
    action.sa_flags     = 0;
    action.sa_handler = sigint_handler;

    _RETURN_ON_TRUE(sigaction(SIGINT, &action, NULL) == -1, -1, LOG_ERR("SIGINT handler set error:"));

    return 0;
}

ret_t set_sigrt_handler()
{
    struct sigaction action = {};
    action.sa_flags     = 0;
    action.sa_handler = sigrt_handler;

    _RETURN_ON_TRUE(sigaction(SIGRTMIN, &action, NULL) == -1, -1, LOG_ERR("SIGRT handler set error:"));

    return 0;
}

ret_t check_unix_sockets_status()
{
    return unix_sockets_status;
}

ret_t check_kill_server_var()
{
    return kill_server;
}