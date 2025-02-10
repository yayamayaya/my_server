#ifndef SEM_SYNC_H
#define SEM_SYNC_H

#include "debugging.h"

#define SEM_PATH "server_socket_sem"

ret_t socket_sem_init();

no_ret_val_t socket_sem_close();

ret_t set_unix_socket_status_true();

ret_t wait_for_unix_socket_status();

#endif