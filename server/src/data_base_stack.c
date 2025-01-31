#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "debugging.h"
#include "data_base_stack.h"
//#include "structs.h"

no_ret_val_t user_stack_destructor(user_stack *self);

static ret_t add_user(user_stack *self, user_t user);

static ret_t increase_stack_size(user_stack *self);

no_ret_val_t rm_user(user_stack *self, unsigned long int user_number);

unsigned long int find_user(user_stack *self, const char *username);

user_t return_usr_data(user_stack *self, unsigned long int user_number);

void stack_dump(user_stack *self);


ret_t init_user_stack(user_stack **stk_ptr)
{
    assert(stk_ptr);

    user_stack *stack = (user_stack *)calloc(1, sizeof(user_stack));
    _RETURN_ON_TRUE(!stack, USER_STACK_INIT_ERR, LOG("]> couldn't allocate memory for user stack\n"););

    stack->users  = (user_t *)calloc(STARTING_USER_NUMBER, sizeof(user_t));
    _RETURN_ON_TRUE(!stack->users, USER_DATA_BASE_INIT_ERR, LOG("]> couldn't allocate memory for data base\n"););

    stack->users_number   = 0;
    stack->stack_size     = STARTING_USER_NUMBER;

    stack->methods.add_user           = add_user;
    stack->methods.rm_user            = rm_user;
    stack->methods.stack_destructor   = user_stack_destructor;
    stack->methods.find_user          = find_user;
    stack->methods.return_usr_data    = return_usr_data;
    stack->methods.dump               = stack_dump;

    *stk_ptr = stack;

    return 0;
}

no_ret_val_t user_stack_destructor(user_stack *self)
{
    assert(self);

    self->stack_size    = 0;
    self->users_number  = 0;

    free(self->users);
    self->users = NULL;

    free(self);

    return;
}

ret_t increase_stack_size(user_stack *self)
{
    assert(self);

    LOG("]> increasing user staack size\n");

    self->stack_size <<= 2;
    user_t *base_holder = (user_t *)realloc(self->users, self->stack_size * sizeof(user_t));
    _RETURN_ON_TRUE(!base_holder, USER_DATABASE_REALC_ERR, LOG("]> couldn't increase memory for data base\n"); self->stack_size >>= 2;);

    self->users = base_holder;

    LOG("]> success\n");

    return 0;
}

unsigned long int find_user(user_stack *self, const char *username)
{
    assert(self);
    assert(username);

    if (self->stack_size == 0) LOG("]> seeking trough empty stack\n");

    for (long unsigned int usr_num = 0; usr_num < self->stack_size; usr_num++)
        if (!strcmp(self->users[usr_num].username, username))
            return usr_num;
    
    return USR_NOT_FOUND;
}

user_t return_usr_data(user_stack *self, unsigned long int user_number)
{
    assert(self);

    if (user_number > self->stack_size)
    {
        LOG("]> seeking user number greater than stack size\n");
        exit(NUMBER_GREATER_SIZE);
    }

    return self->users[user_number];
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

    user.user_number = self->users_number;
    self->users[self->users_number] = user;
    self->users_number++;

    return 0;
}

no_ret_val_t rm_user(user_stack *self, unsigned long int user_number)
{
    assert(self);

    if (user_number > self->stack_size)
    {
        LOG("]> no such user number\n");
        return;
    }

    user_t user_holder = self->users[self->users_number - 1];
    user_holder.user_number = user_number;
    self->users[user_number] = user_holder;

    self->stack_size--;

    return;
}

void stack_dump(user_stack *self)
{
    assert(self);

    LOG("]> --------------------------------------------------------------------------\n");
    LOG("]> stack dump: \n");
    LOG("]> stack size: %lu\n", self->stack_size);
    LOG("]> number of users: %lu\n", self->users_number);

    for (unsigned long int i = 0; i < self->users_number; i++)
    {
        LOG("]> -------------------------------------\n");
        LOG("]> user number: %lu\n", i);
        LOG("]> username: %s\n", self->users[i].username);
        LOG("]> password: %s\n", self->users[i].password);
    }
    
    LOG("]> dump ended\n");
    LOG("]> --------------------------------------------------------------------------\n");

    return;
}

