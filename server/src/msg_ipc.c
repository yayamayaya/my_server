#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "debugging.h"
#include "msg_ipc.h"

msqd_t init_ipc(int flag)
{
    key_t key = ftok(KEY_NAME, 1);
    _RETURN_ON_TRUE(key == -1, -1, LOG_ERR("> couldn't create ipc key:"));

    msqd_t msqd = msgget(key, flag);
    _RETURN_ON_TRUE(msqd == -1, -1, LOG_ERR("> couldn't create msqd:"));

    return msqd;
}