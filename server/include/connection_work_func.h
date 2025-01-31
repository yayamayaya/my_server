#ifndef CONNECTION_WORK_FUNC
#define CONNECTION_WORK_FUNC

#include "debugging.h"

#define SERVER_PORT         1667
#define CONN_LISTEN_NUM     15
#define CONN_REFRESH_TIME   5000

typedef int sockd_t;

ret_t connection_manage_process();

#endif