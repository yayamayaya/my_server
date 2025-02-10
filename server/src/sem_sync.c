#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include "debugging.h"
#include "sem_sync.h"

static sem_t *unix_socket_launch_status;

ret_t socket_sem_init()
{
    unix_socket_launch_status = sem_open(SEM_PATH, O_CREAT | O_EXCL, 0666, 0);
    _RETURN_ON_TRUE(unix_socket_launch_status == SEM_FAILED, -1, LOG_ERR("|> new sem create error:"));

    return 0;
}

no_ret_val_t socket_sem_close()
{
    sem_close(unix_socket_launch_status);
    sem_unlink(SEM_PATH);

    return;
}

ret_t set_unix_socket_status_true()
{
    ret_t ret_val = sem_post(unix_socket_launch_status);
    _RETURN_ON_TRUE(ret_val == -1, -1);
    
    return sem_post(unix_socket_launch_status);
}

ret_t wait_for_unix_socket_status()
{
    return sem_wait(unix_socket_launch_status);
}
