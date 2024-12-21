#include "stdafx.h"
#include "utility.h"



GLboolean Get3DCursorPos(GLint winx, GLint winy, GLdouble point[3])
{
	GLfloat		winz;
	GLint		viewport[4];
	GLdouble	modelview[16];
	GLdouble	projection[16];

	// Get current GL matrix
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	// Get cursor position
	winy = viewport[3] - winy;
	glReadPixels(winx, winy, 1, 1,
		GL_DEPTH_COMPONENT, GL_FLOAT, &winz);	// Get depth of cursor

	if (winz == 1.0)
		return GL_FALSE;

	// Unproject cursor position
	gluUnProject(winx, winy, winz,
		modelview, projection, viewport,
		&point[0], &point[1], &point[2]);

	return GL_TRUE;
}


