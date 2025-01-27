#ifndef DATA_BASE_WORK
#define DATA_BASE_WORK

#include "structs.h"
#include "data_base_stack.h"

#define USERNAME_LINE "username: %32s\n"
#define PASSWORD_LINE "password: %16s\n"

#define LONG_LINE_N   "------------------------------------------\n%n"
#define LONG_LINE     "------------------------------------------\n"

#define TMP_BASE_PATH "/tmp/server_client_base.db"

ret_t register_new_user(user_stack *data_base_stack, const char *username, const char *password, const char *rcv_path, const char *snd_path);

pswd_t check_pswd(user_stack *data_base_stack, const char *username, const char *password);

ret_t read_data_base(user_stack *data_base_stack);

ret_t save_data_base(user_stack *data_base_stack);

void user_base_dump(user_stack *data_base_stack);

ret_t finish_db_work(user_stack *data_base_stack);


#endif