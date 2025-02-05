#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "debugging.h"
#include "msg_ipc.h"

/*msqd_t init_ipc(const int ipc_number, const int flag)
{
    key_t key = ftok(KEY_NAME, ipc_number);
    _RETURN_ON_TRUE(key == -1, -1, LOG_ERR(">> couldn't create ipc key:"));

    msqd_t msqd = msgget(key, flag);
    _RETURN_ON_TRUE(msqd == -1, -1, LOG_ERR(">> couldn't create msqd:"));

    LOG(">> created %d msg line\n", ipc_number);

    return msqd;
}

ret_t test_msg(msqd_t msgd, const int option, const int msg_type)
{
    struct msgbuf test = {};

    if (option == RCV)
    {
        LOG(">> recieving test msg:\n");
        _RETURN_ON_TRUE(msgrcv(msgd, &test, sizeof(sockd_t), msg_type, 0) == -1, -1, LOG_ERR(">> couldn't rcv msg:"));
        LOG(">> test number recieved is: \"%d\" for msg line: %d\n", test.descr, test.mtype);
    }
    else if (option == SND)
    {
        LOG(">> sending test msg to the %d msg line\n", msg_type);
        test.descr = rand();
        test.mtype = msg_type;

        _RETURN_ON_TRUE(msgsnd(msgd, &test, sizeof(sockd_t), 0) == -1, -1, LOG_ERR(">> couln't send msg:"));
    }
    else LOG(">> test_msg wrong option\n");

    return 0;
}*/
    

    