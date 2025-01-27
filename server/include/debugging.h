#ifndef DEBUGGING_FUNCS
#define DEBUGGING_FUNCS

typedef void   no_ret_val_t; 
typedef int    ret_t;

#define STARTING_USER_NUMBER 64

#define _RETURN_ON_TRUE(statement, ret_val, ...)            \
    do                                                      \
    {                                                       \
        if (statement)                                      \
        {                                                   \
            __VA_ARGS__;                                    \
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
    PIPE_CONSTRUCTION_ERR   = 1327,
    MEM_ALC_ERR             = 1328,
    MAPPING_FAILED          = 1329,

    BASE_END                = 1012,
    USR_NOT_FOUND           = 2,
    RIGHT_PSWD              = 1,
    WRONG_PSWD              = 0,
    FILE_FOUND              = 1,
    FILE_NOT_FOUND          = 7,

};

enum EXIT_STATUS
{
    NUMBER_GREATER_SIZE     = 2101,
};


void open_log();

void close_log();

void loggg(const char *format, ...);

void err_log(const char *format);

#define LOG(...)        loggg(__VA_ARGS__)

#define LOG_ERR(...)    err_log(__VA_ARGS__)

#endif