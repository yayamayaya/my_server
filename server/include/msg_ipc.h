#ifndef MSGQ_IPC_H
#define MSGQ_IPC_H

#include "connection_work_func.h"
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_NAME "/tmp/server_msq"

typedef int msqd_t;

struct msgbuf
{
    long    mtype;
    sockd_t descr;
};  

msqd_t init_ipc(int flag);

#endif