#include <GL/glew.h>
//#include <GL/glut.h>
#include "BSpline.h"
#include "Renderer.h"
#include "SphereModel.h"
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

/*

[BSpline]
name		= "BSpline"
scaling		= 1.0 1.0 1.0
position	= 0.0 3.5 0.0
rotation	= 0.0 0.0 1.0 90*/

bool done = false;
float cpts[92][3] = {
		{ 10, 7, 0.2 },
		{ 10, 8, 0.21 },
		{ 8, 10, 0.22 },
		{ 5, 10, 0.23 },
		{ -5, 10, 5.0 },
		{ -8, 10, 5.1 },
		{ -10, 8, 5.05 },
		{ -10, 7, 1.0 },
		{ -10, 4, 0.5 },
		{ -10, 1, 4.0 },
		{ -10, -2, 0.2 },
		{ -8.5, -4, 4.0 },
		{ -6, -6, 0.1 },
		{ -8.5, -8, 4.0 },
		{ -10, -6, 2.5 },
		{ -8.5, -3, 0.2 },
		{ -9, 1, 3.5 },
		{ -6, -0.5, 1.0 },
		{ -3, -2, 4.0 },
		{ -6, -4, 0.2 },
		{ -8, -2, 0.1 },
		{ -8, 1, 4.0 },
		{ -8, 4, 0.5 },
		{ -8, 7, 0.2 },
		{ -6.5, 8.5, 0.2 },
		{ -5, 6, 0.2 },
		{ -5.0, 4, 2 },
		{ -5, 2, 3.5 },
		{ -5.0, 0, 2.5 },
		{ -5, -2, 2.5 },
		{ -5.0, -4, 4 },
		{ -5, -6, 3.5 },
		{ -5, -8, 5 },
		{ -3.5, -9.5, 5 },
		{ -2, -8, 5 },
		{ -2, -7, 4.5 },
		{ -2, -6, 4.4 },
		{ -2, -5, 4 },
		{ -2, -4, 3.9 },
		{ -2, -3, 3.5 },
		{ -2, -2, 3.4 },
		{ -2, -1, 3 },
		{ -2, 0, 2.8 },
		{ -2, 1, 2.5 },
		{ -2, 2, 2.3 },
		{ -2, 3, 2 },
		{ -2, 4, 1.9 },
		{ -2, 5, 1.6 },
		{ -2, 6, 1.4 },
		{ 0, 9, 0.3 },
		{ 7, 9, 0.2 },
		{ 9, 7, 0.2 },
		{ 9, -7, 0.2 },
		{ 7, -9, 0.7 },
		{ 5, -9, 5.1 },
		{ 4, -8, 5.1 },
		{ 4, -6, 1.1 },
		{ 6, -4, 0.2 },
		{ 8, -6, 0.2 },
		{ 6, -8, 0.2 },
		{ 3, -5, 0.2 },
		{ 6, -2, 0.2 },
		{ 8, 0, 2.2 },
		{ 6.5, 1.5, 0.2 },
		{ 5, 3, 2.2 },
		{ 6.5, 4.5, 0.2 },
		{ 8, 6, 2.2 },
		{ 6.5, 7.5, 0.2 },
		{ 5, 6, 2.2 },
		{ 3.5, 4.5, 0.2 },
		{ 2, 6, 2.2 },
		{ 0.5, 7.5, 0.2 },
		{ -1, 6, 2.2 },
		{ 0.5, 4.5, 0.2 },
		{ 2, 3, 2.2 },
		{ 0.5, 1.5, 0.2 },
		{ -1, 0, 2.2 },
		{ 0.5, -1.5, 0.2 },
		{ 2, 0, 1.2 },
		{ 3.5, 1.5, 2.2 },
		{ 5, 0, 0.3 },
		{ 3.5, -1.5, 3.4 },
		{ 2, -3, 1.0 },
		{ 0.2, -4.5, 3.1 },
		{ -1.6, -7, 4.5 },
		{ 8, -10, 0.2 },
		{ 10, -8, 0.2 },
		{ 10, 0, 0.19 },
		{ 10, 7, 0.2 },
		{ 10, 8, 0.21 },
		{ 8, 10, 0.22 },
		{ 5, 10, 0.23 }
};

float* BSpline::GetPoint(const int& i, float (&cpts)[92][3]){
	// return 1st point  
	if (i<0)
		return  cpts[0];

	if (i<sizeof(cpts))
		return cpts[i];
	// return last point  

	return cpts[sizeof(cpts) - 1];
}//*/

BSpline::BSpline(vec3 color) : Model(), noOfPoints(92), LOD(20) {

//	bool first = Spline1.empty(), second = Spline2.empty() == 0;
	Vertex vertexBuffer[1840]; //vBsize
//	Vertex middle[920];
	float t, it, b0, b1, b2, b3;
	vec3 pos;
	int j, i;
	FILE * sceneF;

	if(!done)
		sceneF = fopen ("../Assets/Scenes/spline.scene","w+");

	string a = "\n[Animation]\nname = \"splineAnime\"\n", aKey;//*/

//	if ( Spline1.empty() || Spline2.empty()){
		for (j = 0; j < noOfPoints; j++){
			// for each section of curve, draw LOD number of divisions  
			for (i = 0; i != LOD; ++i){
				//the parametric time value 0 to 1 for this curve segment and use the leftover
				t = (float)i / LOD;
				it = 1.0 - t;
				// calculate blending functions for cubic bspline  
				b0 = it*it*it / 6.0f;
				b1 = (3 * t*t*t - 6 * t*t + 4) / 6.0f;
				b2 = (-3 * t*t*t + 3 * t*t + 3 * t + 1) / 6.0f;
				b3 = t*t*t / 6.0f;

				// calculate the x,y and z of the curve point  
				pos.x = b0 * GetPoint(j, cpts)[0] +
					b1 * GetPoint(j + 1, cpts)[0] +
					b2 * GetPoint(j + 2, cpts)[0] +
					b3 * GetPoint(j + 3, cpts)[0];

				pos.y = b0 * GetPoint(j, cpts)[1] +
					b1 * GetPoint(j + 1, cpts)[1] +
					b2 * GetPoint(j + 2, cpts)[1] +
					b3 * GetPoint(j + 3, cpts)[1];

				pos.z = b0 * GetPoint(j, cpts)[2] +
					b1 * GetPoint(j + 1, cpts)[2] +
					b2 * GetPoint(j + 2, cpts)[2] +
					b3 * GetPoint(j + 3, cpts)[2];

				// specify the point
				//	glVertex3f(x, y, z);
				vertexBuffer[j*LOD + i].position = pos;
				vertexBuffer[j*LOD + i].normal = pos;
				vertexBuffer[j*LOD + i].color = color;
/*				if (Spline1.size() == 0 && Spline2.size() == 0)
					Spline1.push_back(pos);
				else
					Spline2.push_back(pos);//*/
			//	SplineV.push_back(pos);

				if(!done){
					a += "key = \"lineK" + to_string((j*LOD + i)) + "\"\ttime = " + to_string((j*LOD + i)*0.01) + "f\n";
					aKey = "[AnimationKey]\nname = \"lineK" + to_string((j*LOD + i)) + "\"\nposition = " + to_string(pos.x) + " " + to_string(-pos.z) + " " + to_string(pos.y) + "\n\n";
					
					const char* temp = aKey.c_str();
					fputs (	temp,sceneF);

					if ((j*LOD + i) == 1760)
						break;
				}//*/
			}
			if ((j*LOD + i) == 1760)
				break;
		}
		numOfVertices = sizeof(vertexBuffer) / sizeof(Vertex);

		glGenVertexArrays(1, &mVertexArrayID);
		glGenBuffers(1, &mVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);//*/


	  if (!done && sceneF!=NULL){
		  a += "[Cube]\nname  = \"Cube\"\nscaling = 1.0 1.0 1.0\nposition = 0.0 1.0 0.0\nrotation = 0.0 0.0 1.0 180\nanimation = \"splineAnime\"";

		const char* temp = a.c_str();
		fputs (	temp,sceneF);
		fclose (sceneF);
		done = true;
	  }//*/
/*	}
	else{
		for (i = 0, j = 0; i < sizeof(vertexBuffer) - 1; i += 5, j += 2){
			middle[j].position = Spline1[i];
			middle[j].normal = Spline1[i];
			middle[j].color = color;

		//	pos.x = vertexBuffer[j].position.x;
		//	pos.y = vertexBuffer[j].position.y;
		//	pos.z = vertexBuffer[j].position.z;
			middle[j + 1].position = Spline2[i];
			middle[j + 1].normal = Spline2[i];
			middle[j + 1].color = color;
		}
		numOfVertices = sizeof(middle) / sizeof(Vertex);

		glGenVertexArrays(1, &mVertexArrayID);
		glGenBuffers(1, &mVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(middle), middle, GL_STATIC_DRAW);
	}//*/
}

BSpline::~BSpline(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);

	if( remove( "spline.scene" ) != 0 )
		perror( "Error deleting file" );
	else
		puts( "File successfully deleted" );//*/

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
//	if(Spline1.size() || Spline2.size())
		glDrawArrays(GL_LINE_STRIP, 0, noOfPoints * LOD - 60); // GL_QUAD_STRIP
//	else
//		glDrawArrays(GL_LINES, 0, 920);

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
