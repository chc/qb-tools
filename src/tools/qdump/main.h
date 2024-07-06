#ifndef _MAIN_H
#define _MAIN_H
#include <stdio.h>

typedef struct {
    int last_line_number;
    int tab_index;
    FILE *fd_in;
    FILE *fd_out;
} QDumpState;

extern QDumpState g_DumpState;

void show_dump(unsigned char *data, unsigned int len, FILE *stream);

#endif //_MAIN_H