// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#include "WaterModel.h"
#include "Renderer.h"

#include <stdio.h>
// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
using namespace glm;

//	vec3 halfSize = size * 0.5f;

WaterModel::WaterModel(vec3 size) : Model(){
  
  /*
   * Populate INDEX buffer
   */
  short int row, col;
  
  for (row=0; row<mRows-1; row++) { // 0 -> 1
    
    for (col=0; col<mColumns; col++) { // 0 -> 2
      mIndexBuffer.push_back(col + (mRows*row));
      printf("%d, ", col + (mRows*row));
      mIndexBuffer.push_back(col + (mRows*(row+1)));
      printf("%d, ", col + (mRows*(row+1)));
    }
    
    // degenerate
    if (row != mRows-2) {
      mIndexBuffer.push_back(mColumns-1 + (mRows*(row+1))); // last value
      printf("%d\n", mColumns-1 + (mRows*(row+1)));

      mIndexBuffer.push_back(0 + (mRows*(row+1))); // first value
      printf("%d, ", 0 + (mRows*(row+1)));

    }
    
  }
  
  /*
   * Populate VERTEX buffer
   *
   * TODO: Fix pb w/ even number of colums/rows
   */
  std::vector<Vertex> vertexBuffer;
  
  short int halfRow = (mRows-1)/2;
  short int halfColumn = (mColumns-1)/2;
  
  int vertexIndex = 0;

//  Vertex vertexBuffer[324]; // = 3 * sizeof(vec3) * rowNumber * columnNumber = 3 * 12 * 9
  
  for (row = -halfRow; row <= halfRow; row++) {
    for (col = -halfColumn; col <= halfColumn; col++) {
      vec3 position = vec3(col, 0, row);
      vec3 colour;
      
      if ((vertexIndex+(row*2))%3==0) {
        position.y = 0.5;
        colour = vec3(1, 0, 0);
      } else if ((vertexIndex+(row*2))%3==1) {
        colour = vec3(0, 1, 0);
      } else {
        colour = vec3(0, 0, 1);
      }
      
      vertexBuffer.push_back( {
        position,
        vec3(0.0f, 1.0f, 0.0f),
        colour
        });
      vertexIndex++;
    }
  }
  

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), &vertexBuffer[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &mElementBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer.size() * sizeof(unsigned short), &mIndexBuffer[0] , GL_STATIC_DRAW);

}

WaterModel::~WaterModel(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void WaterModel::Update(float dt){
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second
	Model::Update(dt);
}

void WaterModel::Draw(){
	// Draw the Vertex Buffer
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	
	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	0,				// attribute. No particular reason for 0, but must match the layout in the shader.
							3,				// size
							GL_FLOAT,		// type
							GL_FALSE,		// normalized?
							sizeof(Vertex), // stride
							(void*)0        // array buffer offset
						);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	1,
							3,
							GL_FLOAT,
							GL_FALSE,
							sizeof(Vertex),
							(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
						);

	// 3rd attribute buffer : vertex color
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	2,
							3,
							GL_FLOAT,
							GL_FALSE,
							sizeof(Vertex),
							(void*) (2* sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
						);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);

	// Draw the triangles !
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
  
  glDrawElements(
                 GL_TRIANGLE_STRIP,      // mode
                 mIndexBuffer.size(),    // count
                 GL_UNSIGNED_SHORT,   // type
                 (void*)0           // element array buffer offset
                 );

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool WaterModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty())
		return true;
	else
		return Model::ParseLine(token);
}