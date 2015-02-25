#ifndef _STRSEARCH_H_
#define _STRSEARCH_H_

#include <stdint.h>
#include <stdbool.h>

#define STRSEARCH_BUF_LEN 1024*1024

typedef struct _StrSearch
{
    FILE * stream;
    int filesize;
    char * buf;
    int buflen;
} StrSearch;

bool init(StrSearch * search, const char * const path);

bool find(StrSearch * search, const char * const pattern);

void finalize(StrSearch * search);


#endif
