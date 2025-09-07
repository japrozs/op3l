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

char* read_file(const char* path)
{
    FILE* file = fopen(path, "rb");

    if (file == NULL) {
        die("Could not open file \"%s\"", path);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        die("Not enough memory to read \"%s\"", path);
    }
    size_t bytesRead = fread(buffer, sizeof(char), file_size, file);
    if (bytesRead < file_size) {
        die("Could not read file \"%s\"", path);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}
