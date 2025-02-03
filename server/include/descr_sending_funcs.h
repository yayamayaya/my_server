#ifndef SENDING_ACTIVE_DESCR_FUNCS_H
#define SENDING_ACTIVE_DESCR_FUNCS_H

#include "debugging.h"
#include "connection_work_func.h"

#define CONN_WORK_UNIX_SOCKET_PATH "/tmp/server_conn_unix_socket"
#define USER_WORK_UNIX_SOCKET_PATH "/tmp/server_user_unix_socket"

sockd_t open_unix_listen_socket(const char *socket_path);

ret_t accept_unix_connection(sockd_t listen_sock_d, const char *socket_path);

sockd_t connect_to_unix_socket(const char *socket_path);

int rcv_open_file_descriptor(sockd_t sock_d);

ret_t send_open_file_descriptor(sockd_t sock_d, int opened_fd);

#endif