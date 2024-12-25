#ifndef USER_WORK_HEDER
#define USER_WORK_HEDER

#include "structs.h"

typedef struct user_data user_t;
typedef struct user_base user_stack;
typedef struct Ops st_op_table;

typedef struct Ops
{
    ret_t (*add_user)(user_stack *self, user_t user_data);
    no_ret_val_t (*rm_user)(user_stack *self, unsigned long int user_number);

    unsigned long int (*find_user)(user_stack *self, const char *username);
    user_t (*return_usr_data)(user_stack *self, unsigned long int user_number);
    void (*dump)(user_stack *self);

    no_ret_val_t (*stack_destructor)(user_stack *self);
} st_op_table;

typedef struct user_base
{
    user_t *users;

    unsigned long int users_number;
    unsigned long int stack_size;

    st_op_table methods;
} user_stack;

#define DATA_BASE_LOCATION  "/user_data.db"

ret_t init_user_stack(user_stack **stk_ptr);

#endif