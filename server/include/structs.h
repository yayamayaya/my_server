#ifndef SERVER_STRUCTS
#define SERVER_STRUCTS

#define USERNAME_MAX_LENGTH 32
#define USER_PSWRD_MAX_LEN  16

typedef char   pswd_t;
typedef void   no_ret_val_t; 
typedef int    ret_t;

typedef struct user_data
{
    unsigned long int user_number;
    char username[USERNAME_MAX_LENGTH];
    char password[USER_PSWRD_MAX_LEN];
    int rcv_d;
    int snd_d;
} user_t;


#endif