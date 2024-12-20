#ifndef DATA_BASE_WORK
#define DATA_BASE_WORK

#include <structs.h>

#define USERNAME_LINE "username: %32s\n"
#define PASSWORD_LINE "password: %16s\n"
#define RCV_FILE_LINE "rcv_path: %s\n"
#define SND_FILE_LINE "snd_path: %s\n"

#define LONG_LINE     "------------------------------------------\n%n"



ret_t register_new_user(const char *username, const char *password, const char *rcv_path, const char *snd_path);

ret_t check_pswd(const char *username, const char *password);

ret_t read_data_base();

void user_base_dump();



#endif