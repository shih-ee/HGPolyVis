#ifndef INC_JPGLERROR_H
#define INC_JPGLERROR_H

#include <GL/glu.h>
#include "JPOGLWrite.h"

static int checkGLError(const char *file, int line){
	GLenum glError;
	int returnCode = 0;

	glError = glGetError();
	while (glError != GL_NO_ERROR) 
	{
		JPOGL_writeError("GL Error #%d(%s) in File %s at line: %d", glError, gluErrorString(glError), file, line);
		returnCode = 1;
		glError = glGetError();
	}
	return returnCode;
}
#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)


#endif

