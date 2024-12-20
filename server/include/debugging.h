#ifndef DEBUGGING_FUNCS
#define DEBUGGING_FUNCS

#define STARTING_USER_NUMBER 64

#define LOG(...)

#define LOG_ERR(...)

#define _RETURN_ON_TRUE(statement, ret_val, ...)            \
    do                                                      \
    {                                                       \
        if (statement)                                      \
        {                                                   \
            __VA_ARGS__                                     \
            return ret_val;                                 \
        }                                                   \
    } while (0)


enum SERVER_ERRORS
{
    USER_STACK_INIT_ERR     = 1321,
    USER_DATA_BASE_INIT_ERR = 1322,
    USER_DATABASE_REALC_ERR = 1323,
    STACK_ADD_ERR           = 1324,
};


#endif