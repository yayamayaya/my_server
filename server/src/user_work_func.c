#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "data_base_stack.h"
#include "data_base_work.h"
#include "user_work_func.h"
#include "debugging.h"

user_stack *data_base_init();


ret_t user_work_process()
{
    //ret_t ret_val = 0;
    LOG("]> user stack initiation:\n");
    user_stack *user_base = data_base_init();
    _RETURN_ON_TRUE(!user_base, -1);

    user_base->methods.stack_destructor(user_base);

    return 0;
}

user_stack *data_base_init()
{
    LOG("]> creating data base stack:\n");
    user_stack *user_base = NULL;
    ret_t ret_val = init_user_stack(&user_base);
    _RETURN_ON_TRUE(ret_val, NULL);

    LOG("]> parsing data base:\n");
    ret_val = read_data_base(user_base);
    _RETURN_ON_TRUE(ret_val, NULL);

    LOG("]> dumping data base:\n");
    user_base_dump(user_base);

    return user_base;
}