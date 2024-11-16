#include "log.h"

#include <string.h>
#include <stdlib.h>

/* #include "ez_comm.h" */

#ifndef CUSTOM_LOG
#define CUSTOM_LOG 0
#endif

#ifndef CUSTOM_PRINTF
#define CUSTOM_PRINTF 0
#endif


log_param_t log_param =
{
    .level = LOG_LEVEL_INFO,
    .show_location = 0,
    .func_len = 0,
    .file_len = 0,
    .line_len = 0
};

#if !defined(__STDC_VERSION__)
    /* __STDC_VERSION__ is defined in C99 and later */
    #include "stdlib_noniso.h"
#endif

void log_init(log_param_t param)
{
    log_level_set(param.level);
    log_show_location_set(param.show_location);
    log_show_len_set(param.func_len, param.file_len, param.line_len);
    log_param.handler = param.handler;
}

void log_level_set(LogLevel level)
{
    log_param.level = level;
}

void log_show_location_set(unsigned char show_location)
{
    log_param.show_location = show_location;
}

void log_show_len_set(unsigned char func_len, unsigned char file_len, unsigned char line_len)
{
    log_param.func_len = func_len;
    log_param.file_len = file_len;
    log_param.line_len = line_len;
}

int log_attach(log_handler_t handler)
{
    if (handler == NULL)
    {
        /* handel = NULL; */
        return -1;
    }
    log_param.handler = handler;
    return 0;
}

void log_detach(void)
{
    log_param.handler = NULL;
}


#if (CUSTOM_LOG == 1)
#include <stdarg.h>

#if (CUSTOM_PRINTF == 1)

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static int print_byte(unsigned char c)
{
    // putchar(c);  // 输出字符函数，可根据需要修改
    if (log_param.handler != NULL)
        log_param.handler(&c, 1);
    return 1;
}

static int print_char(char c)
{
    return print_byte((unsigned char)c);
}

static int print_padding(char pad_char, int width)
{
    int count = 0;
    for (int i = 0; i < width; i++) {
        count += print_char(pad_char);
    }
    return count; // 返回填充字符的数量
}

static int print_string(const char *str, int precision, int width, bool left_align)
{
    int count = 0;
    int length = 0;
    const char *s = str;
    
    /* 计算字符串长度 */
    while (*s++ && (precision < 0 || length < precision)) {
        length++;
    }
    int printed_length = length; /* Track the length of the number printed */

    /* 如果字符串长度小于宽度，填充空格 */
    if (!left_align && width > printed_length) {
        count += print_padding(' ', width - printed_length);
    }
    
    s = str; /* 重新赋值，指向字符串的开始 */
    while (*s && (precision < 0 || length-- > 0)) {
        count += print_char(*s++);
    }
    
    if (left_align && width > printed_length) {
        count += print_padding(' ', width - printed_length);
    }
    return count; /* Returns the total number of printed characters */
}

static int print_integer(unsigned long long value, int base, bool uppercase, int width, bool left_align, bool zero_pad)
{
    char buffer[32];
    char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    int length = 0;

    do {
        buffer[length++] = digits[value % base];
        value /= base;
    } while (value > 0);

    int printed_length = length; /* Track the length of the number printed */
    int count = 0;
    if (!left_align && width > printed_length) {
        count += print_padding(zero_pad ? '0' : ' ', width - printed_length);
    }

    while (length > 0) {
        count += print_char(buffer[--length]);
    }

    if (left_align && width > printed_length) {
        count += print_padding(' ', width - printed_length);
    }

    return count; /* Returns the total number of printed characters */
}

static int print_signed_integer(long long value, int base, int width, bool left_align, bool zero_pad)
{
    int count = 0;
    if (value < 0) {
        count += print_char('-');
        value = -value;
        if (width > 0) width--;
    }
    count += print_integer((unsigned long long)value, base, false, width, left_align, zero_pad);
    return count; /* Returns the total number of printed characters */
}

static int print_float(double value, int precision, int width, bool left_align, bool zero_pad)
{
    if (precision < 0) {
        precision = 6;  /* The default is 6 decimal places */
    }

    int count = 0;
    if (value < 0) {
        count += print_char('-');
        value = -value;
        width--;
    }

    int integer_part = (int)value;
    double fraction_part = value - integer_part;

    /* Process fractional parts and round them */
    for (int i = 0; i < precision; i++) {
        fraction_part *= 10;
    }
    int rounded_fraction = (int)(fraction_part + 0.5);

    char int_buffer[20];
    int int_length = 0;
    do {
        int_buffer[int_length++] = '0' + (integer_part % 10);
        integer_part /= 10;
    } while (integer_part > 0);

    char frac_buffer[10];
    int frac_length = 0;
    if (precision > 0) {
        do {
            frac_buffer[frac_length++] = '0' + (rounded_fraction % 10);
            rounded_fraction /= 10;
        } while (rounded_fraction > 0);
        
        /* Filling fractional parts is a precision requirement */
        for (int i = frac_length; i < precision; i++) {
            frac_buffer[i] = '0';
        }
        frac_length = precision;
    }

    /* Including decimal point */
    int total_length = int_length + (precision > 0 ? frac_length + 1 : 0);
    char pad_char = zero_pad ? '0' : ' ';

    if (!left_align && width > total_length) {
        count += print_padding(pad_char, width - total_length);
    }

    while (int_length > 0) {
        count += print_char(int_buffer[--int_length]);
    }

    if (precision > 0) {
        count += print_char('.');
        for (int i = frac_length - 1; i >= 0; i--) {
            count += print_char(frac_buffer[i]);
        }
    }

    if (left_align && width > total_length) {
        count += print_padding(' ', width - total_length);
    }

    return count; /* Returns the total number of printed characters */
}

/**
 * @brief Custom printf function with extended format specifiers.
 *
 * This function replicates the behavior of the standard printf function
 * while providing support for various format specifiers including:
 * - Integers: %d, %u, %ld, %lu, %lld, %llu
 * - Floating point numbers: %f, %lf
 * - Characters: %c
 * - Strings: %s
 * - Pointers: %p
 * - Octal: %o
 * - Hexadecimal: %x, %X
 * - Width and precision modifiers for formatting
 *
 * Default precision for floating-point numbers is set to 6 decimal places.
 *
 * @param fmt The format string that specifies how subsequent arguments are
 *            converted for output.
 * @param ... The additional arguments to be formatted and printed.
 * @return The total number of characters printed (excluding the null byte).
 */
__attribute__((weak)) int log_fmt(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int printed = 0;  /* Cumulative number of printed characters */

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            /* Check for separate % or unexpected end */
            if (*fmt == '\0') {
                va_end(args);
                log_fmt("Invalid format string in log_fmt. For details, open show_location\n");
                return -1;  /* Returning -1 indicates an error: an incomplete formatter */
            }

            /* Check %% */
            if (*fmt == '%') {
                printed += print_char('%');
                fmt++;
                continue;
            }
            
            bool left_align = false;
            bool zero_pad = false;
            int width = 0;
            int precision = -1;
            bool long_modifier = false;
            bool long_long_modifier = false;
            bool short_modifier = false;

            if (*fmt == '-') {
                left_align = true;
                fmt++;
            }
            if (*fmt == '0') {
                zero_pad = true;
                fmt++;
            }
            
            /* 检查宽度是否为动态值 */
            if (*fmt == '*') {
                width = va_arg(args, int);
                if (width < 0) {
                    left_align = true;
                    width = -width;
                }
                fmt++;
            } else {
                while (*fmt >= '0' && *fmt <= '9') {
                    width = width * 10 + (*fmt++ - '0');
                }
            }

            /* 检查精度是否为动态值 */
            if (*fmt == '.') {
                fmt++;
                if (*fmt == '*') {
                    precision = va_arg(args, int);
                    fmt++;
                } else {
                    precision = 0;
                    while (*fmt >= '0' && *fmt <= '9') {
                        precision = precision * 10 + (*fmt++ - '0');
                    }
                }
            }
            
            if (*fmt == 'l') {
                fmt++;
                if (*fmt == 'l') {
                    long_long_modifier = true;
                    fmt++;
                } else {
                    long_modifier = true;
                }
            } else if (*fmt == 'h') {
                fmt++;
                short_modifier = true;
            }

            switch (*fmt++) {
                case 'd': case 'i':
                    if (long_long_modifier) {
                        printed += print_signed_integer(va_arg(args, long long), 10, width, left_align, zero_pad);
                    } else if (long_modifier) {
                        printed += print_signed_integer(va_arg(args, long), 10, width, left_align, zero_pad);
                    } else if (short_modifier) {
                        printed += print_signed_integer((short)va_arg(args, int), 10, width, left_align, zero_pad);
                    } else {
                        printed += print_signed_integer(va_arg(args, int), 10, width, left_align, zero_pad);
                    }
                    break;
                case 'u':
                    if (long_long_modifier) {
                        printed += print_integer(va_arg(args, unsigned long long), 10, false, width, left_align, zero_pad);
                    } else if (long_modifier) {
                        printed += print_integer(va_arg(args, unsigned long), 10, false, width, left_align, zero_pad);
                    } else {
                        printed += print_integer(va_arg(args, unsigned int), 10, false, width, left_align, zero_pad);
                    }
                    break;
                case 'o':
                    printed += print_integer(va_arg(args, unsigned int), 8, false, width, left_align, zero_pad);
                    break;
                case 'x':
                    printed += print_integer(va_arg(args, unsigned int), 16, false, width, left_align, zero_pad);
                    break;
                case 'X':
                    printed += print_integer(va_arg(args, unsigned int), 16, true, width, left_align, zero_pad);
                    break;
                case 'f':
                    printed += print_float(va_arg(args, double), precision, width, left_align, zero_pad);
                    break;
                case 'c':
                    printed += print_char((char)va_arg(args, int));
                    break;
                case 's':
                    printed += print_string(va_arg(args, const char *), precision, width, left_align);
                    break;
                case 'p':
                    printed += print_integer((uintptr_t)va_arg(args, void *), 16, false, width, left_align, zero_pad);
                    break;
                default:
                    printed += print_char('%');
                    printed += print_char(*(fmt - 1));
                    va_end(args);
                    log_fmt("Invalid format string in log_fmt. For details, open show_location\n");
                    return -1;
                    break;
            }
        } else {
            printed += print_char(*fmt++);
        }
    }

    va_end(args);
    return printed;  /* 返回总打印字符数 */
}
#endif /* CUSTOM_PRINTF == 1 */



/* Modify the implementation of the log printf function */
int log_fmt(const char *fmt, ...)
{
    if(log_param.handler == NULL) {
        printf("%s: log_handel is NULL\n", __FILE__);
        return -1;
    }

    char loc_buf[64];
    char *temp = loc_buf;
    int len;
    va_list args;
    va_start(args, fmt);

    len = vsnprintf(NULL, 0, fmt, args);
    if(len >= (int)sizeof(loc_buf)) {
        temp = (char*)malloc(len + 1);
        if(temp == NULL) {
            return 0;
        }
    }

    va_start(args, fmt);
    len = vsnprintf((char *)temp, len + 1, fmt, args);
    va_end(args);

    log_param.handler((unsigned char *)temp, len);

    /* if(len >= (int)sizeof(loc_buf)) {
        free(temp);
    } */

    if(temp != loc_buf) {
        free(temp);
    }

    return len;
}




// int log_fmt(const char *fmt, ...)
// {
//     char loc_buf[64];
//     char * temp = loc_buf;
//     va_list arg;
//     va_list copy;
//     va_start(arg, fmt);
//     va_copy(copy, arg);
//     int len = ez_vsnprintf(temp, sizeof(loc_buf), fmt, copy);
//     va_end(copy);
//     if(len < 0) {
//         va_end(arg);
//         return 0;
//     }
//     if(len >= (int)sizeof(loc_buf)) {  /* comparation of same sign type for the compiler */
//         temp = (char*) malloc(len+1);
//         if(temp == NULL) {
//             va_end(arg);
//             return 0;
//         }
//         len = ez_vsnprintf(temp, len+1, fmt, arg);
//     }
//     va_end(arg);
//     /* len = write((uint8_t*)temp, len); */
//     len = log_param.handler((unsigned char *)temp, len);
//     if(temp != loc_buf) {
//         free(temp);
//     }
//     return len;
// }

#endif /* CUSTOM_LOG == ? */
