#include <stdio.h>
#include <unistd.h>
#include "debugging.h"
#include "msg_ipc.h"
//#include 

ret_t user_work_process()
{  
    ret_t ret_val = 0;

    LOG("> inititating user work ipc");
    msqd_t msgd = init_ipc(2, IPC_CREAT | IPC_EXCL | 0777);
    _RETURN_ON_TRUE(msgd == -1, -1);

    struct msgbuf requesting_socket = {};
    
    while(1)
    {
        ret_val = msgrcv(msgd, &requesting_socket, sizeof(sockd_t), 2, 0);
        
    }


    //Работа с нитями
}