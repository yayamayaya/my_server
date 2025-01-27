#include <stdio.h>
#include "debugging.h"
#include "server_start.h"

int main(int argc, char const *argv[])
{
    open_log();

    ret_t ret_val = run_server();

    close_log();
    return ret_val;
}
