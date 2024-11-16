#ifndef __LOG_H__
#define __LOG_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "log_config.h"

#include <stdio.h>
#include <stdint.h>


#ifndef CUSTOM_LOG
    #define CUSTOM_LOG              0 
#endif /* ! CUSTOM_LOG */

typedef int (*log_handler_t)(const unsigned char *, int);


/* Defining log levels. This parameter is invalid when CUSTOM LOG == 0 */
typedef enum {
    LOG_LEVEL_OFF = 0,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_ALL,
} LogLevel;

/* This parameter is invalid when CUSTOM LOG == 0 */
typedef struct log_struct_param_t {
    LogLevel level;
    unsigned char show_location;
    unsigned char func_len;
    unsigned char file_len;
    unsigned char line_len;
    log_handler_t handler;
} log_param_t;
extern log_param_t log_param;


/* void log_param_init(LOG_PORT_TYPE *port_p, LogLevel level, uint8_t show_location); */

// void log_init(LogLevel level, unsigned char show_location);
void log_init(log_param_t param);

void log_level_set(LogLevel level);

void log_show_location_set(unsigned char show_location);

void log_show_len_set(unsigned char func_len, unsigned char file_len, unsigned char line_len);

int log_attach(log_handler_t handler);

void log_detach(void);





#if (CUSTOM_LOG == 0)

    #define LOG_SEND(format, ...) \
        #define LOG_SEND(format, ...) \
        do { \
            printf(format, ##__VA_ARGS__); \
        } while (0)

#elif (CUSTOM_LOG == 1)
    /* function declaration */
    int log_fmt(const char *fmt, ...);

    #define LOG_SEND(format, ...) \
        do { \
            log_fmt(format, ##__VA_ARGS__); \
        } while (0)

    #define log_type(level, format, ...) \
        do { \
            switch (level) \
            { \
                case LOG_LEVEL_FATAL: \
                    LOG_SEND("\n%-*.*s    %-*.*s    line:%-*d    [F]:" format,\
                            __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break; \
                case LOG_LEVEL_ERR: \
                    LOG_SEND("\n%-*.*s    %-*.*s    line:%-*d    [E]:" format,\
                            __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break; \
                case LOG_LEVEL_WARN: \
                    LOG_SEND("\n%-*.*s    %-*.*s    line:%-*d    [W]:" format,\
                            __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break; \
                case LOG_LEVEL_INFO: \
                    LOG_SEND("\n%-*.*s    %-*.*s    line:%-*d    [I]:" format,\
                            __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break; \
                case LOG_LEVEL_ALL: \
                    LOG_SEND("\n%-*.*s    %-*.*s    line:%-*d    [D]:" format,\
                            __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
                    break; \
                    \
                default: \
                    break; \
            } \
        } while (0)



    #define log_fatal(format, ...) \
        do { \
            if(log_param.level >= LOG_LEVEL_FATAL)\
                LOG_SEND("\n%s()    %s    line:%d    [F]:" format, \
                        __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
        } while (0)

    #define log_err(format, ...) \
        do { \
            if(log_param.level >= LOG_LEVEL_ERR)\
                LOG_SEND("\n%s()    %s    line:%d    [E]:" format, \
                        __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
        } while (0)

    #define log_warn(format, ...) \
        do { \
            if(log_param.level >= LOG_LEVEL_WARN) \
                LOG_SEND("\n%s()    %s    line:%d    [W]:" format, \
                        __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
        } while (0)

    #define log_info(format, ...) \
        do { \
            if(log_param.level >= LOG_LEVEL_INFO) \
                LOG_SEND("\n%s()    %s    line:%d    [I]:" format, \
                        __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
        } while (0)

    #define log_debug(format, ...) \
        do { \
            if(log_param.level >= LOG_LEVEL_ALL) \
                LOG_SEND("\n%s()    %s    line:%d    [D]:" format, \
                        __func__, __FILE__, __LINE__, ##__VA_ARGS__); \
        } while (0)

#endif /*  CUSTOM_LOG == ? */


#define log_printf(format, ...) \
    do { \
        if (log_param.level < LOG_LEVEL_INFO) break; \
        if (log_param.show_location) { \
            if (log_param.line_len != 0) { \
                LOG_SEND("\n%-*.*s    %-*.*s    line:%-*d    [I]:", \
                        log_param.func_len, log_param.func_len, __func__, \
                        log_param.file_len, log_param.file_len, __FILE__, \
                        log_param.line_len, __LINE__); \
            } else { \
                LOG_SEND("\n%-*.*s    %-*.*s    [I]:", \
                        log_param.func_len, log_param.func_len, __func__, \
                        log_param.file_len, log_param.file_len, __FILE__); \
            } \
        } \
        LOG_SEND(format, ##__VA_ARGS__); \
    } while (0)


#ifdef __cplusplus
}
#endif

#endif /* ! __LOG_H__ */


