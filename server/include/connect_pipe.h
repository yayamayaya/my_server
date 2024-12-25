#ifndef CONNECT_PIPE_STR
#define CONNECT_PIPE_STR

#include "structs.h"

#define PIPE_PATH "/tmp/request_fifo"
#define INTERM_BUFF_SIZE 32

typedef struct connect_pipe link_pipe_st;
typedef struct opera        ops;

//Сервер умеет только читать из этого файла

typedef struct opera
{
    ret_t (*check_request)(link_pipe_st *self);

    void (* destructor)(link_pipe_st *self);
} ops;

typedef struct connect_pipe
{
    int pipe_fd;
    char buff[INTERM_BUFF_SIZE];

    ops methods;
} link_pipe_st;


link_pipe_st *construct_pipe();

#endif