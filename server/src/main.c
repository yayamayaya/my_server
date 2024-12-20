#include <stdio.h>
#include "data_base_work.h"
#include "debugging.h"

int main(int argc, char const *argv[])
{
    int ret_val = read_data_base();
    _RETURN_ON_TRUE(ret_val, ret_val);

    ret_val = register_new_user("sashanya11", "hui_tebe", "./new_rcv", "./new_snd");
    _RETURN_ON_TRUE(ret_val, ret_val);

    ret_val = check_pswd("user", "123");
    LOG("> user password: %d\n", ret_val);

    user_base_dump();

    return 0;
}
