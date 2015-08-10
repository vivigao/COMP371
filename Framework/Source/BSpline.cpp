#include <GL/glew.h>
//#include <GL/glut.h>
#include "BSpline.h"
#include "Renderer.h"
#include <vector>

using namespace std;
using namespace glm;

/*float* GetPoint(int i, ){
	// return 1st point  
	if (i<0)
		return  Points[0];

	if (i<noOfPoints)
		return Points[i];
	// return last point  

	return Points[noOfPoints - 1];
}//*/

BSpline::BSpline(vec3 color) : Model() {
	noOfPoints = 92;
	LOD = 50;
	Vertex vertexBuffer[] = {
			// position,             normal,                 color
			{ vec3(10, 7, 0.2),		vec3(10, 7, 0.2),		color },
			{ vec3(10, 8, 0.21),	vec3(10, 8, 0.21),		color },
			{ vec3(8, 10, 0.22),	vec3(8, 10, 0.22),		color },
			{ vec3(5, 10, 0.23),	vec3(5, 10, 0.23),		color },
			{ vec3(-5, 10, 5.0),	vec3(-5, 10, 5.0),		color },
			{ vec3(-8, 10, 5.1),	vec3(-8, 10, 5.1),		color },
			{ vec3(-10, 8, 5.05),	vec3(-10, 8, 5.05),		color },
			{ vec3(-10, 7, 1.0),	vec3(-10, 7, 1.0),		color },
			{ vec3(-10, 4, 0.5),	vec3(-10, 4, 0.5),		color },
			{ vec3(-10, 1, 4.0),	vec3(-10, 1, 4.0),		color },
			{ vec3(-10, -2, 0.2),	vec3(-10, -2, 0.2),		color },
			{ vec3(-8.5, -4, 4.0),	vec3(-8.5, -4, 4.0),	color },
			{ vec3(-6, -6, 0.1),	vec3(-6, -6, 0.1),		color },
			{ vec3(-8.5, -8, 4.0),	vec3(-8.5, -8, 4.0),	color },
			{ vec3(-10, -6, 2.5),	vec3(-10, -6, 2.5),		color },
			{ vec3(-8.5, -3, 0.2),	vec3(-8.5, -3, 0.2),	color },
			{ vec3(-9, 1, 3.5),		vec3(-9, 1, 3.5),		color },
			{ vec3(-6, -0.5, 1.0),	vec3(-6, -0.5, 1.0),	color },
			{ vec3(-3, -2, 4.0),	vec3(-3, -2, 4.0),		color },
			{ vec3(-6, -4, 0.2),	vec3(-6, -4, 0.2),		color },
			{ vec3(-8, -2, 0.1),	vec3(-8, -2, 0.1),		color },
			{ vec3(-8, 1, 4.0),		vec3(-8, 1, 4.0),		color },
			{ vec3(-8, 4, 0.5),		vec3(-8, 4, 0.5),		color },
			{ vec3(-8, 7, 0.2),		vec3(-8, 7, 0.2),		color },
			{ vec3(-6.5, 8.5, 0.2), vec3(-6.5, 8.5, 0.2),	color },
			{ vec3(-5, 6, 0.2),		vec3(-5, 6, 0.2),		color },
			{ vec3(-5.0, 4, 2),		vec3(-5.0, 4, 2),		color },
			{ vec3(-5, 2, 3.5),		vec3(-5, 2, 3.5),		color },
			{ vec3(-5.0, 0, 2.5),	vec3(-5.0, 0, 2.5),		color },
			{ vec3(-5, -2, 2.5),	vec3(-5, -2, 2.5),		color },
			{ vec3(-5.0, -4, 4),	vec3(-5.0, -4, 4),		color },
			{ vec3(-5, -6, 3.5),	vec3(-5, -6, 3.5),		color },
			{ vec3(-5, -8, 5),		vec3(-5, -8, 5),		color },
			{ vec3(-3.5, -9.5, 5),	vec3(-3.5, -9.5, 5),	color },
			{ vec3(-2, -8, 5),		vec3(-2, -8, 5),		color },
			{ vec3(-2, -7, 4.5),	vec3(-2, -7, 4.5),		color },
			{ vec3(-2, -6, 4.4),	vec3(-2, -6, 4.4),		color },
			{ vec3(-2, -5, 4),		vec3(-2, -5, 4),		color },
			{ vec3(-2, -4, 3.9),	vec3(-2, -4, 3.9),		color },
			{ vec3(-2, -3, 3.5),	vec3(-2, -3, 3.5),		color },
			{ vec3(-2, -2, 3.4),	vec3(-2, -2, 3.4),		color },
			{ vec3(-2, -1, 3),		vec3(-2, -1, 3),		color },
			{ vec3(-2, 0, 2.8),		vec3(-2, 0, 2.8),		color },
			{ vec3(-2, 1, 2.5),		vec3(-2, 1, 2.5),		color },
			{ vec3(-2, 2, 2.3),		vec3(-2, 2, 2.3),		color },
			{ vec3(-2, 3, 2),		vec3(-2, 3, 2),			color },
			{ vec3(-2, 4, 1.9),		vec3(-2, 4, 1.9),		color },
			{ vec3(-2, 5, 1.6),		vec3(-2, 5, 1.6),		color },
			{ vec3(-2, 6, 1.4),		vec3(-2, 6, 1.4),		color },
			{ vec3(0, 9, 0.3),		vec3(0, 9, 0.3),		color },
			{ vec3(7, 9, 0.2),		vec3(7, 9, 0.2),		color },
			{ vec3(9, 7, 0.2),		vec3(9, 7, 0.2),		color },
			{ vec3(9, -7, 0.2),		vec3(9, -7, 0.2),		color },
			{ vec3(7, -9, 0.7),		vec3(7, -9, 0.7),		color },
			{ vec3(5, -9, 5.1),		vec3(5, -9, 5.1),		color },
			{ vec3(4, -8, 5.1),		vec3(4, -8, 5.1),		color },
			{ vec3(4, -6, 1.1),		vec3(4, -6, 1.1),		color },
			{ vec3(6, -4, 0.2),		vec3(6, -4, 0.2),		color },
			{ vec3(8, -6, 0.2),		vec3(8, -6, 0.2),		color },
			{ vec3(6, -8, 0.2),		vec3(6, -8, 0.2),		color },
			{ vec3(3, -5, 0.2),		vec3(3, -5, 0.2),		color },
			{ vec3(6, -2, 0.2),		vec3(6, -2, 0.2),		color },
			{ vec3(8, 0, 2.2),		vec3(8, 0, 2.2),		color },
			{ vec3(6.5, 1.5, 0.2),	vec3(6.5, 1.5, 0.2),	color },
			{ vec3(5, 3, 2.2),		vec3(5, 3, 2.2),		color },
			{ vec3(6.5, 4.5, 0.2),	vec3(6.5, 4.5, 0.2),	color },
			{ vec3(8, 6, 2.2),		vec3(8, 6, 2.2),		color },
			{ vec3(6.5, 7.5, 0.2),	vec3(6.5, 7.5, 0.2),	color },
			{ vec3(5, 6, 2.2),		vec3(5, 6, 2.2),		color },
			{ vec3(3.5, 4.5, 0.2),	vec3(3.5, 4.5, 0.2),	color },
			{ vec3(2, 6, 2.2),		vec3(2, 6, 2.2),		color },
			{ vec3(0.5, 7.5, 0.2),	vec3(0.5, 7.5, 0.2),	color },
			{ vec3(-1, 6, 2.2),		vec3(-1, 6, 2.2),		color },
			{ vec3(0.5, 4.5, 0.2),	vec3(0.5, 4.5, 0.2),	color },
			{ vec3(2, 3, 2.2),		vec3(2, 3, 2.2),		color },
			{ vec3(0.5, 1.5, 0.2),	vec3(0.5, 1.5, 0.2),	color },
			{ vec3(-1, 0, 2.2),		vec3(-1, 0, 2.2),		color },
			{ vec3(0.5, -1.5, 0.2),	vec3(0.5, -1.5, 0.2),	color },
			{ vec3(2, 0, 1.2),		vec3(2, 0, 1.2),		color },
			{ vec3(3.5, 1.5, 2.2),	vec3(3.5, 1.5, 2.2),	color },
			{ vec3(5, 0, 0.3),		vec3(5, 0, 0.3),		color },
			{ vec3(3.5, -1.5, 3.4), vec3(3.5, -1.5, 3.4),	color },
			{ vec3(2, -3, 1.0),		vec3(2, -3, 1.0),		color },
			{ vec3(0.2, -4.5, 3.1), vec3(0.2, -4.5, 3.1),	color },
			{ vec3(-1.6, -7, 4.5),	vec3(-1.6, -7, 4.5),	color },
			{ vec3(8, -10, 0.2),	vec3(8, -10, 0.2),		color },
			{ vec3(10, -8, 0.2),	vec3(10, -8, 0.2),		color },
			{ vec3(10, 0, 0.19),	vec3(10, 0, 0.19),		color },
			{ vec3(10, 7, 0.2),		vec3(10, 7, 0.2),		color },
			{ vec3(10, 8, 0.21),	vec3(10, 8, 0.21),		color },
			{ vec3(8, 10, 0.22),	vec3(8, 10, 0.22),		color },
			{ vec3(5, 10, 0.23),	vec3(5, 10, 0.23),		color },
	};
	numOfVertices = sizeof(vertexBuffer) / sizeof(Vertex);

	glGenVertexArrays(1, &mVertexArrayID);
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);//*/
}

BSpline::~BSpline(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void BSpline::Draw(){
	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(0,				// attribute. No particular reason for 0, but must match the layout in the shader.
		3,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		sizeof(Vertex), // stride
		(void*)0        // array buffer offset
		);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
		);

	// 3rd attribute buffer : vertex color
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
		);

	// Draw the triangles !
	glDrawArrays(GL_LINE_STRIP, 0, noOfPoints); // 92 nb of pts

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void BSpline::Update(float dt){ Model::Update(dt); }

bool BSpline::ParseLine(const std::vector<ci_string> &token){
	if (token.empty())
		return true;
	else
		return Model::ParseLine(token);
}

/*void onUpdate(int w, int h){
	if (h == 0)
		h = 1;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// just use a perspective projection
	gluPerspective(45, (float)w / h, 0.1, 100);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
}

float* GetPoint(int i){
	// return 1st point  
	if (i<0)
		return  controlP[0];

	if (i<noOfPoints)
		return controlP[i];
	// return last point  

	return controlP[noOfPoints - 1];
}

void onDraw(){
	// clear the screen & depth buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// clear the previous transform
	glLoadIdentity();

	// set the camera position
	gluLookAt(1, 10, 30,	//	eye pos
		0, 0, 0,	//	aim point
		0, 1, 0);	//	up direction

	glColor3f(1, 1, 0);
	glBegin(GL_LINE_STRIP);

	for (int start_cv = 0, j = 0; j<noOfPoints; j++, start_cv++){
		// for each section of curve, draw LOD number of divisions  
		for (int i = 0; i != LOD; ++i){
			// use the parametric time value 0 to 1 for this curve  
			// segment.  
			float t = (float)i / LOD;
			// the t value inverted  
			float it = 1.0f - t;

			// calculate blending functions for cubic bspline  
			float b0 = it*it*it / 6.0f;
			float b1 = (3 * t*t*t - 6 * t*t + 4) / 6.0f;
			float b2 = (-3 * t*t*t + 3 * t*t + 3 * t + 1) / 6.0f;
			float b3 = t*t*t / 6.0f;

			// calculate the x,y and z of the curve point  
			float x = b0 * GetPoint(start_cv)[0] +
				b1 * GetPoint(start_cv + 1)[0] +
				b2 * GetPoint(start_cv + 2)[0] +
				b3 * GetPoint(start_cv + 3)[0];

			float y = b0 * GetPoint(start_cv + 0)[1] +
				b1 * GetPoint(start_cv + 1)[1] +
				b2 * GetPoint(start_cv + 2)[1] +
				b3 * GetPoint(start_cv + 3)[1];

			float z = b0 * GetPoint(start_cv + 0)[2] +
				b1 * GetPoint(start_cv + 1)[2] +
				b2 * GetPoint(start_cv + 2)[2] +
				b3 * GetPoint(start_cv + 3)[2];

			// specify the point
			glVertex3f(x, y, z);
		}
	}
	glEnd();

	// draw the control controlP
	glColor3f(0, 1, 0);
	glPointSize(3);
	glBegin(GL_POINTS);
	for (int i = 0; i != noOfPoints; ++i) {
		glVertex3fv(controlP[i]);
	}
	glEnd();

	// draw the hull of the curve
	glColor3f(0, 1, 1);
	//glBegin(GL_LINE_STRIP);
	for (int i = 0; i != noOfPoints; ++i) {
		glVertex3fv(controlP[i]);
	}
	glEnd();

	// currently we've been drawing to the back buffer, we need
	// to swap the back buffer with the front one to make the image visible
//	glutSwapBuffers();
}//*/

/*void BSpline::setFunctions() {
	// set the function to use to draw our scene
	glutDisplayFunc(onDraw);

	// set the function to handle changes in screen size
	glutReshapeFunc(onUpdate);

	// set the function for the key presses
	//glutKeyboardFunc(OnKeyPress);
}//*/