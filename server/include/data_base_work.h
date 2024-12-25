#ifndef DATA_BASE_WORK
#define DATA_BASE_WORK

#include "structs.h"

#define USERNAME_LINE "username: %32s\n"
#define PASSWORD_LINE "password: %16s\n"
#define RCV_FILE_LINE "rcv_path: %s\n"
#define SND_FILE_LINE "snd_path: %s\n"

#define FILEPATH_DATA  "/proc/self/%d"

#define LONG_LINE_N   "------------------------------------------\n%n"
#define LONG_LINE     "------------------------------------------\n"

#define TMP_BASE_PATH "/tmp/server_client_base.db"

ret_t register_new_user(const char *username, const char *password, const char *rcv_path, const char *snd_path);

pswd_t check_pswd(const char *username, const char *password);

ret_t read_data_base();

ret_t save_data_base();

void user_base_dump();

ret_t finish_db_work();


#endif