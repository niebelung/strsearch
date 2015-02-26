#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "strsearch.h"


bool init(StrSearch * search, const char * const path)
{
//     search->buf = malloc(STRSEARCH_BUF_LEN);
//     search->buflen =  STRSEARCH_BUF_LEN;
    search->buf = malloc(20);
    search->buflen =  20;

    if (NULL == (search->stream = fopen(path, "r")))
    {
        printf("Error opening file %s : %s !!! \n",
               path,
               strerror(errno));
        return false;
    }

    fseek(search->stream, 0, SEEK_END); // seek to end of file
    search->filesize = ftell(search->stream);   // get current file pointer
    fseek(search->stream, 0, SEEK_SET); // seek back to the beginning of file

    printf("Opened file %s of size %d\n", path, search->filesize);

    memset(search->buf, 0 , sizeof(search->buf));

    return true;
}

bool find(StrSearch * search, const char * const pattern)
{
    fseek(search->stream, 0, SEEK_SET); // seek back to the beginning of file

    int patlen = strlen(pattern);
    while (patlen * 2 > search->buflen )
    {
        search->buflen *= 2;
        search->buf = (char *)realloc(search->buf, search->buflen);
    }

    for(int pos = 0; pos < search->filesize - patlen + 1; )
    {
        int posInBuf = (pos) ? patlen - 1 : 0 ;

        int bytesToRead = (search->filesize - pos > search->buflen)
                              ? search->buflen - posInBuf
                              : search->filesize - pos - posInBuf;

        if(!fread(search->buf + posInBuf, 1, bytesToRead, search->stream))
        {
            printf("Error reading %d bytes from pos %d from file : %s\n",
                   bytesToRead, pos, strerror(errno));
            return false;
        }

        for (int i = 0; i < search->buflen - patlen + 1; ++ i)
        {
            bool found = true;
            for(int j = 0; j < patlen; ++ j)
            {
                if(search->buf[i+j] != pattern[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                return true;
            }
        }

        //Copying pattern size minus one bytes to the beginning of the buffer
        memcpy(search->buf, search->buf + search->buflen - patlen + 1, patlen - 1);
        pos += bytesToRead;
    }
    return false;
}

void finalize(StrSearch * search)
{
    fclose(search->stream);
    free(search->buf);
}

