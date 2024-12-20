#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "debugging.h"
#include "data_base_stack.h"

static user_stack *user_data_base = NULL;




ret_t register_new_user(const char *username, const char *password, const char *rcv_path, const char *snd_path)
{
    assert(username);
    assert(password);
    assert(user_data_base);

    user_t new_user = {};
    memcpy(new_user.username, username, strlen(username));
    memcpy(new_user.password, password, strlen(password));

    _RETURN_ON_TRUE(mkfifo(rcv_path, 0777) == -1, -1, LOG_ERR("> fifo creation error:"););
    _RETURN_ON_TRUE(mkfifo(snd_path, 0777) == -1, -1, LOG_ERR("> fifo creation error:"); unlink(rcv_path););

    if (user_data_base->methods.add_user(user_data_base, new_user))
    {
        unlink(rcv_path);
        unlink(snd_path);

        return STACK_ADD_ERR;
    }

    return 0;
}

