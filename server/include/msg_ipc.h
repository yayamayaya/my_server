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

/*enum OPTIONS
{
    SND = 1,
    RCV = 0
};

msqd_t init_ipc(const int ipc_number, const int flag);

ret_t test_msg(msqd_t msgd, const int option, const int msg_type);*/

#endif