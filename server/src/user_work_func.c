#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include "debugging.h"
#include "connection_work_func.h"
#include "msg_ipc.h"
#include "user_work_func.h"
#include "descr_sending_funcs.h"

ret_t user_work_process()
{  
    ssize_t bytes_rcvd = 0;

    LOG("/> inititating user work ipc\n");
    /*sockd_t msg_sock = init_ipc(2, IPC_CREAT | IPC_EXCL | 0777);
    _RETURN_ON_TRUE(msg_sock == -1, -1);
    _RETURN_ON_TRUE(test_msg(msg_sock, RCV, 2) == -1, -1,
    msgctl(msg_sock, IPC_RMID, NULL));*/
    
    //Временно
    sleep(1);

    LOG("/> connecting to unix socket:\n");
    sockd_t msg_sock = connect_to_unix_socket(USER_WORK_UNIX_SOCKET_PATH);
    _RETURN_ON_TRUE(msg_sock == -1, -1);
    LOG("/> done\n");

    struct pollfd msg_sock_poll = {};
    msg_sock_poll.fd        = msg_sock;
    msg_sock_poll.events    = POLLIN;

    LOG("/> waiting for first requests\n");
    while(1)
    {
        if (!poll(&msg_sock_poll, 1, UNIX_SOCKET_POLL_TIMEOUT)) continue;
        LOG("/> UNIX SOCKET REVENT: %d\n", msg_sock_poll.revents);
        msg_sock_poll.revents = 0;

        sockd_t req_socket = rcv_open_file_descriptor(msg_sock);
        if (req_socket == -1) LOG("/> error occured while rcving request socket\n");

        LOG("/> requesting socket descr is: %d\n", req_socket);

        LOG("/> recieving test data from descriptor: \n");
        char data[32] = {0};
        bytes_rcvd = recv(req_socket, data, 32 * sizeof(char), 0);
        if (bytes_rcvd == -1) LOG_ERR("/> recv failed:");
        else LOG("%s\n", data);
    }


    //Работа с нитями
}