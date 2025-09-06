#include "main.h"

void die(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf(stderr, "%s[-]%s ", COLOR_RED, COLOR_RESET);
    vfprintf(stderr, fmt, ap);
    if (errno != 0) {
        perror(" ");
    } else {
        fprintf(stderr, "\n");
    }
    va_end(ap);
    // printf("Exited with exit code : 1\n");
    exit(EXIT_FAILURE);
}

void print(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf(stderr, "%s[+]%s ", COLOR_GREEN, COLOR_RESET);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

bool endswith(const char* str, char* suffix)
{
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);

    if (lensuffix > lenstr)
        return false;

    return strcmp(str + lenstr - lensuffix, suffix) == 0;
}

int isnum(const char* str)
{
    if (*str == '\0')
        return 0;
    if (*str == '+' || *str == '-') {
        str++;
        if (*str == '\0')
            return 0;
    }
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}
