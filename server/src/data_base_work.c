#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/types.h>
#include "debugging.h"
#include "data_base_work.h"
#include "data_base_stack.h"

static user_stack *data_base_stack = NULL;

ret_t parse_user(user_t *user_ptr, FILE *db_ptr);

//Регистриурем нового пользователя
ret_t register_new_user(const char *username, const char *password, const char *rcv_path, const char *snd_path)
{
    assert(username);
    assert(password);
    assert(data_base_stack);

    LOG("> registering new user:\n");

    user_t new_user = {};
    memcpy(new_user.username, username, strlen(username));
    memcpy(new_user.password, password, strlen(password));

    LOG("> creating fifo files\n");

    _RETURN_ON_TRUE(mkfifo(rcv_path, 0777 | O_CREAT) == -1, -1, LOG_ERR("> fifo creation error:"););
    _RETURN_ON_TRUE(mkfifo(snd_path, 0777 | O_CREAT) == -1, -1, LOG_ERR("> fifo creation error:"); unlink(rcv_path););

    LOG("> adding new user to stack\n");

    if (data_base_stack->methods.add_user(data_base_stack, new_user))
    {
        unlink(rcv_path);
        unlink(snd_path);

        return STACK_ADD_ERR;
    }

    LOG("> complete\n");

    return 0;
}

ret_t check_pswd(const char *username, const char *password)
{
    assert(username);
    assert(password);

    long unsigned int usr_num = data_base_stack->methods.find_user(data_base_stack, username);
    _RETURN_ON_TRUE(usr_num == USR_NOT_FOUND, USR_NOT_FOUND, LOG("> searched user wasn't found\n"););

    user_t user_data = data_base_stack->methods.return_usr_data(data_base_stack, usr_num);
    return !strcmp(user_data.password, password) ? RIGHT_PSWD : WRONG_PSWD;
}

void user_base_dump()
{
    assert(data_base_stack);

    data_base_stack->methods.dump(data_base_stack);

    return;
}


//------------------------Читаем базу данных----------------------------------------------------
ret_t read_data_base()
{
    char path[PATH_MAX] = {};
    snprintf(path, PATH_MAX * sizeof(char), "%s%s", BIN_PATH, DATA_BASE_LOCATION);

    LOG("> data base location is: %s\n", path);

    LOG("> initialising data base:\n");
    LOG("> opening data base file\n");
    
    FILE *data_base_file = fopen(path, "rb");
    _RETURN_ON_TRUE(!data_base_file, FILE_OPEN_ERR, LOG("> data base file open error\n"););

    LOG("> user stack init\n");

    ret_t ret_val = init_user_stack(&data_base_stack);
    _RETURN_ON_TRUE(ret_val, ret_val);

    LOG("> parsing users\n");

    ret_val = 0;
    while (ret_val != BASE_END)
    {
        user_t user = {0};

        ret_val = parse_user(&user, data_base_file);
        LOG("> ret_val = %d\n", ret_val);
        if (ret_val && ret_val != BASE_END)
        {
            //LOG("> parsing error occured\n");
            fclose(data_base_file);
            data_base_stack->methods.stack_destructor(data_base_stack);
            data_base_stack = NULL;

            return ret_val;
        }

        data_base_stack->methods.add_user(data_base_stack, user);
    }
    
    LOG("> parsing complete, data base is initiated\n");

    fclose(data_base_file);

    return 0;
}

ret_t parse_user(user_t *user_ptr, FILE *db_ptr)
{
    assert(user_ptr);
    assert(db_ptr);

    user_t  user_holder     = {};
    int     char_scanned    = 0;
    char    path[PATH_MAX]  = {0};

    if (fscanf(db_ptr, USERNAME_LINE, user_holder.username) != 1) return USER_PARSE_ERR;
    LOG("> username parsed\n");
    if (fscanf(db_ptr, PASSWORD_LINE, user_holder.password) != 1) return USER_PARSE_ERR;
    LOG("> password parsed\n");

    if (fscanf(db_ptr, RCV_FILE_LINE, path) != 1)                 return USER_PARSE_ERR;
    LOG("> rcv file parsed\n");

    //Убрать O_CREAT
    int rcv_fd = open(path, 0777 | O_CREAT);
    _RETURN_ON_TRUE(rcv_fd == -1, -1, LOG_ERR("> fifo file open error: "););

    if (fscanf(db_ptr, SND_FILE_LINE, path) != 1)                 return USER_PARSE_ERR;
    LOG("> snd file parsed\n");

    //Убрать O_CREAT
    int snd_fd = open(path, 0777 | O_CREAT);
    _RETURN_ON_TRUE(snd_fd == -1, -1, LOG_ERR("> fifo file open error: "); close(rcv_fd););

    user_holder.rcv_d = rcv_fd;
    user_holder.snd_d = snd_fd;

    *user_ptr = user_holder;

    fscanf(db_ptr, LONG_LINE, &char_scanned);
    if (!char_scanned)
        return BASE_END;
    else
        return 0;
    
    return 0;
}