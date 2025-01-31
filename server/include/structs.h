#ifndef SERVER_STRUCTS
#define SERVER_STRUCTS

#include "debugging.h"

#define USERNAME_MAX_LENGTH 32
#define USER_PSWRD_MAX_LEN  16

typedef char   pswd_t;

typedef struct user_data
{
    unsigned long int user_number;
    char username[USERNAME_MAX_LENGTH];
    char password[USER_PSWRD_MAX_LEN];
} user_t;

#endif