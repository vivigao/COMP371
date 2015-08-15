// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#include "WaterModel.h"
#include "Renderer.h"
#include "Camera.h"
#include "World.h"

#include <math.h>       /* fmod */
#include <stdio.h>
// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
using namespace glm;

//	vec3 halfSize = size * 0.5f;

WaterModel::WaterModel(int textureID) : Model(){
  mTextureID = textureID;
  
  /*
   * Populate INDEX buffer
   */
  short int row, col;
  
  for (row=0; row<mRows-1; row++) { // 0 -> 1
    
    for (col=0; col<mColumns; col++) { // 0 -> 2
      mIndexBuffer.push_back(col + (mRows*row));
      mIndexBuffer.push_back(col + (mRows*(row+1)));
    }
    
    // degenerate
    if (row != mRows-2) {
      mIndexBuffer.push_back(mColumns-1 + (mRows*(row+1))); // last value
      mIndexBuffer.push_back(0 + (mRows*(row+1))); // first value
    }
    
  }
  
  /*
   * Populate VERTEX buffer
   *
   * TODO: Fix pb w/ even number of colums/rows
   */  
  short int halfRow = (mRows-1)/2;
  short int halfColumn = (mColumns-1)/2;
  
  int vertexIndex = 0;

//  Vertex vertexBuffer[324]; // = 3 * sizeof(vec3) * rowNumber * columnNumber = 3 * 12 * 9
  
  for (row = -halfRow; row <= halfRow; row++) {
    for (col = -halfColumn; col <= halfColumn; col++) {
      vec3 position = vec3(col, 0.0f, row);
      vec3 normal = vec3(0.0f, -1.0f, 0.0f);
      vec3 colour = vec3(0.0f, 0.3f, 1.0f);
      
//      if ((vertexIndex+(row*2))%3==0) {
////        position.y = 0.5f;
//        colour = vec3(0, 0, 1);
//      } else if ((vertexIndex+(row*2))%3==1) {
//        colour = vec3(0, 1, 0);
//      } else {
//        colour = vec3(0, 0, 1);
//      }
      
      vertexBuffer.push_back({position, normal, colour});
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
  
  waveTimeLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "waveTime");
  waveWidthLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "waveWidth");
  waveHeightLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "waveHeight");
}

WaterModel::~WaterModel(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void WaterModel::Update(float dt){
	Model::Update(dt);
  
  timer+=dt;
  
  float two_pies = 3.14f*2.0f;
//  float wavePosition = fmodf(timer, 2*pie);
//  float waveProgression = wavePosition / (2.0f*pie);
  int waveLength = 10;
  float waveSegmentLength = two_pies/(waveLength);
  
  float waveProgression = fmodf(timer, waveLength)/waveLength;
  
  for (int i=0; i<mColumns; i++) {
    for (int j=0; j<mRows; j++) {
      int index = i + (j*mColumns);
      float yCoord = sinf(((i%waveLength)*waveSegmentLength)+(waveProgression*two_pies));
      
      vertexBuffer[index].position.y = yCoord;
    }
  }

//  GLint waveTimeLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "waveTime");
//  GLint waveWidthLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "waveWidth");
//  GLint waveHeightLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "waveHeight");
//  glUniform1f(waveTimeLocation, waveTime);
//  glUniform1f(waveWidthLocation, waveWidth);
//  glUniform1f(waveHeightLocation, waveHeight);
//    waveTime += waveFreq;
////  
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), &vertexBuffer[0], GL_STATIC_DRAW);
}

void WaterModel::Draw(){
  Renderer::CheckForErrors();
  // Set current shader to be the Textured Shader
//  ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();
  
  Renderer::SetShader(SHADER_WATER);
  glUseProgram(Renderer::GetShaderProgramID());
  
  GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glUniform1i(textureLocation, 0);				// Set our Texture sampler to user Texture Unit 0
  
  // This looks for the MVP Uniform variable in the Vertex Program
  GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
  
  // Send the view projection constants to the shader
  const Camera* currentCamera = World::GetInstance()->GetCurrentCamera();
  mat4 VP = currentCamera->GetViewProjectionMatrix();
  glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
  
	// Draw the Vertex Buffer
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
  
  glUniform1f(waveTimeLocation, waveTime);
  glUniform1f(waveWidthLocation, waveWidth);
	glUniform1f(waveHeightLocation, waveHeight);
  
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
  
  // 4th attribute buffer : texture coordinates
  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glVertexAttribPointer(3,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void*) (2* sizeof(vec3) + sizeof(vec4)) // texture coordinates are Offseted by 2 vec3 (see class Vertex) and a vec4
                        );
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
  
  glDrawElements(
                 GL_TRIANGLE_STRIP,      // mode
                 (int) mIndexBuffer.size(),    // count
                 GL_UNSIGNED_SHORT,   // type
                 (void*)0           // element array buffer offset
                 );
  
  glDisableVertexAttribArray(3);
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