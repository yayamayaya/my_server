#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "debugging.h"
#include "structs.h"

ret_t user_stack_destructor(user_stack *self);

static ret_t add_user(user_stack *self, user_t user);

static ret_t increase_stack_size(user_stack *self);

no_ret_val_t rm_user(user_stack *self, int user_number);

ret_t init_user_stack(user_stack *stk_ptr)
{
    assert(stk_ptr);

    stk_ptr = (user_stack *)calloc(1, sizeof(user_stack));
    _RETURN_ON_TRUE(!stk_ptr, USER_STACK_INIT_ERR, LOG("> couldn't allocate memory for user stack\n"););

    stk_ptr->users  = (user_t *)calloc(STARTING_USER_NUMBER, sizeof(user_t));
    _RETURN_ON_TRUE(!stk_ptr->users, USER_DATA_BASE_INIT_ERR, LOG("> couldn't allocate memory for data base\n"););

    stk_ptr->users_number   = 0;
    stk_ptr->stack_size     = STARTING_USER_NUMBER;

    stk_ptr->methods.add_user           = add_user;
    stk_ptr->methods.rm_user            = rm_user;
    stk_ptr->methods.stack_destructor   = user_stack_destructor;

    return 0;
}

//Периодическое обновление базы данных через дополнительную нить исполнения
ret_t user_stack_destructor(user_stack *self)
{
    assert(self);

    //save_data_base()

    self->stack_size    = 0;
    self->users_number  = 0;

    free(self->users);
    self->users = NULL;

    free(self);

    return 0;
}

ret_t increase_stack_size(user_stack *self)
{
    assert(self);

    LOG("> increasing user staack size\n");

    self->stack_size <<= 2;
    user_t *base_holder = (user_t *)realloc(self->users, self->stack_size * sizeof(user_t));
    _RETURN_ON_TRUE(!base_holder, USER_DATABASE_REALC_ERR, LOG("> couldn't increase memory for data base\n"); self->stack_size >>= 2;);

    self->users = base_holder;

    LOG("> success\n");

    return 0;
}

ret_t add_user(user_stack *self, user_t user)
{
    assert(self);

    ret_t ret_val = 0;

    if (self->users_number == self->stack_size)
    {
        ret_val = increase_stack_size(self);
        _RETURN_ON_TRUE(ret_val, ret_val);
    }

    self->users[self->users_number] = user;
    self->users_number++;

    return 0;
}

no_ret_val_t rm_user(user_stack *self, int user_number)
{
    assert(self);

    user_t user_holder = self->users[self->users_number - 1];
    user_holder.user_number = user_number;
    self->users[user_number] = user_holder;

    self->stack_size--;

    return;
}

