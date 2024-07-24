#include "JPOGLWrite.h"
#include "LogConsole.h"
#include <QtDebug>

bool JPOGL_writeEnabled = true;
int (* JPOGL_write_ptr)(const char *) = puts;
int (* JPOGL_writeLine_ptr)(const char *) = puts;
int (* JPOGL_writeArbLen_ptr)(char *) = NULL;

int JPOGL_write(const char *msg, ...){
	char text[MAX_JPOGL_WRITELINE_STR_LEN];
	va_list args;
	va_start(args, msg);
	
	if(JPOGL_writeEnabled){
		vsnprintf(text, sizeof(text)/sizeof(char), msg, args);
		text[MAX_JPOGL_WRITELINE_STR_LEN-1] = '\0';
		qInfo(text);
		LogConsole::writeColorConsole(1.0, 0.0, 0.0, text);
	}
	return 1;
}

int JPOGL_writeArbLen(char *msg){
	if(JPOGL_writeEnabled){
		return (*JPOGL_writeArbLen_ptr)(msg);
	}
	return 1;
}

int JPOGL_writeLine(const char *msg, ...){
	char text[MAX_JPOGL_WRITELINE_STR_LEN];
	va_list args;
	va_start(args, msg);

	if(JPOGL_writeEnabled){
		int numwritten =  vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
		text[MAX_JPOGL_WRITELINE_STR_LEN-1] = '\0';
		LogConsole::writeGreenConsole(text);
		qInfo(text);
	}
	return 1;
}


extern void JPOGL_writeError(const char *msg, ...) {

	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';

	LogConsole::writeColorConsole(1.0, 0.0, 0.0, text);
	qInfo(text);
}
