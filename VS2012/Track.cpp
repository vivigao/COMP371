#include "Track.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glxew.h>




GLUnurbsObj *theNurb;

void myinit(void)
{
    glShadeModel (GL_FLAT);
    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty (theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
}

void display(void)
{
	GLfloat knots[8]={0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0};
	GLfloat ctrpoints[4][3]={
    {-.75,-0.75,0.0},
	{-.5, .75, 0.0}, 
	{.5, .75, 0.0}, 
	{.75, -.75, 0.0}
	                          };




}
GLUnurbsObj* gluNewNurbsRenderer(void);

