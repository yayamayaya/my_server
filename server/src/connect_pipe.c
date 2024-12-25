#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include "structs.h"
#include "connect_pipe.h"
#include "debugging.h"

static void pipe_destructor(link_pipe_st *self);
static ret_t pipe_check_request(link_pipe_st *self);

//Что читает сервер: длина username -> username -> длина pswd -> pswd
//Далее отправляем данные на стек, стек смотрит есть ли данные или нет, если да линкуемся, если нет предлагаем создать нового пользователя через сигнал

link_pipe_st *construct_pipe()
{
    LOG("> initialising request pipe:\n");
    int fifo_d = 0;
    link_pipe_st *pipe = NULL;

    LOG("> creating fifo\n");

    int ret_val = mkfifo(PIPE_PATH, O_CREAT | O_EXCL | 0777);
    _RETURN_ON_TRUE(ret_val == -1, NULL, LOG_ERR("> couldn't make fifo: "););

    LOG("> opening fifo for reading\n");

    fifo_d = open(PIPE_PATH, O_RDONLY);
    _RETURN_ON_TRUE(fifo_d == -1, NULL, 
        LOG_ERR("> couldn't open created fifo: ");
        unlink(PIPE_PATH););

    LOG("> allocating memory for struture\n");

    pipe = (link_pipe_st *)calloc(1, sizeof(link_pipe_st));
    _RETURN_ON_TRUE(!pipe, NULL, 
        LOG("> pipe structure memory allocation error\n");
        close(fifo_d);
        unlink(PIPE_PATH););

    pipe->pipe_fd = fifo_d;

    pipe->methods.destructor    = pipe_destructor;
    pipe->methods.check_request = pipe_check_request;

    LOG("> done\n");

    return pipe;
}

static void pipe_destructor(link_pipe_st *self)
{
    assert(self);

    close(self->pipe_fd);
    unlink(PIPE_PATH);

    free(self);

    return;
}

static ret_t pipe_check_request(link_pipe_st *self)
{
    assert(self);

    LOG("> checking for connection attempt\n");

    static struct pollfd fds = {0};
    fds.fd      = self->pipe_fd;
    fds.events  = POLLIN;

    int ret_val = poll(&fds, 1, 0);
    _RETURN_ON_TRUE(ret_val == -1, -1, LOG_ERR("> poll error: "););
    _RETURN_ON_TRUE(!ret_val, 0, LOG("> no connection attempts\n"););

    //read_pipe

    return 0;
}