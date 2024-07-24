#ifndef INC_JPOGLWRITE_H
#define INC_JPOGLWRITE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <tchar.h>

extern bool JPOGL_writeEnabled;

#define MAX_JPOGL_WRITELINE_STR_LEN 4096
extern int (* JPOGL_write_ptr)(const char *);
extern int (* JPOGL_writeArbLen_ptr)(char *);
extern int (* JPOGL_writeLine_ptr)(const char *);
extern int JPOGL_write(const char *msg, ...);
extern int JPOGL_writeArbLen(char *msg);
extern int JPOGL_writeLine(const char *msg, ...);
extern void JPOGL_writeError(const char *msg, ...);



#endif


