#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "debugging.h"
#include "connection_work_func.h"

int create_listen_socket();

ret_t connection_manage_process()
{
    LOG("> starting server interface:\n");
    LOG("> listen socket init:\n");
    int listen_socket_fd = create_listen_socket();
    _RETURN_ON_TRUE(listen_socket_fd == -1, -1);
    
    
    return 0;
}

int create_listen_socket()
{
    LOG("> creating new socket:\n");
    int listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    _RETURN_ON_TRUE(listen_sock_fd == -1, -1, LOG_ERR("socket creation error\n"));

    struct sockaddr_in server_address = {};
    server_address.sin_family       = AF_INET;
    server_address.sin_addr.s_addr  = INADDR_ANY;
    server_address.sin_port         = SERVER_PORT;

    LOG("> binding socket to server address\n");
    _RETURN_ON_TRUE(bind(listen_sock_fd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in)) == -1, -1,
        LOG_ERR("> couldn't bind socket to that address:"));

    LOG("> transforming socket to a listen form\n");
    _RETURN_ON_TRUE(listen(listen_sock_fd, CONN_LISTEN_NUM), -1,
        LOG("> couldn't transform socket to a listen form:"));

    LOG("> listen socket creation successfull\n");
    return listen_sock_fd;
}