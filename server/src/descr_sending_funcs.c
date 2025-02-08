#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>
#include "descr_sending_funcs.h"

sockd_t open_unix_listen_socket(const char *socket_path)
{
    LOG(">> creating UNIX listen socket with for path: %s\n", socket_path);
    sockd_t sock_d = socket(AF_UNIX, SOCK_STREAM, 0);
    _RETURN_ON_TRUE(sock_d == -1, -1, LOG_ERR(">> UNIX socket creation error:"));
    
    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    memcpy(addr.sun_path, socket_path, strlen(socket_path));

    sockd_t ret_val = bind(sock_d, (struct sockaddr *)&addr, sizeof(addr));
    _RETURN_ON_TRUE(ret_val == -1, -1,
        LOG_ERR(">> UNIX socket bind error:");
        close(sock_d));
    
    ret_val = listen(sock_d, NUMBER_OF_CONN_REQS);
    _RETURN_ON_TRUE(ret_val == -1, -1,
        LOG_ERR(">> UNIX socket listen() error:");
        close(sock_d));

    LOG(">> UNIX socket created\n");
    return sock_d;
}

sockd_t accept_unix_connection(sockd_t listen_sock_d, const char *socket_path)
{
    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    memcpy(addr.sun_path, socket_path, strlen(socket_path));

    LOG(">> establishing connection with other process\n");
    socklen_t socklen = sizeof(addr);
    int conn_sock = accept(listen_sock_d, (struct sockaddr *)&addr, &socklen);
    _RETURN_ON_TRUE(conn_sock == -1, -1, 
        LOG_ERR(">> couldn't connect to requesting socket:");
        close(listen_sock_d));

    LOG(">> connection socket is: %d\n", conn_sock);

    LOG(">> done\n");
    
    close(listen_sock_d);

    return conn_sock;
}

sockd_t connect_to_unix_socket(const char *socket_path)
{
    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    memcpy(addr.sun_path, socket_path, strlen(socket_path));

    LOG(">> attempting to connect to unix socket\n");

    sockd_t sock = socket(AF_UNIX, SOCK_STREAM, 0);
    _RETURN_ON_TRUE(sock == -1, -1, LOG_ERR(">> couldn't create unux socket:"));
    
    _RETURN_ON_TRUE(connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1, -1,
        LOG_ERR(">> couldn't connect to listen socket:"));

    LOG(">> connection success\n");

    return sock;
}

int rcv_open_file_descriptor(sockd_t sock_d)
{
    struct msghdr msgh      = {0};
    struct iovec iov        = {0};
    struct cmsghdr *cmsgp   = NULL;
    int data                = 0;
    int fd                  = 0;

    iov.iov_base = &data;
    iov.iov_len  = sizeof(int);

    msgh.msg_name       = NULL;
    msgh.msg_namelen    = 0;
    msgh.msg_iov        = &iov;
    msgh.msg_iovlen     = 1;

    union
    {
        char buff[CMSG_SPACE(sizeof(int))];
        struct cmsghdr align;
    } control_msg;

    msgh.msg_control    = control_msg.buff;
    msgh.msg_controllen = sizeof(control_msg.buff);

    LOG(">> recieving a file descriptor from unix socket...\n");
    ssize_t ret_val = recvmsg(sock_d, &msgh, 0);
    _RETURN_ON_TRUE(ret_val == -1, -1, LOG_ERR(">> recvmsg error:"));

    cmsgp = CMSG_FIRSTHDR(&msgh);
    if (cmsgp == NULL
        || cmsgp->cmsg_level != SOL_SOCKET
        || cmsgp->cmsg_type  != SCM_RIGHTS
        || cmsgp->cmsg_len   != CMSG_LEN(sizeof(int)))
    {
        LOG(">> message is corrupted, descriptor recieve error\n");
        return -1;
    }

    memcpy(&fd, CMSG_DATA(cmsgp), sizeof(int));

    return fd;
}

ret_t send_open_file_descriptor(sockd_t sock_d, int opened_fd)
{
    struct msghdr msgh      = {0};
    struct iovec iov        = {0};
    struct cmsghdr *cmsgp   = NULL;
    int data                = 12;

    iov.iov_base = &data;
    iov.iov_len  = sizeof(int);

    msgh.msg_name       = NULL;
    msgh.msg_namelen    = 0;
    msgh.msg_iov        = &iov;
    msgh.msg_iovlen     = 1;

    union
    {
        char buf[CMSG_SPACE(sizeof(int))];
        struct cmsghdr align;
    } control_msg;

    msgh.msg_control    = control_msg.buf;
    msgh.msg_controllen = sizeof(control_msg.buf);

    cmsgp               = CMSG_FIRSTHDR(&msgh);
    cmsgp->cmsg_level   = SOL_SOCKET;
    cmsgp->cmsg_type    = SCM_RIGHTS;
    cmsgp->cmsg_len     = CMSG_LEN(sizeof(int));
    memcpy(CMSG_DATA(cmsgp), &opened_fd, sizeof(int));

    LOG(">> sending active file descriptor through the socket\n");
    ssize_t ret_val = sendmsg(sock_d, &msgh, 0);
    _RETURN_ON_TRUE(ret_val == -1, -1, LOG_ERR(">> message send error:"));

    LOG(">> descriptor sent\n");
    return 0;
}