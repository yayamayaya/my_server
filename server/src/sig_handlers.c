#include <stdio.h>
#include <signal.h>
#include "sig_handlers.h"
#include "sem_sync.h"

static volatile sig_atomic_t kill_server            = 0;
static volatile sig_atomic_t unix_sockets_status    = 0;

void sigint_handler(int signum)
{
    (void)signum;

    socket_sem_close();

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

ret_t check_kill_server_var()
{
    return kill_server;
}