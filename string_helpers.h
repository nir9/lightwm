#pragma once

void trim(char* str);
void removeControlChars(char* str);

inline void strip(char* str)
{
    size_t len = strlen(str);
    char *start = str;
    char *end = str + len - 1;

    while (*start == ' ')
    {
        start++;
    }

    while (*end == ' ')
    {
        end--;
    }

    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

inline void removeControlChars(char* str)
{
    int i, j = 0;
    char temp[1024];

    for (i = 0; str[i] != '\0'; ++i)
    {
        if ((unsigned char)str[i] < 32) continue; // Skip control characters
        temp[j] = str[i];
        ++j;
    }

    temp[j] = '\0';
    strncpy(str, temp, strlen(str));
}
