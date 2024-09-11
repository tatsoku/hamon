#ifndef __WIN32


#include <windows.h>

#include <hamon_strl.h>

size_t strlcpy(char* dest, const char* src, size_t size){
    return strcpy_s(dest, size, src);
}


size_t strlcat(char* dest, const char* src, size_t size){
    return strcat_s(dest, size, src);
}

// THANK YOU STACK OVERFLOW!
char *strndup(char *str, int chars)
{
    char *buffer;
    int n;

    buffer = (char *) malloc(chars +1);
    if (buffer)
    {
        for (n = 0; ((n < chars) && (str[n] != 0)) ; n++) buffer[n] = str[n];
        buffer[n] = 0;
    }

    return buffer;
}









#endif