#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "debugging.h"
#include "connection_work_func.h"
#include "descr_sending_funcs.h"
#include "sig_handlers.h"
#include "sem_sync.h"

extern int server_shutdown;

struct sockaddr_in server_addr_create();

sockd_t create_listen_socket(const struct sockaddr_in *server_address);

ret_t connection_attempt_handler(const sockd_t msg_sock, const sockd_t listen_descr, struct sockaddr_in *server_address);


ret_t connection_manage_process()
{
    LOG("> setting sighandlers:\n");
    ret_t ret_val = set_sigint_handler();
    _RETURN_ON_TRUE(ret_val, ret_val);

    LOG("> starting server interface:\n");
    LOG("> setting server adress:\n");
    struct sockaddr_in server_adress = server_addr_create();
    LOG("> server address was set\n");

    LOG("> listen socket init:\n");
    sockd_t listen_socket_fd = create_listen_socket(&server_adress);
    _RETURN_ON_TRUE(listen_socket_fd == -1, -1);

    _RETURN_ON_TRUE(wait_for_unix_socket_status() == -1, -1,
        LOG_ERR("> sem wait error:"));

    sockd_t msg_sock = connect_to_unix_socket(CONN_WORK_UNIX_SOCKET_PATH);
    _RETURN_ON_TRUE(msg_sock == -1, -1, close(listen_socket_fd));
    
    LOG("> connection attempt handler start:\n");
    ret_t func_ret_val = 0;
    while (!check_kill_server_var())
        func_ret_val = connection_attempt_handler(msg_sock, listen_socket_fd, &server_adress);

    close(msg_sock);
    
    return func_ret_val;
}

struct sockaddr_in server_addr_create()
{
    struct sockaddr_in server_address = {};
    server_address.sin_family       = AF_INET;
    server_address.sin_addr.s_addr  = INADDR_ANY;
    server_address.sin_port         = htons(SERVER_PORT);

    return server_address;
}

sockd_t create_listen_socket(const struct sockaddr_in *server_address)
{
    LOG("> creating new socket:\n");
    sockd_t listen_sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    _RETURN_ON_TRUE(listen_sock_fd == -1, -1, LOG_ERR("socket creation error\n"));

    LOG("> binding socket to server address\n");
    _RETURN_ON_TRUE(bind(listen_sock_fd, (const struct sockaddr *)server_address, sizeof(struct sockaddr_in)) == -1, -1,
        LOG_ERR("> couldn't bind socket to that address:"));

    LOG("> transforming socket to a listen form\n");
    _RETURN_ON_TRUE(listen(listen_sock_fd, CONN_LISTEN_NUM), -1,
        LOG("> couldn't transform socket to a listen form:"));

    LOG("> listen socket creation successfull\n");
    return listen_sock_fd;
}

ret_t connection_attempt_handler(const sockd_t msg_sock, const sockd_t listen_descr, struct sockaddr_in *server_address)
{
    socklen_t address_length = sizeof(*server_address);

    struct pollfd sock_fd = {};
    sock_fd.fd      = listen_descr;
    sock_fd.events  = POLLIN;

    _RETURN_ON_TRUE(poll(&sock_fd, 1, CONN_REFRESH_TIME) == 0, 0);
    LOG("> LISTEN SOCKET REVENTS: %d\n", sock_fd.revents);

    sockd_t new_conn = accept(listen_descr, (struct sockaddr *)server_address, &address_length);
    _RETURN_ON_TRUE(new_conn == -1, -1, LOG_ERR("> couldn't create new connection:"));
    LOG("> connection occured, sending new socket \"%d\" descriptor to socket stack\n", new_conn);

    _RETURN_ON_TRUE(send_open_file_descriptor(msg_sock, new_conn) == -1, -1);
    LOG("> new socket data sent\n");

    return 0;
}