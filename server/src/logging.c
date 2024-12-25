#define _GNU_SOURCE

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "debugging.h"

#define LOG_PATH "./logs/server.log"

static FILE *log_file = NULL;
//сделать mkdir
void open_log()
{
    log_file = fopen(LOG_PATH, "wb");
    if (!log_file) printf("> couldn't open log file for server, resuming work without logs\n");
    
    return;
}

void close_log()
{
    if (log_file) fclose(log_file);
}

void loggg(const char *format, ...)
{
    if (!log_file) return;

    time_t t;
    time(&t);
    fprintf(log_file, "[%s]", strtok(ctime(&t), "\n"));

    va_list args;
    va_start(args, format);

    vfprintf(log_file, format, args);

    va_end(args);
}

void err_log(const char *format)
{
    if (!log_file) return;

    time_t t;
    time(&t);
    fprintf(log_file, "[%s]", strtok(ctime(&t), "\n"));

    fprintf(log_file, "> %s%s\n", format, strerrorname_np(errno));
}