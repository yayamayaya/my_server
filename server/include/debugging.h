#ifndef DEBUGGING_FUNCS
#define DEBUGGING_FUNCS

#define STARTING_USER_NUMBER 64

#define LOG(...)        fprintf(stderr, __VA_ARGS__)

#define LOG_ERR(...)    perror(__VA_ARGS__)

#define _RETURN_ON_TRUE(statement, ret_val, ...)            \
    do                                                      \
    {                                                       \
        if (statement)                                      \
        {                                                   \
            __VA_ARGS__                                     \
            return ret_val;                                 \
        }                                                   \
    } while (0)


enum SERVER_RET_VAL
{
    USER_STACK_INIT_ERR     = 1321,
    USER_DATA_BASE_INIT_ERR = 1322,
    USER_DATABASE_REALC_ERR = 1323,
    STACK_ADD_ERR           = 1324,
    USER_PARSE_ERR          = 1325,
    FILE_OPEN_ERR           = 1326,

    BASE_END                = 1012,
    USR_NOT_FOUND           = 1013,
    RIGHT_PSWD              = 1014,
    WRONG_PSWD              = 1015,

};

#endif