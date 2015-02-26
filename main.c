#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "strsearch.h"

#define INPUT_STRING_DEFAULT_SIZE 32

typedef enum _LineEnding
{
    LE_DOS,
    LE_UNIX,
    LE_MAC,
    LE_NONE
}LineEnding;

static bool readStdIn(char *s, int size)
{
    printf("Ready to process string: \n");
    return NULL != fgets(s, size, stdin);
}

static LineEnding getLineEnding(char * line)
{
    int len = strlen(line);
    if(line[len-1] == '\x0D')
    {
        return LE_MAC;
    }
    else if(line[len-2] == '\x0D' &&
            line[len-1] == '\x0A')
    {
        return LE_DOS;
    }
    else if(line[len-1] == '\x0A')
    {
        return LE_UNIX;
    }
    else
    {
        return LE_NONE;
    }
}

static char * trimLine(char * line)
{
    int len = strlen(line);

    switch(getLineEnding(line))
    {
        case LE_UNIX:
        case LE_MAC:
            line[len - 1] = '\0';
            return line;
        case LE_DOS:
            line[len - 2] = '\0';
            return line;
        default:
            return line;

    }
}

int main(int argc, char ** argv)
{
    if(argc - 1 != 1)
    {
        printf("Wrong arguments number : %d\n",argc - 1);
        return EXIT_FAILURE;
    }

    char * filename = argv[1];

    StrSearch search;
    if(!init(&search, filename))
    {
        printf("Failed to initialize StrSearch\n");
        finalize(&search);
        return EXIT_FAILURE;
    }

    int inputStringSize = INPUT_STRING_DEFAULT_SIZE;

    char * pattern = (char *)malloc(inputStringSize);
    memset(pattern, 0xFF ,inputStringSize);

    int pos = 0;

    while(readStdIn(pattern + pos, inputStringSize - pos))
    {

        //Couldn't read the whole string so doubling the buffer
        if(LE_NONE == getLineEnding(pattern))
        {
            char * tmp = (char *)malloc(inputStringSize*2);
            memcpy(tmp, pattern, inputStringSize);
            free(pattern);
            pattern = tmp;
            pos = strlen(pattern);
            inputStringSize *= 2;
        }
        else
        {
            pos = 0;
            if(strlen(trimLine(pattern)))
            {
                if(!strcmp(pattern,"exit"))
                {
                    finalize(&search);
                    return EXIT_SUCCESS;
                }
                const char * result  = (find(&search,pattern)) ? "YES" : "NO";
                printf("%s\n",result);
            }
        }

        if(feof(stdin))
        {
            break;
        }
    }

    finalize(&search);
    return EXIT_SUCCESS;
}