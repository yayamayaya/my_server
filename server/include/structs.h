#ifndef SERVER_STRUCTS
#define SERVER_STRUCTS

typedef struct user_data user_t;
typedef struct user_base user_stack;
typedef struct Ops st_op_table;

typedef int    ret_t;
typedef void   no_ret_val_t; 

#define DATA_BASE_LOCATION  "/user_data.txt"

typedef struct user_data
{
    int user_number;
    char username[32];
    char password[16];
    int rcv_d;
    int snd_d;
} user_t;

typedef struct Ops
{
    ret_t (*add_user)(user_stack *self, user_t user_data);
    no_ret_val_t (*rm_user)(user_stack *self, int user_number);

    ret_t (*stack_destructor)(user_stack *self);
} st_op_table;

typedef struct user_base
{
    user_t *users;

    unsigned long int users_number;
    unsigned long int stack_size;

    st_op_table methods;
} user_stack;

ret_t init_user_stack(user_stack *stk_ptr);

#endif