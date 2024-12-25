#include <stdio.h>
#include "data_base_work.h"
#include "debugging.h"

int main(int argc, char const *argv[])
{
    open_log();

    ret_t ret_val = read_data_base();
    _RETURN_ON_TRUE(ret_val, ret_val, close_log(););

    ret_val = register_new_user("sashanya11", "hui_tebe", "./new_rcv", "./new_snd");
    _RETURN_ON_TRUE(ret_val, ret_val, close_log(); finish_db_work(););

    pswd_t pswd_check = check_pswd("user", "123");
    LOG("> user password: %d\n", pswd_check);

    user_base_dump();

    ret_val = finish_db_work();

    close_log();
    return ret_val;
}
